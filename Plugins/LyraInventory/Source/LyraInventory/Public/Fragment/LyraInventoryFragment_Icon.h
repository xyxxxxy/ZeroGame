
#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemFragment.h"
#include "LyraInventoryFragment_Icon.generated.h"

class UTexture2D;

UCLASS()
class LYRAINVENTORY_API ULyraInventoryFragment_Icon : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
	UTexture2D* Icon;
};
