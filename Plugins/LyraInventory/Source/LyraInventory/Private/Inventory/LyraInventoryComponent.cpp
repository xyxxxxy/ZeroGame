
#include "Inventory/LyraInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "LyraInventoryLibrary.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Processors/LyraInventoryProcessor_Bag.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryComponent)

ULyraInventoryComponent::ULyraInventoryComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

UAbilitySystemComponent* ULyraInventoryComponent::GetOwnerAbilitySystemComponent()
{
	if(AbilitySystemComponent == nullptr)
	{
		if(AActor* Actor = GetOwner())
		{
			AbilitySystemComponent = Cast<UAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
		}
	}
	return AbilitySystemComponent;
}

void ULyraInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OnItemSlotUpdateDelegate().RemoveAll(this);
	OnItemSlotUpdateDelegate().AddUObject(this, &ULyraInventoryComponent::OnItemSlotUpdate);

	BP_OnItemSlotUpdate.RemoveDynamic(this, &ULyraInventoryComponent::OnItemSlotUpdate);
	BP_OnItemSlotUpdate.AddDynamic(this, &ULyraInventoryComponent::OnItemSlotUpdate);

	ForEachProcessor([](ULyraInventoryProcessor* Processor)
		{
			Processor->OnInventoryInit();
		});
}

void ULyraInventoryComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void ULyraInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// TODO: Create slot by processor ???
	ForEachProcessor([](ULyraInventoryProcessor* Processor)
	{
		Processor->OnInventoryBeginPlay();	
	}
	);
}

void ULyraInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ULyraInventoryComponent::BindToASC()
{
	ForEachProcessor([](ULyraInventoryProcessor* Processor)
{
	Processor->BindToASC();
});
}

void ULyraInventoryComponent::UnbindFromASC()
{
	if (!GetOwner()->HasActorBegunPlay())
	{
		return;
	}
	ForEachProcessor([](ULyraInventoryProcessor* Processor)
	{
		Processor->UnbindFromASC();
	});
}

void ULyraInventoryComponent::ForEachProcessor(ForEachProcessorFunc Func)
{
	for(const auto& [_, ProcessorsArray] : CategoryToProcessors)
	{
		for (ULyraInventoryProcessor* Processor : ProcessorsArray.Processors)
		{
			if (IsValid(Processor))
			{
				Func(Processor);
			}
		}
	}
}

ULyraInventoryProcessor* ULyraInventoryComponent::AddProcessor(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, const FGameplayTagContainer& ProcessorTags, ELyraItemCategory InItemCategory)
{
	if (!IsValid(ProcessorClass))
	{
		return nullptr;
	}

	ULyraInventoryProcessor* NewProcessor = NewObject<ULyraInventoryProcessor>(this, ProcessorClass);
	//NewProcessor->FragmentTags = ProcessorTags;
	//NewProcessor->ItemCategory = InItemCategory;
	// 1
	CategoryToProcessors[InItemCategory].Processors.Add(NewProcessor);

	if (IsUsingRegisteredSubObjectList())
	{
		AddReplicatedSubObject(NewProcessor);
	}

	NewProcessor->OnInventoryInit();
	if (HasBegunPlay())
	{
		NewProcessor->OnInventoryBeginPlay();
	}

	return NewProcessor;
}

ULyraInventoryProcessor* ULyraInventoryComponent::FindFirstProcessor(ProcessorQueryPredicate Predicate) const
{
	// 1
	for(const auto& [Category, ProcessorArray] : CategoryToProcessors)
	{
		for (ULyraInventoryProcessor* P : ProcessorArray.Processors)
		{
			if (IsValid(P) && Predicate(P, Category))
			{
				return P;
			}
		}
	}

	return nullptr;
}

void ULyraInventoryComponent::FindAllProcessors(TArray<ULyraInventoryProcessor*>& OutProcessors, ProcessorQueryPredicate Predicate) const
{
	// 1
	for(const auto& [Category, ProcessorArray] : CategoryToProcessors)
	{
		for (ULyraInventoryProcessor* P : ProcessorArray.Processors)
		{
			if (IsValid(P) && Predicate(P, Category))
			{
				OutProcessors.Add(P);
			}
		}
	}
}

