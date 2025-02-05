// Copyright Epic Games, Inc. All Rights Reserved.


#include "LyraGameMode.h"

#include "LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerPawn.h"
#include "Player/LyraCharacter.h"
#include "UI/LyraHUD.h"


ALyraGameMode::ALyraGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ALyraGameState::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	DefaultPawnClass = ALyraCharacter::StaticClass();
	HUDClass = ALyraHUD::StaticClass();
}


void ALyraGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}


void ALyraGameMode::InitGameState()
{
	Super::InitGameState();
}

