
#include "Processors/LyraInventoryProcessor_Bag.h"
#include "Inventory/LyraInventoryComponent.h"
#include "LyraInventory/LyraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryProcessor_Bag)

void ULyraInventoryProcessor_Bag::OnInventoryBeginPlay_Implementation()
{
	Super::OnInventoryBeginPlay_Implementation();

	UE_LOG(LogLyraInventorySystem, Warning, TEXT("%s : OnInventoryBeginPlay"), *GetNameSafe(this));
	InitInventorySlots();
}

void ULyraInventoryProcessor_Bag::BulkCreateInventorySlots(ELyraItemCategory Category,
	const FGameplayTagContainer& SlotTags, const FLyraInventoryItemFilterHandle& Filter, int32 Count,
	TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles)
{
	
}

void ULyraInventoryProcessor_Bag::PostInventoryUpdate(FAuraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles)
{
	SlotHandles.Empty(SlotHandles.Num() + 1);
	PopulateSlotReferenceArray(InventoryRef, SlotHandles);
}

void ULyraInventoryProcessor_Bag::PopulateSlotReferenceArray(FAuraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles)
{
	for (int i = 0; i < InventoryRef.Slots.Num(); ++i)
	{
		FLyraInventoryItemSlotHandle Handle(InventoryRef.Slots[i], GetOwningInventory());
		SlotHandles.Add(Handle);
	}
}

void ULyraInventoryProcessor_Bag::InitInventorySlots()
{
	FLyraInventoryItemFilterHandle FilterHandle = FLyraInventoryItemFilterHandle(new FLyraInventoryItemSlotFilter(SlotFilter));
	for(int32 Index = 0; Index < BagSlotCount; ++Index)
	{
		FLyraInventoryItemSlot Slot;
		Slot.ItemInstance = nullptr;
		Slot.SlotTags = BagSlotTags;
		Slot.ItemSlotFilter = FilterHandle;
		Slot.ItemCategory = ItemCategory;
		Slot.SlotId = IdCounter;
		Slot.Owner = GetOwningInventory();
		IdCounter++;
		
		Inventory.Slots.Add(Slot);
	}
	PostInventoryUpdate(Inventory, AllSlotHandles);
}
