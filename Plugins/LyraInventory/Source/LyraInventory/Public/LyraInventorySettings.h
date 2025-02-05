
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LyraInventorySettings.generated.h"

UCLASS(Config = Game, meta = (DisplayName = "Inventory Settings"), DefaultConfig)
class LYRAINVENTORY_API ULyraInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	ULyraInventorySettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()):Super(ObjectInitializer){}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items", Config, AdvancedDisplay)
	TSoftObjectPtr<UDataTable> ItemDefDataTable;
};
