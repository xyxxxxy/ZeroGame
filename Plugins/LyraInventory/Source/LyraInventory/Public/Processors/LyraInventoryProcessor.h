// Copyright Garrett Fleenor & Puny Human %Y. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/EngineTypes.h"
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "LyraInventoryProcessor.generated.h"

class UAbilitySystemComponent;

/**
 *
 */
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew, DefaultToInstanced)
class LYRAINVENTORY_API ULyraInventoryProcessor : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	ULyraInventoryProcessor();
	
	UFUNCTION(BlueprintPure, Category="Inventory")
	ULyraInventoryComponent* GetOwningInventory() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UAbilitySystemComponent* GetOwningAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	AActor* GetOwningActor() const;
	ENetRole GetOwnerRole() const;
	inline bool HasAuthority() const { return (GetOwnerRole() == ROLE_Authority); }

	UFUNCTION(BlueprintNativeEvent)
	void OnInventoryInit();
	virtual void OnInventoryInit_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnInventoryBeginPlay();
	virtual void OnInventoryBeginPlay_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnInventoryEndPlay(const EEndPlayReason::Type EndPlayReason);
	virtual void OnInventoryEndPlay_Implementation(const EEndPlayReason::Type EndPlayReason) {}
	
	UFUNCTION(BlueprintNativeEvent)
	void BindToASC();
	virtual void BindToASC_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void UnbindFromASC();
	virtual void UnbindFromASC_Implementation() {}

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	// Inventory
	UFUNCTION(BlueprintNativeEvent)
	void OnItemSlotChange(const FLyraInventoryItemSlotHandle& SlotHandle, ULyraInventoryItemInstance* Item, ULyraInventoryItemInstance* PreviousItem);
	virtual void OnItemSlotChange_Implementation(const FLyraInventoryItemSlotHandle& SlotHandle, ULyraInventoryItemInstance* Item, ULyraInventoryItemInstance* PreviousItem) {}
	

	// check
	// UFUNCTION(BlueprintCallable, Category = "Inventory")
	// void RemoveInventorySlot(const FLyraInventoryItemSlotHandle& SlotHandle){};

	//virtual bool RemoveItemFromInventory(const FLyraInventoryItemSlotHandle &ItemHandle);

	UFUNCTION(BlueprintNativeEvent)
	void OnInventorySlotUpdate();
	virtual void OnInventorySlotUpdate_Implementation() {}


	UFUNCTION(BlueprintCallable, Category = "Lyra|Inventory", Meta = (HidePin = "bEvenIfPendingKill"))
	virtual ULyraInventoryItemInstance* GetItemInSlot(const FLyraInventoryItemSlotHandle& SlotHandle, bool bEvenIfPendingKill = false) const;

public:
	virtual void BulkCreateInventorySlots( const FGameplayTagContainer& SlotTags, const FLyraInventoryItemFilterHandle& Filter, int32 Count, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles);
	virtual void BulkRemoveInventorySlots(const TArray<FLyraInventoryItemSlotHandle>& SlotHandles){};

protected:
	// virtual void PostInventoryUpdate(FAuraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles){};
	// virtual void PopulateSlotReferenceArray(FAuraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles){};

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FGameplayTagContainer FragmentTags;

	UFUNCTION(BlueprintPure, Category = "Lyra|Inventory")
	bool HasBegunPlay() const;
	
	virtual void GetDebugStrings(TArray<FString>& OutStrings, bool Detailed) const { }



};