ULyraInventoryProcessor* ULyraInventoryComponent::FindFirstProcessorMatchingCategory(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, ELyraItemCategory ItemCategory)
{
	return FindFirstProcessor([&ProcessorClass, &ItemCategory](const ULyraInventoryProcessor* Processor, ELyraItemCategory Category) -> bool {
		return Processor->IsA(ProcessorClass) && Category == ItemCategory;
	});
}

bool ULyraInventoryComponent::LootItem(ULyraInventoryItemInstance* Item, FGameplayTag EventTag)
{
	FLyraInventoryTableRowFragment_UI Fragment_UI;
	if(ULyraInventoryLibrary::FindFragment_UI(Item, Fragment_UI))
	{
		if(ULyraInventoryProcessor_Bag* Processor_Bag = GetProcessorBag(Fragment_UI.ItemCategory))
		{
			for(auto& Slot : Processor_Bag->Inventory.Slots)
			{
				FLyraInventoryItemSlotHandle Handle = FLyraInventoryItemSlotHandle(Slot, this);
				if(AcceptsItem(Item, Handle))
				{
					if(PlaceItemIntoSlot(Item, Handle))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool ULyraInventoryComponent::PlaceItemIntoSlot(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle)
{
	if(!IsValidItemSlot(ItemHandle))return false;
	
	FLyraInventoryItemSlot& Slot = GetItemSlot(ItemHandle);
	
	ULyraInventoryItemInstance* PreviousItem = Slot.ItemInstance;

	// merge
	if(PreviousItem)
	{
		if(PreviousItem->CanStackWith(Item))
		{
			return PreviousItem->MergeItemStacks(Item);
		}
	}
	
	Slot.ItemInstance = Item;

	OnInventoryUpdateDelegate().Broadcast(this);
	BP_OnInventoryUpdate.Broadcast(this);
	
	//OnItemSlotUpdateDelegate().Broadcast(this, ItemHandle, Item, PreviousItem);
	//BP_OnItemSlotUpdate.Broadcast(this, ItemHandle, Item, PreviousItem);
	
	return true;
}

bool ULyraInventoryComponent::RemoveAllItemsFromInventory(TArray<ULyraInventoryItemInstance*>& OutItemsRemoved)
{
	return true;
}

bool ULyraInventoryComponent::RemoveItemFromInventory(const FLyraInventoryItemSlotHandle& ItemHandle)
{
	if(!IsValidItemSlot(ItemHandle))return false;

	FLyraInventoryItemSlot& ItemSlot = GetItemSlot(ItemHandle);
	ULyraInventoryItemInstance* PreviousItem = ItemSlot.ItemInstance;
	
	if(!IsValid(ItemSlot.ItemInstance))return false;
	
	ItemSlot.ItemInstance = nullptr;

	OnInventoryUpdateDelegate().Broadcast(this);
	BP_OnInventoryUpdate.Broadcast(this);
	
	OnItemSlotUpdateDelegate().Broadcast(this, ItemHandle,ItemSlot.ItemInstance, PreviousItem);
	BP_OnItemSlotUpdate.Broadcast(this, ItemHandle, ItemSlot.ItemInstance, PreviousItem);
	
	return true;
}

ULyraInventoryItemInstance* ULyraInventoryComponent::GetItemInstanceBySlot(const FLyraInventoryItemSlotHandle& ItemHandle)
{
	if(!IsValidItemSlot(ItemHandle))return nullptr;
	
	const FLyraInventoryItemSlot& ItemSlot = GetItemSlot(ItemHandle);
	return ItemSlot.ItemInstance;
}

FLyraInventoryItemSlot& ULyraInventoryComponent::GetItemSlot(const FLyraInventoryItemSlotHandle& Handle)
{
	check(IsValidItemSlot(Handle));
	if(ULyraInventoryProcessor_Bag* Processor_Bag = GetProcessorBag(Handle.ItemCategory))
	{
		for(auto& Slot : Processor_Bag->Inventory.Slots)
		{
			if(Handle == Slot)
			{
				return Slot;
			}
		}
	}
	return GuardSlot;
}

bool ULyraInventoryComponent::IsValidItemSlot(const FLyraInventoryItemSlotHandle& Handle)
{
	if(ULyraInventoryProcessor_Bag* Processor_Bag = GetProcessorBag(Handle.ItemCategory))
	{
		for(const auto& SlotHandle : Processor_Bag->AllSlotHandles)
		{
			if(Handle == SlotHandle)
			{
				return true;
			}
		}
	}
	return false;
}

bool ULyraInventoryComponent::AcceptsItem(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle)
{
	if(!AcceptsItem_AssumeEmptySlot(Item, ItemHandle))
	{
		return false;
	}
	const FLyraInventoryItemSlot& Slot = GetItemSlot(ItemHandle);
	if(IsValid(Slot.ItemInstance) && *Item->GetItemStructHandle().Data.Get() != *Slot.ItemInstance->GetItemStructHandle().Data.Get())
	{
		return false;
	}
	return true;
}

bool ULyraInventoryComponent::AcceptsItem_AssumeEmptySlot(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle)
{
	if(!IsValidItemSlot(ItemHandle))
	{
		return false;
	}

	const FLyraInventoryItemSlot& Slot = GetItemSlot(ItemHandle);

	if(IsValid(Slot.ItemSlotFilter) && IsValid(Item))
	{
		return Slot.ItemSlotFilter.AcceptsItem(Item);
	}
	return true;
}

void ULyraInventoryComponent::RemoveInventorySlot(const FLyraInventoryItemSlotHandle& Handle)
{
	check(IsValidItemSlot(Handle));
	if(ULyraInventoryProcessor_Bag* Processor_Bag = GetProcessorBag(Handle.ItemCategory))
	{
		for(auto& Slot : Processor_Bag->Inventory.Slots)
		{
			if(Handle == Slot)
			{
				Processor_Bag->Inventory.Slots.Remove(Slot);
				Processor_Bag->PostInventoryUpdate(Processor_Bag->Inventory,Processor_Bag->AllSlotHandles);
			}
		}
	}
}


void ULyraInventoryComponent::BulkCreateInventorySlots(ELyraItemCategory Category, const FGameplayTagContainer& SlotTags,
                                                       const FLyraInventoryItemFilterHandle& Filter, int32 Count, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles)
{
	if(ULyraInventoryProcessor* Processor = FindFirstProcessorMatchingCategory(ULyraInventoryProcessor::StaticClass(), Category))
	{
		//Processor->BulkCreateInventorySlots()
	}
	
}

void ULyraInventoryComponent::OnItemSlotUpdate(ULyraInventoryComponent* InventoryComponent, const FLyraInventoryItemSlotHandle& SlotHandle, ULyraInventoryItemInstance* CurrentItem, ULyraInventoryItemInstance* PreviousItem)
{
	ForEachProcessor([InventoryComponent, SlotHandle, CurrentItem, PreviousItem](ULyraInventoryProcessor* Processor) 
		{
			Processor->OnItemSlotChange(SlotHandle, CurrentItem, PreviousItem);
		});
}


bool ULyraInventoryComponent::Query_GetAllSlots(const FLyraInventoryQuery& Query, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles)
{
	if(ULyraInventoryProcessor_Bag* Processor_Bag = GetProcessorBag(Query.Category))
	{
		for(auto& ItemSlot : Processor_Bag->Inventory.Slots)
		{
			if(Query.MatchesSlot(ItemSlot))
			{
				OutSlotHandles.Add(FLyraInventoryItemSlotHandle(ItemSlot, this));
			}
		}
	}
	return OutSlotHandles.Num() > 0;
}


ULyraInventoryProcessor_Bag* ULyraInventoryComponent::GetProcessorBag(ELyraItemCategory Category)
{
	return FindFirstProcessorMatchingCategory<ULyraInventoryProcessor_Bag>(Category);
}

