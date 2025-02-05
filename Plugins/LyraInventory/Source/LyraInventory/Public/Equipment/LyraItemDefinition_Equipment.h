
#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemDefinition.h"
#include "LyraItemDefinition_Equipment.generated.h"

UCLASS(BlueprintType)
class LYRAINVENTORY_API ULyraItemDefinition_Equipment : public ULyraInventoryItemDefinition
{
	GENERATED_BODY()
public:
	ULyraItemDefinition_Equipment(const FObjectInitializer& ObjectInitializer);
};
