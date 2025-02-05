
#include "Nodes/DialogueSpeechNode.h"
#include "Dialogue.h"
#include "DialogueRuntimeLogChannels.h"
#include "Transitions/DialogueTransition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueSpeechNode)

void UDialogueSpeechNode::InitSpeechData(const FSpeechDetails& InDetails, TSubclassOf<UDialogueTransition> TransitionType)
{
	check(TransitionType);

	Details = InDetails;
	Transition = NewObject<UDialogueTransition>(this,TransitionType);
	Transition->SetOwningNode(this);
}

bool UDialogueSpeechNode::GetCanSkip() const
{
	return Details.bCanSkip;
}

FSpeechDetails UDialogueSpeechNode::GetDetails() const
{
	return Details;
}

UDialogueSpeakerComponent* UDialogueSpeechNode::GetSpeaker() const
{
	return Dialogue->GetSpeaker(Details.SpeakerName);
}

void UDialogueSpeechNode::EnterNode()
{
	Super::EnterNode();
	
	if (!Dialogue->SpeakerIsPresent(Details.SpeakerName))
	{
		UE_LOG(
			LogDialogueRuntime,
			Error,
			TEXT("Terminating dialogue early: A participant speaker component was not found. Verify that the dialogue name property matches the speaker's role in the dialogue.")
		);
		Dialogue->EndDialogue();
		return;
	}

	if (!Details.bIgnoreContent)
	{
		//
		Dialogue->DisplaySpeech(Details);
		StartAudio();
	}
	
	if (!Transition)
	{
		UE_LOG(LogDialogueRuntime, Error, TEXT("Speech node is missing transition."));
		Dialogue->EndDialogue();
		return; 
	}
	
	//Play the transition 
	Transition->PreTransition();
}

FDialogueOption UDialogueSpeechNode::GetAsOption()
{
	return FDialogueOption{ Details, this };
}

void UDialogueSpeechNode::SelectOption(int32 InOptionIndex)
{
	//Super::SelectOption(InOptionIndex);
	Transition->SelectOption(InOptionIndex);
}

void UDialogueSpeechNode::Skip()
{
	if(Details.bCanSkip)
	{
		Transition->Skip();
	}
}

void UDialogueSpeechNode::StartAudio()
{
}

UDialogueTransition* UDialogueSpeechNode::GetTransition() const
{
	return Transition;
}
