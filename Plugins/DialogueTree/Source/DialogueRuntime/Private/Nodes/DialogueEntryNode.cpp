
#include "Nodes/DialogueEntryNode.h"
#include "Dialogue.h"
#include "DialogueRuntimeLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueEntryNode)

void UDialogueEntryNode::EnterNode()
{
	check(Dialogue);
	Super::EnterNode();

	if(Children.Num() < 1 || Children[0] == nullptr)
	{
		UE_LOG(LogDialogueRuntime,Warning,TEXT("Exiting dialogue: Entry node has no children..."));
		Dialogue->EndDialogue();
		return;
	}
	Dialogue->TraverseNode(Children[0]);
}
