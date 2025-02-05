
#include "Graph/Nodes/DialogueNodeFactory.h"

#include "Graph/Nodes/GraphNodeDialogueEntry.h"
#include "Graph/Nodes/GraphNodeDialogueSpeech.h"
#include "Graph/Slate/SGraphNodeDialogueBase.h"
#include "Graph/Slate/SGraphNodeDialogueSpeech.h"

TSharedPtr<SGraphNode> FDialogueNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if(UGraphNodeDialogueEntry* GraphNode = Cast<UGraphNodeDialogueEntry>(Node))
	{
		return SNew(SGraphNodeDialogueBase,GraphNode);
	}
	
	if(UGraphNodeDialogueSpeech* GraphNode = Cast<UGraphNodeDialogueSpeech>(Node))
	{
		return SNew(SGraphNodeDialogueSpeech,GraphNode);
	}
	return nullptr;
}
