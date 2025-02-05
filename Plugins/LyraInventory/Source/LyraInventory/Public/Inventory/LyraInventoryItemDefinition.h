
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayEffect.h"

#include "LyraInventoryItemDefinition.generated.h"

class ULyraInventoryItemDefinition;
class ULyraInventoryItemFragment;



/////////////////////////////////////////////////////////

// For Object
UCLASS(BlueprintType)
class LYRAINVENTORY_API ULyraInventoryItemDefinition : public UDataAsset,public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	ULyraInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	const ULyraInventoryItemFragment* FindFragmentByClass(TSubclassOf<ULyraInventoryItemFragment> FragmentClass) const;
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;



	
	int32 GetMaxStackSize() const;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
	FInheritedTagContainer ItemTags;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
	int32 MaxStackSize = 1;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
	TArray<TObjectPtr<ULyraInventoryItemFragment>> Fragments;
	
};

