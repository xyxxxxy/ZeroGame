#pragma once

#include "CoreMinimal.h"
#include "CommonGameViewportClient.h"
#include "LyraGameViewportClient.generated.h"


/**
 * CommonGame Game Viewport Client
 *
 * Must be configured in DefaultEngine.ini:
 * 
 * [/Script/Engine.Engine]
 * GameViewportClientClassName=/Script/EditorProject.LyraGameViewportClient
 */
UCLASS()
class ULyraGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:
	ULyraGameViewportClient(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UGameViewportClient interface
	virtual void Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice) override;
	//~End of UGameViewportClient

};