
#pragma once

#include "Transitions/DialogueTransition.h"
#include "AutoDialogueTransition.generated.h"

UCLASS()
class DIALOGUERUNTIME_API UAutoDialogueTransition : public UDialogueTransition
{
	GENERATED_BODY()
public:
	virtual FText GetDisplayName() const override;
	virtual FText GetNodeCreationTooltip() const override;
	virtual void TransitionOut() override;
};
