
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Tasks/LyraAbilityTask_SimpleTarget.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "LyraAbilityTask_DropItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLyraDropItemRecieved, const FLyraInventoryItemSlotHandle&, FromSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLyraDropItemCancelled);

USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_DropItem : public FGameplayAbilityTargetData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FLyraInventoryItemSlotHandle FromSlot;

	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FString ToString() const override;
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};
template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_DropItem> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_DropItem>
{
	enum
	{
		WithNetSerializer = true
	};
};

UCLASS()
class LYRAINVENTORY_API ULyraAbilityTask_DropItem : public ULyraAbilityTask_SimpleTarget
{
	GENERATED_BODY()
public:
	ULyraAbilityTask_DropItem(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", HideSpawnParms = "Instigator"), Category = "Ability|Tasks")
	static ULyraAbilityTask_DropItem* DropItemFromInventory(UGameplayAbility* OwningAbility, const FLyraInventoryItemSlotHandle& FromSlot);

	virtual FGameplayAbilityTargetDataHandle GenerateTargetHandle() override;
	virtual void HandleTargetData(const FGameplayAbilityTargetDataHandle& Data) override;
	virtual void HandleCancelled() override;

	UPROPERTY(BlueprintAssignable)
	FLyraDropItemRecieved OnDataReceived;
	
	UPROPERTY(BlueprintAssignable)
	FLyraDropItemCancelled OnDataCancelled;
	
protected:
	FLyraInventoryItemSlotHandle SlotReference;
};
