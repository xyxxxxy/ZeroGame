
#include "Transitions/InputDialogueTransition.h"
#include "DialogueRuntimeLogChannels.h"
#include "Nodes/DialogueSpeechNode.h"
#include "Dialogue.h"
#include "DialogueController.h"

#define LOCTEXT_NAMESPACE "InputDialogueTransition"

void UInputDialogueTransition::PreTransition()
{
	Super::PreTransition();

    GetOptions();
	TryTransitionOut();

	ShowOptionsHandle = OwningNode->GetDialogue()->GetDialogueController()->OnStatementEnd().AddUObject(this, &UInputDialogueTransition::ShowOptions);
}

void UInputDialogueTransition::TransitionOut()
{
	Super::TransitionOut();

	//If there is a child to transition to, pick it
	if (!OwningNode->GetChildren().IsEmpty())
	{
		UE_LOG(
			LogDialogueRuntime,
			Warning,
			TEXT("Terminating dialogue: input transition node has no valid options to select from. Valid options include speech nodes or nodes that eventually arrive at speech nodes.")
		);
	}

	OwningNode->GetDialogue()->EndDialogue();
	return;
}

void UInputDialogueTransition::SelectOption(int32 InOptionIndex)
{
	//End the dialogue if fed a bad index
	OwningNode->GetDialogue()->GetDialogueController()->OnStatementEnd().Remove(ShowOptionsHandle);
	if (!Options.IsValidIndex(InOptionIndex))
	{
		UE_LOG(LogDialogueRuntime, Error, TEXT("Terminating dialogue: attempted to transition to invalid option index."));

		OwningNode->GetDialogue()->EndDialogue();
		return;
	}

	//Transition to the next node of the selected one ````````````````````````````````````````````````````Transition Out / Mark node visited
	UDialogueNode* Selected = Options[InOptionIndex].TargetNode;
	const TArray<UDialogueNode*> NextNodes = Selected->GetChildren();
	if(!NextNodes.IsEmpty() && NextNodes[0])
	{
		//
		OwningNode->GetDialogue()->TraverseNode(NextNodes[0]);
	}
	else
	{
		UE_LOG(LogDialogueRuntime, Warning, TEXT("%s : Transition Out / End Dialogue."), *GetNameSafe(this));
		OwningNode->GetDialogue()->EndDialogue();
	}
}

FText UInputDialogueTransition::GetDisplayName() const
{
    return LOCTEXT("DisplayName", "InputTransition");
}

FText UInputDialogueTransition::GetNodeCreationTooltip() const
{
	return LOCTEXT("Tooltip", "Speech node that waits for the user to select an optionbefore transitioning.");
}

EDialogueConnectionLimit UInputDialogueTransition::GetConnectionLimit() const
{
    return EDialogueConnectionLimit::Unlimited;
}

void UInputDialogueTransition::TryTransitionOut()
{
	if (Options.IsEmpty())
	{
		TransitionOut();
	}	
}

void UInputDialogueTransition::ShowOptions()
{
	//If valid options, display them 
	if (!Options.IsEmpty())
	{
		OwningNode->GetDialogue()->DisplayOptions(Options);
	}
}

void UInputDialogueTransition::GetOptions()
{
	//Retrieve all valid options 
	Options.Empty();
	TArray<UDialogueNode*> NodeChildren = OwningNode->GetChildren();

	for (UDialogueNode* Node : NodeChildren)
	{
		FDialogueOption NodeOption = Node->GetAsOption();

		//If a valid option
		if (!NodeOption.Details.SpeechText.IsEmpty() && NodeOption.TargetNode)
		{
			Options.Add(NodeOption);
		}
	}
}


#undef LOCTEXT_NAMESPACE