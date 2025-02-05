
#pragma once

#include "CoreMinimal.h"
#include "SpeechDetails.h"
#include "DialogueOption.generated.h"

class UDialogueNode;

USTRUCT()
struct DIALOGUERUNTIME_API FDialogueOption
{
	GENERATED_BODY()

	FSpeechDetails Details = FSpeechDetails();
	TObjectPtr<UDialogueNode> TargetNode = nullptr;
};