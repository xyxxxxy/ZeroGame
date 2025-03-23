
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
	FGameplayTagContainer SlotTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	int32 BagSlotCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FLyraInventoryItemSlotFilter SlotFilter;

	virtual void OnInventoryBeginPlay_Implementation() override;
	
protected:
	friend class ULyraInventoryComponent;

	// TODO:???
	TArray<FLyraInventoryItemSlotHandle> BagSlotHandles;
};
