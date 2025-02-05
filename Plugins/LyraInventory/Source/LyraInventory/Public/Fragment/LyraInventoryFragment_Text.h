
#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemFragment.h"
#include "LyraInventoryFragment_Text.generated.h"

UCLASS()
class LYRAINVENTORY_API ULyraInventoryFragment_Text : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
	FName ItemName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (MultiLine = true),Category = "Inventory")
	FText Description;
};
