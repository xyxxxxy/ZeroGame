
#pragma once

#include "CoreMinimal.h"
#include "DialogueConnectionLimit.h"
#include "DialogueTransition.generated.h"
class UDialogueSpeechNode;

UCLASS(Abstract)
class DIALOGUERUNTIME_API UDialogueTransition : public UObject
{
	GENERATED_BODY()
public:
	UDialogueTransition();

public:
	void SetOwningNode(UDialogueSpeechNode* InNode);
	virtual void PreTransition();
	virtual void TransitionOut();
	virtual void SelectOption(int32 InOptionIndex){};
	virtual void Skip(){};
	virtual FText GetDisplayName() const;
	virtual FText GetNodeCreationTooltip() const;
	virtual EDialogueConnectionLimit GetConnectionLimit() const;
	virtual void TryTransitionOut();

protected:
	UPROPERTY()
	TObjectPtr<UDialogueSpeechNode> OwningNode;

};