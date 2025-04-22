
#include "Inventory/LyraInventoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "LyraInventoryLibrary.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "Processors/LyraInventoryProcessor_Bag.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryComponent)

ULyraInventoryComponent::ULyraInventoryComponent(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
	IdCounter = 0;
}

UAbilitySystemComponent *ULyraInventoryComponent::GetOwnerAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr)
	{
		if (AActor *Actor = GetOwner())
		{
			AbilitySystemComponent = Cast<UAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
		}
	}
	return AbilitySystemComponent;
}

void ULyraInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// TODO : check delegate?
	OnItemSlotUpdateDelegate().RemoveAll(this);
	OnItemSlotUpdateDelegate().AddUObject(this, &ULyraInventoryComponent::OnItemSlotUpdate);

	BP_OnItemSlotUpdate.RemoveDynamic(this, &ULyraInventoryComponent::OnItemSlotUpdate);
	BP_OnItemSlotUpdate.AddDynamic(this, &ULyraInventoryComponent::OnItemSlotUpdate);

	ForEachProcessor([](ULyraInventoryProcessor *Processor)
			 { Processor->OnInventoryInit(); });
}

void ULyraInventoryComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void ULyraInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// TODO: Create slot by processor ???
	ForEachProcessor([](ULyraInventoryProcessor *Processor)
			 { Processor->OnInventoryBeginPlay(); });
}

void ULyraInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ULyraInventoryComponent::BindToASC()
{
	ForEachProcessor([](ULyraInventoryProcessor *Processor)
			 { Processor->BindToASC(); });
}

void ULyraInventoryComponent::UnbindFromASC()
{
	if (!GetOwner()->HasActorBegunPlay())
	{
		return;
	}
	ForEachProcessor([](ULyraInventoryProcessor *Processor)
			 { Processor->UnbindFromASC(); });
}

void ULyraInventoryComponent::ForEachProcessor(ForEachProcessorFunc Func)
{
	for (const auto &Processor : Processors)
	{
		if (IsValid(Processor))
		{
			Func(Processor);
		}
	}
}

