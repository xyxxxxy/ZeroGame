
#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "LyraPlayerState.generated.h"


/**
 * Modular PlayerState
 */
UCLASS(meta=(Category=Lyra))
class ALyraPlayerState : public AModularPlayerState
{
	GENERATED_BODY()

public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};