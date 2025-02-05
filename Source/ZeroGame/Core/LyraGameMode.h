// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "LyraGameMode.generated.h"


/**
 * Modular Game Mode
 */
UCLASS(meta=(Category=Lyra))
class ALyraGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	ALyraGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	//~End of AGameModeBase interface

};