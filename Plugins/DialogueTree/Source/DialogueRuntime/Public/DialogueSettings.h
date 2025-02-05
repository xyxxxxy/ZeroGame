
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DialogueSettings.generated.h"

class ADialogueController;

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Dialogue Tree"))
class DIALOGUERUNTIME_API UDialogueSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	const FString DefaultControllerCoords = "/Script/Engine.Blueprint'/DialogueTree/Controller/BP_BasicDialogueController.BP_BasicDialogueController_C'";
	
public:
	UPROPERTY(Config,EditAnywhere,BlueprintReadOnly,Category = "Dialogue")
	TSubclassOf<ADialogueController> DialogueControllerType = LoadClass<class ADialogueController>(nullptr,*DefaultControllerCoords);
	
};
