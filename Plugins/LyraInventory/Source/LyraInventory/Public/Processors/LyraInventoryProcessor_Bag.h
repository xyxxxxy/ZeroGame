
#pragma once

#include "CoreMinimal.h"
#include "LyraInventoryProcessor.h"
#include "LyraInventoryProcessor_Bag.generated.h"

class ULyraInventoryComponent;

UCLASS()
class ULyraInventoryProcessor_Bag : public ULyraInventoryProcessor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	int32 BagSlotCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FGameplayTagContainer BagSlotTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	ELyraItemCategory ItemCategory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FLyraInventoryItemSlotFilter SlotFilter;

	virtual void OnInventoryBeginPlay_Implementation() override;
	virtual void BulkCreateInventorySlots(ELyraItemCategory Category, const FGameplayTagContainer& SlotTags, const FLyraInventoryItemFilterHandle& Filter, int32 Count, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles) override;
	
protected:
	virtual void PostInventoryUpdate(FAuraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles) override;
	virtual void PopulateSlotReferenceArray(FAuraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles) override;
	
private:
	void InitInventorySlots();

protected:
	friend class ULyraInventoryComponent;
	int32 IdCounter = 0;
	
	UPROPERTY()
	FAuraInventoryItemArray Inventory;

	TArray<FLyraInventoryItemSlotHandle> AllSlotHandles;
};
