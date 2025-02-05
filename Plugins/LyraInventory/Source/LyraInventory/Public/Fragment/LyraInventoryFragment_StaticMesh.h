
#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemFragment.h"
#include "LyraInventoryFragment_StaticMesh.generated.h"

class UStaticMesh;

UCLASS()
class LYRAINVENTORY_API ULyraInventoryFragment_StaticMesh : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
	UStaticMesh* StaticMesh;
};
