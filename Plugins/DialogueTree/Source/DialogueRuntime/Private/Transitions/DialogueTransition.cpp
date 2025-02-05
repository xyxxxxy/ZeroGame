
#include "Transitions/DialogueTransition.h"
#include "DialogueRuntimeLogChannels.h"
#include "Nodes/DialogueSpeechNode.h"
#include "Dialogue.h"
#include "DialogueSpeakerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueTransition)

UDialogueTransition::UDialogueTransition()
{
}

void UDialogueTransition::SetOwningNode(UDialogueSpeechNode* InNode)
{
	OwningNode = InNode;
}

void UDialogueTransition::PreTransition()
{
	if (!OwningNode)
	{
		UE_LOG(LogDialogueRuntime, Error, TEXT("Transition failed to find owning node. Ending dialogue early."));
		OwningNode->GetDialogue()->EndDialogue();
		return;
	}
	
	UDialogueSpeakerComponent* SpeakerComp = OwningNode->GetSpeaker();
	if (!SpeakerComp)
	{
		UE_LOG(LogDialogueRuntime,Error, TEXT("Transition failed to find speaker component. Ending dialogue early."));
		OwningNode->GetDialogue()->EndDialogue();
		return;
	}
	

	//If no minimum time or audio content, just transition out 
	//TryTransitionOut();
}

void UDialogueTransition::TransitionOut()
{
	UE_LOG(LogDialogueRuntime, Warning, TEXT("%s : Transition Out."), *GetNameSafe(this));
}

FText UDialogueTransition::GetDisplayName() const
{
	return FText::FromString(StaticClass()->GetName());
}

FText UDialogueTransition::GetNodeCreationTooltip() const
{
	return FText::GetEmpty();
}

EDialogueConnectionLimit UDialogueTransition::GetConnectionLimit() const
{
	return EDialogueConnectionLimit::Single;
}

void UDialogueTransition::TryTransitionOut()
{
	TransitionOut();
}



