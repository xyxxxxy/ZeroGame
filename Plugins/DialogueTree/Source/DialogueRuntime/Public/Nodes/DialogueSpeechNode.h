
#pragma once

#include "DialogueNode.h"
#include "SpeechDetails.h"
#include "DialogueSpeechNode.generated.h"

class UDialogueTransition;
class UDialogueSpeakerComponent;

UCLASS()
class DIALOGUERUNTIME_API UDialogueSpeechNode : public UDialogueNode
{
	GENERATED_BODY()
public:
	void InitSpeechData(const FSpeechDetails& InDetails, TSubclassOf<UDialogueTransition> TransitionType);
	bool GetCanSkip() const;
	FSpeechDetails GetDetails() const;
	UDialogueSpeakerComponent* GetSpeaker() const;
	
	virtual void EnterNode() override;
	virtual FDialogueOption GetAsOption() override;
	virtual void SelectOption(int32 InOptionIndex) override;
	virtual void Skip() override;

	UDialogueTransition* GetTransition() const;

private:
	void StartAudio();

private:
	UPROPERTY()
	FSpeechDetails Details;
	
	UPROPERTY()
	TObjectPtr<UDialogueTransition> Transition;
};