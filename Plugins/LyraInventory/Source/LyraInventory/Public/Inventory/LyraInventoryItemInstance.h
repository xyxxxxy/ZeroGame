
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "LyraInventoryItemInstance.generated.h"

class ULyraInventoryItemFragment;

UCLASS(Blueprintable, BlueprintType)
class LYRAINVENTORY_API ULyraInventoryItemInstance : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	ULyraInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	virtual FLyraInventoryTableRowFragmentsHandle GetItemStructHandle() const;
	virtual void SetItemStructsHandle(const FLyraInventoryTableRowFragmentsHandle& InHandle);
	
public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool CanStackWith(ULyraInventoryItemInstance* Other);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool MergeItemStacks(ULyraInventoryItemInstance* Other);

	// Item def, regardless of stacks
	bool operator==(const ULyraInventoryItemInstance& Other) const
	{
		return ItemStructsHandle.Data.Get() == Other.ItemStructsHandle.Data.Get();
	}
	bool operator!=(const ULyraInventoryItemInstance& Other) const
	{
		return !ULyraInventoryItemInstance::operator==(Other);
	}

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inventory")
	int32 StackSize = 1;

protected:
	FLyraInventoryTableRowFragmentsHandle ItemStructsHandle;

	UPROPERTY()
	TArray<TObjectPtr<ULyraInventoryItemFragment>> DynamicFragments;
};




// UFUNCTION(BlueprintCallable,BlueprintPure="false",meta=(DeterminesOutputType=FragmentClass))
// virtual const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;
//
// template<typename ResultClass>
// const ResultClass* FindFragmentByClass() const
// {
// 	return (ResultClass*)(FindFragmentByClass(ResultClass::StaticClass()));
// }

// virtual ULyraInventoryItemDefinition* GetItemDefinition() const;
// virtual void SetItemDefinition(ULyraInventoryItemDefinition* InDefinition);
