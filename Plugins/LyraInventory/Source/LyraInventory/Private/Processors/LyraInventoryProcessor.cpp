
#include "Processors/LyraInventoryProcessor.h"

#include "Inventory/LyraInventoryComponent.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventoryProcessor)

ULyraInventoryProcessor::ULyraInventoryProcessor()
{
}

ULyraInventoryComponent* ULyraInventoryProcessor::GetOwningInventory() const
{
	return CastChecked<ULyraInventoryComponent>(GetOuter());
}

UAbilitySystemComponent* ULyraInventoryProcessor::GetOwningAbilitySystemComponent() const
{
	return GetOwningInventory()->GetOwnerAbilitySystemComponent();
}

AActor* ULyraInventoryProcessor::GetOwningActor() const
{
	return GetOwningInventory()->GetOwner();
}

ENetRole ULyraInventoryProcessor::GetOwnerRole() const
{
	return GetOwningInventory()->GetOwnerRole();
}


void ULyraInventoryProcessor::RemoveInventorySlot(const FLyraInventoryItemSlotHandle& SlotHandle)
{
	GetOwningInventory()->RemoveInventorySlot(SlotHandle);
}

ULyraInventoryItemInstance* ULyraInventoryProcessor::GetItemInSlot(const FLyraInventoryItemSlotHandle& SlotHandle,
	bool bEvenIfPendingKill) const
{
	return GetOwningInventory()->GetItemInstanceBySlot(SlotHandle);
}

bool ULyraInventoryProcessor::HasBegunPlay() const
{
	return GetOwningInventory()->HasBegunPlay();
}

