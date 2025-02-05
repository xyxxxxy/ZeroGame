
#include "Transitions/AutoDialogueTransition.h"
#include "Nodes/DialogueSpeechNode.h"
#include "Dialogue.h"
#include "DialogueRuntimeLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AutoDialogueTransition)

FText UAutoDialogueTransition::GetDisplayName() const
{
	return FText::FromString("AutoTransition");
}

FText UAutoDialogueTransition::GetNodeCreationTooltip() const
{
	return FText::FromString("Create speech node that automatically transitions to the first viable option");
}

void UAutoDialogueTransition::TransitionOut()
{
	Super::TransitionOut();
	TArray<UDialogueNode*> Children = OwningNode->GetChildren();
	if (!Children.IsEmpty() && Children[0])
	{
		OwningNode->GetDialogue()->TraverseNode(Children[0]);
	}
	else
	{
		UE_LOG(LogDialogueRuntime, Warning, TEXT("%s : Transition Out / End Dialogue."), *GetNameSafe(this));
		OwningNode->GetDialogue()->EndDialogue();
	}
}