ULyraInventoryProcessor *ULyraInventoryComponent::AddProcessor(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, const FGameplayTagContainer &ProcessorTags)
{
	if (!IsValid(ProcessorClass))
	{
		return nullptr;
	}

	ULyraInventoryProcessor *NewProcessor = NewObject<ULyraInventoryProcessor>(this, ProcessorClass);
	NewProcessor->FragmentTags = ProcessorTags;

	Processors.Add(NewProcessor);

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

ULyraInventoryProcessor *ULyraInventoryComponent::FindFirstProcessor(ProcessorQueryPredicate Predicate) const
{
	for (const auto &Processor : Processors)
	{
		if (IsValid(Processor))
		{
			return Processor;
		}
	}
	return nullptr;
}

void ULyraInventoryComponent::FindAllProcessors(TArray<ULyraInventoryProcessor *> &OutProcessors, ProcessorQueryPredicate Predicate) const
{
	for (const auto &Processor : Processors)
	{
		if (IsValid(Processor))
		{
			OutProcessors.Add(Processor);
		}
	}
}

ULyraInventoryProcessor *ULyraInventoryComponent::K2_FindFirstProcessor(TSubclassOf<ULyraInventoryProcessor> ProcessorClass)
{
	return FindFirstProcessor([&ProcessorClass](ULyraInventoryProcessor *Processor) -> bool
				  { return Processor->IsA(ProcessorClass); });
}

ULyraInventoryProcessor *ULyraInventoryComponent::K2_FindFirstProcessorMatchingTags(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, FGameplayTagQuery Query)
{
	return FindFirstProcessor([&ProcessorClass, &Query](ULyraInventoryProcessor *Processor) -> bool
				  {
		FGameplayTagContainer Tags;
		Processor->GetOwnedGameplayTags(Tags);
		return Processor->IsA(ProcessorClass) && Query.Matches(Tags); });
}

TArray<ULyraInventoryProcessor *> ULyraInventoryComponent::K2_FindAllProcessors(TSubclassOf<ULyraInventoryProcessor> ProcessorClass)
{
	TArray<ULyraInventoryProcessor *> OutProcessors;
	FindAllProcessors(OutProcessors,
			  [&ProcessorClass](ULyraInventoryProcessor *Processor) -> bool
			  {
				  return Processor->IsA(ProcessorClass);
			  });

	return OutProcessors;
}

TArray<ULyraInventoryProcessor *> ULyraInventoryComponent::K2_FindAllProcessorsMatchingTags(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, FGameplayTagQuery Query)
{
	TArray<ULyraInventoryProcessor *> OutProcessors;
	FindAllProcessors(OutProcessors,
			  [&ProcessorClass, &Query](ULyraInventoryProcessor *Processor) -> bool
			  {
				  FGameplayTagContainer Tags;
				  Processor->GetOwnedGameplayTags(Tags);
				  return Processor->IsA(ProcessorClass) && Query.Matches(Tags);
			  });

	return OutProcessors;
}

bool ULyraInventoryComponent::LootItem(ULyraInventoryItemInstance *Item, FGameplayTag EventTag)
{
	FLyraInventoryTableRowFragment_UI Fragment_UI;
	if (ULyraInventoryLibrary::FindFragment_UI(Item, Fragment_UI))
	{
		FGameplayTagContainer TagContainer;
		Item->GetOwnedGameplayTags(TagContainer);
		// TODO: 缩小查询范围
		for (auto &Slot : Inventory.Slots)
		{
			FLyraInventoryItemSlotHandle Handle = FLyraInventoryItemSlotHandle(Slot, this);
			if (AcceptsItem(Item, Handle))
			{
				if (PlaceItemIntoSlot(Item, Handle))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ULyraInventoryComponent::PlaceItemIntoSlot(ULyraInventoryItemInstance *Item, const FLyraInventoryItemSlotHandle &ItemHandle)
{
	if (!IsValidItemSlot(ItemHandle))
		return false;

	FLyraInventoryItemSlot &Slot = GetItemSlot(ItemHandle);

	ULyraInventoryItemInstance *PreviousItem = Slot.ItemInstance;

	// merge
	if (PreviousItem)
	{
		if (PreviousItem->CanStackWith(Item))
		{
			return PreviousItem->MergeItemStacks(Item);
		}
	}

	Slot.ItemInstance = Item;

	OnInventoryUpdateDelegate().Broadcast(this);
	OnItemSlotUpdateDelegate().Broadcast(this, FLyraInventoryPayload(ItemHandle, Item, PreviousItem));
	BP_OnInventoryUpdate.Broadcast(this);
	BP_OnItemSlotUpdate.Broadcast(this, FLyraInventoryPayload(ItemHandle, Item, PreviousItem));

	return true;
}

bool ULyraInventoryComponent::RemoveAllItemsFromInventory(TArray<ULyraInventoryItemInstance *> &OutItemsRemoved)
{
	return true;
}

bool ULyraInventoryComponent::RemoveItemFromInventory(const FLyraInventoryItemSlotHandle &ItemHandle, bool bNeedSort)
{
	if (!IsValidItemSlot(ItemHandle))
		return false;

	FLyraInventoryItemSlot &ItemSlot = GetItemSlot(ItemHandle);
	ULyraInventoryItemInstance *PreviousItem = ItemSlot.ItemInstance;

	if (!IsValid(ItemSlot.ItemInstance))
		return false;

	ItemSlot.ItemInstance = nullptr;

	// TODO : sort after removing item?
	// if (bNeedSort)
	// {
	// 	int32 Index = Inventory.Slots.Find(ItemSlot);
	// 	if (Index != INDEX_NONE)
	// 	{
	// 	}
	// }

	OnInventoryUpdateDelegate().Broadcast(this);
	BP_OnInventoryUpdate.Broadcast(this);

	OnItemSlotUpdateDelegate().Broadcast(this, FLyraInventoryPayload(ItemHandle, ItemSlot.ItemInstance, PreviousItem));
	BP_OnItemSlotUpdate.Broadcast(this, FLyraInventoryPayload(ItemHandle, ItemSlot.ItemInstance, PreviousItem));

	return true;
}

ULyraInventoryItemInstance *ULyraInventoryComponent::GetItemInstanceBySlot(const FLyraInventoryItemSlotHandle &ItemHandle)
{
	if (!IsValidItemSlot(ItemHandle))
		return nullptr;

	const FLyraInventoryItemSlot &ItemSlot = GetItemSlot(ItemHandle);
	return ItemSlot.ItemInstance;
}

FLyraInventoryItemSlot &ULyraInventoryComponent::GetItemSlot(const FLyraInventoryItemSlotHandle &Handle)
{
	check(IsValidItemSlot(Handle));

	for (auto &Slot : Inventory.Slots)
	{
		if (Handle == Slot)
		{
			return Slot;
		}
	}

	return Inventory.Slots[0];
}

bool ULyraInventoryComponent::IsValidItemSlot(const FLyraInventoryItemSlotHandle &Handle)
{
	for (const auto &SlotHandle : AllSlotHandles)
	{
		if (Handle == SlotHandle)
		{
			return true;
		}
	}

	return false;
}

bool ULyraInventoryComponent::AcceptsItem(ULyraInventoryItemInstance *Item, const FLyraInventoryItemSlotHandle &ItemHandle)
{
	if (!AcceptsItem_AssumeEmptySlot(Item, ItemHandle))
	{
		return false;
	}
	const FLyraInventoryItemSlot &Slot = GetItemSlot(ItemHandle);
	if (IsValid(Slot.ItemInstance) && *Item->GetItemStructHandle().Data.Get() != *Slot.ItemInstance->GetItemStructHandle().Data.Get())
	{
		return false;
	}
	return true;
}

bool ULyraInventoryComponent::AcceptsItem_AssumeEmptySlot(ULyraInventoryItemInstance *Item, const FLyraInventoryItemSlotHandle &ItemHandle)
{
	if (!IsValidItemSlot(ItemHandle))
	{
		return false;
	}

	const FLyraInventoryItemSlot &Slot = GetItemSlot(ItemHandle);

	if (IsValid(Slot.ItemSlotFilter) && IsValid(Item))
	{
		return Slot.ItemSlotFilter.AcceptsItem(Item);
	}
	return true;
}

void ULyraInventoryComponent::RemoveInventorySlot(const FLyraInventoryItemSlotHandle &Handle)
{
	check(IsValidItemSlot(Handle));

	for (auto &Slot : Inventory.Slots)
	{
		if (Handle == Slot)
		{
			Inventory.Slots.Remove(Slot);
			PostInventoryUpdate(Inventory, AllSlotHandles);
		}
	}
}

void ULyraInventoryComponent::BulkCreateInventorySlots(const FGameplayTagContainer &SlotTags,
						       const FLyraInventoryItemFilterHandle &Filter, int32 Count, TArray<FLyraInventoryItemSlotHandle> &OutSlotHandles)
{
	for (int i = 0; i < Count; i++)
	{
		FLyraInventoryItemSlot Slot;
		Slot.SlotId = IdCounter;
		Slot.SlotTags = SlotTags;
		Slot.ItemSlotFilter = Filter;
		Slot.Owner = this;
		Inventory.Slots.Add(Slot);
		IdCounter++;

		OutSlotHandles.Add(FLyraInventoryItemSlotHandle(Slot, this));
	}

	PostInventoryUpdate(Inventory, OutSlotHandles);
}

void ULyraInventoryComponent::PostInventoryUpdate(FLyraInventoryItemArray &InventoryRef, TArray<FLyraInventoryItemSlotHandle> &SlotHandles)
{
	SlotHandles.Empty(SlotHandles.Num() + 1);
	PopulateSlotReferenceArray(InventoryRef, SlotHandles);
}

void ULyraInventoryComponent::PopulateSlotReferenceArray(FLyraInventoryItemArray &InventoryRef, TArray<FLyraInventoryItemSlotHandle> &SlotHandles)
{
	for (int i = 0; i < InventoryRef.Slots.Num(); ++i)
	{
		FLyraInventoryItemSlotHandle Handle(InventoryRef.Slots[i], this);
		SlotHandles.Add(Handle);
	}
}

bool ULyraInventoryComponent::UseItem(const FLyraInventoryItemSlotHandle &ItemHandle)
{
	RemoveItemFromInventory(ItemHandle);
	return true;
}

bool ULyraInventoryComponent::RemoveItems(const FLyraInventoryItemSlotHandle &ItemHandle, int32 Number)
{
	return true;
}

bool ULyraInventoryComponent::Query_GetAllSlotHandles(const FLyraInventoryQuery &Query, TArray<FLyraInventoryItemSlotHandle> &OutSlotHandles)
{

	for (auto &ItemSlot : Inventory.Slots)
	{
		if (Query.MatchesSlot(ItemSlot))
		{
			OutSlotHandles.Add(FLyraInventoryItemSlotHandle(ItemSlot, this));
		}
	}

	return OutSlotHandles.Num() > 0;
}

bool ULyraInventoryComponent::Query_GetAllSlots(const FLyraInventoryQuery &Query, TArray<FLyraInventoryItemSlot> &OutSlots)
{
	TArray<int32> Indices;
	TArray<int32> Instances;

	for (int i = 0; i < Inventory.Slots.Num(); ++i)
	{
		if (Query.MatchesSlot(Inventory.Slots[i]))
		{
			if (IsValid(Inventory.Slots[i].ItemInstance))
				Instances.Add(i);
			Indices.Add(i);
		}
	}
	int i = 0, j = 0;
	while (i < Indices.Num() && j < Instances.Num())
	{
		if (IsValid(Inventory.Slots[Indices[i]].ItemInstance))
		{
			i++;
		}
		else
		{
			// TODO: check SlotFiler
			Inventory.Slots[Indices[i]].ItemInstance = Inventory.Slots[Instances[j]].ItemInstance;
			Inventory.Slots[Instances[j]].ItemInstance = nullptr;
		}
		OutSlots.Add(Inventory.Slots[Indices[i]]);
		j++;
	}

	return OutSlots.Num() > 0;
}

bool ULyraInventoryComponent::Query_GetAllItems(const FLyraInventoryQuery &Query, TArray<ULyraInventoryItemInstance *> &OutItems)
{
	for (auto &ItemSlot : Inventory.Slots)
	{
		if (Query.MatchesSlot(ItemSlot))
		{
			OutItems.Add(ItemSlot.ItemInstance);
		}
	}

	return OutItems.Num() > 0;
}

void ULyraInventoryComponent::OnItemSlotUpdate(ULyraInventoryComponent *InventoryComponent, const FLyraInventoryPayload &Payload)
{
	ForEachProcessor([InventoryComponent, Payload](ULyraInventoryProcessor *Processor)
			 { Processor->OnItemSlotChange(Payload.SlotHandle, Payload.CurrentItem, Payload.PreviousItem); });
}