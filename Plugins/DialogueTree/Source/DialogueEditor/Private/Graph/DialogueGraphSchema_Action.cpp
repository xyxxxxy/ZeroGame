
#include "Graph/DialogueGraphSchema_Action.h"
#include "Graph/Nodes/GraphNodeDialogue.h"
#include "Settings/EditorStyleSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueGraphSchema_Action)

#define LOCTEXT_NAMESPACE "DialogueEditorSchemaAction"

FDialogueSchemaAction_NewNode::FDialogueSchemaAction_NewNode()
{
}

FDialogueSchemaAction_NewNode::FDialogueSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, UGraphNodeDialogue* InTemplateNode)
: FEdGraphSchemaAction(MoveTemp(InNodeCategory),MoveTemp(InMenuDesc),MoveTemp(InToolTip),0),TemplateNode(InTemplateNode)
{
	check(TemplateNode);
}

UEdGraphNode* FDialogueSchemaAction_NewNode::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode)
{
	if (TemplateNode != nullptr)
	{
		//Begin transaction 
		const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}
		TemplateNode->SetFlags(RF_Transactional);

		//Setup the node within the graph 
		PlaceNodeInGraph(ParentGraph, Location, FromPin);

		return TemplateNode;
	}

	//No result node was created 
	return nullptr;
}

void FDialogueSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(TemplateNode);
}

void FDialogueSchemaAction_NewNode::SetTemplateNode(UGraphNodeDialogue* InNode)
{
	check(InNode);
	TemplateNode = InNode;
}

void FDialogueSchemaAction_NewNode::PlaceNodeInGraph(UEdGraph* InParentGraph, const FVector2D InLocation, UEdGraphPin* FromPin)
{
	//Add template node to graph
	TemplateNode->Rename(nullptr, InParentGraph, REN_NonTransactional);
	InParentGraph->AddNode(TemplateNode, true);
	TemplateNode->CreateNewGuid();
	TemplateNode->PostPlacedNewNode();

	//Set the node's location, pins and ID
	TemplateNode->InitNodeInDialogueGraph(InParentGraph);
	SetNodeLocation(InLocation);
	TemplateNode->AllocateDefaultPins();
	TemplateNode->AutowireNewNode(FromPin);
}

void FDialogueSchemaAction_NewNode::SetNodeLocation(const FVector2D InLocation)
{
	TemplateNode->NodePosX = InLocation.X;
	TemplateNode->NodePosY = InLocation.Y;
	TemplateNode->SnapToGrid(GetDefault<UEditorStyleSettings>()->GridSnapSize);
}

#undef LOCTEXT_NAMESPACE
