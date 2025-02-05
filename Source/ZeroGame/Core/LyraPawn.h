#pragma once

#include "CoreMinimal.h"
#include "ModularPawn.h"
#include "LyraPawn.generated.h"


/**
 * Modular Pawn
 */
UCLASS(meta=(Category=Lyra))
class ALyraPawn : public AModularPawn
{
	GENERATED_BODY()

public:
	ALyraPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};