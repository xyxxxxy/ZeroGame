
#include "Processors/LyraInventoryProcessor_Bag.h"
#include "Inventory/LyraInventoryComponent.h"
#include "LyraInventory/LyraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryProcessor_Bag)

void ULyraInventoryProcessor_Bag::OnInventoryBeginPlay_Implementation()
{
	Super::OnInventoryBeginPlay_Implementation();

	UE_LOG(LogLyraInventorySystem, Warning, TEXT("OnInventoryBeginPlay: %s-CreateInventorySlots"), *GetNameSafe(this));
	//InitInventorySlots();
	//TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles;
	BulkCreateInventorySlots(SlotTags, FLyraInventoryItemFilterHandle(new FLyraInventoryItemSlotFilter(SlotFilter)), BagSlotCount, GetOwningInventory()->AllSlotHandles);
}

