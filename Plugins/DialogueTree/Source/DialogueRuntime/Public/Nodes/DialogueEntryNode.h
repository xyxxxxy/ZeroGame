
#pragma once

#include "CoreMinimal.h"

#include "Nodes/DialogueNode.h"
#include "DialogueEntryNode.generated.h"
UCLASS()
class DIALOGUERUNTIME_API UDialogueEntryNode : public UDialogueNode
{
	GENERATED_BODY()
public:
	virtual void EnterNode() override;
};