
#include "AbilitySystem/Tasks/LyraAbilityTask_DropItem.h"

UScriptStruct* FGameplayAbilityTargetData_DropItem::GetScriptStruct() const
{
	return FGameplayAbilityTargetData_DropItem::StaticStruct();
}

FString FGameplayAbilityTargetData_DropItem::ToString() const
{
	return TEXT("FGameplayAbilityTargetData_DropItem");
}

bool FGameplayAbilityTargetData_DropItem::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	// SlotHandle Net
	return true;
}

ULyraAbilityTask_DropItem::ULyraAbilityTask_DropItem(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

ULyraAbilityTask_DropItem* ULyraAbilityTask_DropItem::DropItemFromInventory(UGameplayAbility* OwningAbility,
	const FLyraInventoryItemSlotHandle& FromSlot)
{
	ULyraAbilityTask_DropItem* Task = NewObject<ULyraAbilityTask_DropItem>(OwningAbility);
	Task->SlotReference = FromSlot;
	return Task;
}

FGameplayAbilityTargetDataHandle ULyraAbilityTask_DropItem::GenerateTargetHandle()
{
	FGameplayAbilityTargetData_DropItem* ItemSwitchData = new FGameplayAbilityTargetData_DropItem();
	ItemSwitchData->FromSlot = SlotReference;
	return Super::GenerateTargetHandle();
}

void ULyraAbilityTask_DropItem::HandleTargetData(const FGameplayAbilityTargetDataHandle& Data)
{
	const FGameplayAbilityTargetData_DropItem* SwitchData = static_cast<const FGameplayAbilityTargetData_DropItem*>(Data.Get(0));
	if(SwitchData != nullptr)
	{
		OnDataReceived.Broadcast(SwitchData->FromSlot);
	}
	else
	{
		OnDataCancelled.Broadcast();
	}
}

void ULyraAbilityTask_DropItem::HandleCancelled()
{
	OnDataCancelled.Broadcast();
}
