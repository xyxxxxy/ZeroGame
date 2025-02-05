
#include "Nodes/DialogueNode.h"
#include "DialogueRuntimeLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueNode)

UDialogue* UDialogueNode::GetDialogue() const
{
	return Dialogue;
}

void UDialogueNode::SetDialogue(UDialogue* InDialogue)
{
	Dialogue = InDialogue;
}

void UDialogueNode::AddChild(UDialogueNode* InNode)
{
	if(!Children.Contains(InNode))
	{
		Children.Add(InNode);
	}
}

void UDialogueNode::AddParent(UDialogueNode* InNode)
{
	if(!Parent.Contains(InNode))
	{
		Parent.Add(InNode);
	}
}

TArray<UDialogueNode*> UDialogueNode::GetChildren() const
{
	return Children;
}

TArray<UDialogueNode*> UDialogueNode::GetParent() const
{
	return Parent;
}
void UDialogueNode::EnterNode()
{
	UE_LOG(LogDialogueRuntime, Warning, TEXT("Enter Node : %s"), *GetNameSafe(this));
}

int32 UDialogueNode::GetNodeIndex() const
{
	return NodeIndex;
}

void UDialogueNode::SetNodeIndex(int32 Index)
{
	NodeIndex = Index;
}

FDialogueOption UDialogueNode::GetAsOption()
{
	return FDialogueOption();
}

void UDialogueNode::SelectOption(int32 InOptionIndex)
{
}
