#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "LyraGameState.generated.h"


/**
 * Modular Game State
 */
UCLASS(meta=(Category=Lyra))
class  ALyraGameState : public AModularGameStateBase
{
	GENERATED_BODY()

public:
	ALyraGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    
};