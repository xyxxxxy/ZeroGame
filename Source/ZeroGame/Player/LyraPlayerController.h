
#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "LyraPlayerController.generated.h"


/**
 * CommonGame PlayerController
 */
UCLASS(meta=(Category=Lyra))
class ALyraPlayerController : public ACommonPlayerController /** CommonGame base */
{
	GENERATED_BODY()

public:
	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//AActor interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of AActor interface

};