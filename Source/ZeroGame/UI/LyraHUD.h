#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LyraHUD.generated.h"


/**
 * Modular HUD
 *
 * ModularGameplayActors does not provide a HUD, so we implement it here.
 */
UCLASS(meta = (DisplayName = "Lyra HUD", Category = Lyra))
class  ALyraHUD : public AHUD
{
	GENERATED_BODY()

public:
	ALyraHUD(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~UObject interface
	virtual void PreInitializeComponents() override;
	//~End of UObject interface

	//~AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

};