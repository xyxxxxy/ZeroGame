
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LyraInventoryItemFragment.generated.h"

class ULyraInventoryItemInstance;

UCLASS(DefaultToInstanced,EditInlineNew,Abstract,BlueprintType)
class LYRAINVENTORY_API ULyraInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(ULyraInventoryItemInstance* Instance) const{}
};
