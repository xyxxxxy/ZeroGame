#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "LyraLocalPlayer.generated.h"


/**
 * CommonGame LocalPlayer
 *
 * Must be configured in DefaultEngine.ini:
 * 
 * [/Script/Engine.Engine]
 * LocalPlayerClassName=/Script/EditorProject.LyraLocalPlayer
 */
UCLASS(meta = (Category = Lyra))
class  ULyraLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:
	ULyraLocalPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};