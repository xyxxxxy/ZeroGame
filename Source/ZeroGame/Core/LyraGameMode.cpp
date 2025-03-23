// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraGameMode.h"
#include "LyraGameState.h"
#include "Player/LyraPlayerController.h"
#include "Player/LyraPlayerPawn.h"
#include "Player/LyraCharacter.h"
#include "UI/LyraHUD.h"
#include "UI/LyraNavigationConfig.h"

ALyraGameMode::ALyraGameMode(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
	GameStateClass = ALyraGameState::StaticClass();
	PlayerControllerClass = ALyraPlayerController::StaticClass();
	DefaultPawnClass = ALyraCharacter::StaticClass();
	HUDClass = ALyraHUD::StaticClass();
}

void ALyraGameMode::InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ALyraGameMode::InitGameState()
{
	Super::InitGameState();
}

void ALyraGameMode::BeginPlay()
{
	Super::BeginPlay();

	const auto navigation = MakeShared<FNavigationConfig>();
	navigation->bKeyNavigation = false;
	navigation->bTabNavigation = false;
	navigation->bAnalogNavigation = false;
	FSlateApplication::Get().SetNavigationConfig(navigation);
}
