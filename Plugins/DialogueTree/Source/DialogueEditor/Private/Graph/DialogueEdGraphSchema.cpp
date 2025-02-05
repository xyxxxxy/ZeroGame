
#include "Graph/DialogueEdGraphSchema.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/DialogueGraphSchema_Action.h"
#include "Graph/Nodes/GraphNodeDialogueEntry.h"
#include "Graph/Nodes/GraphNodeDialogueSpeech.h"
#include "Transitions/DialogueTransition.h"
#include "Graph/DialogueTreeConnectionDrawingPolicy.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueEdGraphSchema)

#define LOCTEXT_NAMESPACE "DialogueEditorSchema"

int32 UDialogueEdGraphSchema::CurrentCacheRefreshID = 0;

FConnectionDrawingPolicy* UDialogueEdGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID,
	int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements,
	UEdGraph* InGraphObj) const
{
	return new FDialogueTreeConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements);
}

void UDialogueEdGraphSchema::OnPinConnectionDoubleCicked(UEdGraphPin* PinA, UEdGraphPin* PinB,
	const FVector2D& GraphPosition) const
{
	Super::OnPinConnectionDoubleCicked(PinA, PinB, GraphPosition);
}

void UDialogueEdGraphSchema::GetPinContextMenu(UToolMenu* Menu, UEdGraphPin* Pin, FText PinText) const
{

}

void UDialogueEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	GetSpeechNodeMenuActions(ContextMenuBuilder);
}

void UDialogueEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if(Context->Node)
	{
		GetNodeContextMenu(Menu);
	}
	
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UDialogueEdGraphSchema::CanCreateConnection(const UEdGraphPin* A,
	const UEdGraphPin* B) const
{

	//Connection values to work with 
	FConnectionArgs ConnectionArgs;
	ConnectionArgs.PinA = A;
	ConnectionArgs.PinB = B;
	ConnectionArgs.NodeA = Cast<UGraphNodeDialogueBase>(A->GetOwningNode());
	ConnectionArgs.NodeB = Cast<UGraphNodeDialogueBase>(B->GetOwningNode());

	//Response to return 
	FPinConnectionResponse Response;

	//Check for disallowed connections
	CheckForDisallowedConnection(ConnectionArgs, Response);

	if (Response.Response == CONNECT_RESPONSE_DISALLOW)
	{
		return Response;
	}

	//Connection is allowed - either limited or unlimited
	if (!ConnectionHasConnectionLimit(ConnectionArgs, Response))
	{
		//No connection limit 
		Response.Response = CONNECT_RESPONSE_MAKE;
		Response.Message = LOCTEXT("AllowConnection", "Connect nodes");
	}
	return Response;
}

void UDialogueEdGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UGraphNodeDialogueEntry> NodeCreator(Graph);
	UGraphNodeDialogueEntry* TargetNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(TargetNode,FNodeMetadata::DefaultGraphNode);
	
	UDialogueEdGraph* DialogueGraph = Cast<UDialogueEdGraph>(&Graph);
	TargetNode->InitNodeInDialogueGraph(DialogueGraph);
	DialogueGraph->SetGraphRoot(TargetNode);
}

FLinearColor UDialogueEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor(.5f,.5f,.5f,1.f);
}

bool UDialogueEdGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return Super::IsCacheVisualizationOutOfDate(InVisualizationCacheID);
}

int32 UDialogueEdGraphSchema::GetCurrentVisualizationCacheID() const
{
	return Super::GetCurrentVisualizationCacheID();
}

void UDialogueEdGraphSchema::ForceVisualizationCacheClear() const
{
	Super::ForceVisualizationCacheClear();
}

void UDialogueEdGraphSchema::GetNodeContextMenu(UToolMenu* Menu) const
{
	FToolMenuSection& NodeMenuSection = Menu->AddSection(FName(TEXT("DialogueTreeNodeAcitons")), LOCTEXT("NodeActionCategory", "Node Actions"));
	NodeMenuSection.AddMenuEntry(FGenericCommands::Get().Delete);
	NodeMenuSection.AddMenuEntry(FGenericCommands::Get().Cut);
	NodeMenuSection.AddMenuEntry(FGenericCommands::Get().Copy);
	NodeMenuSection.AddMenuEntry(FGenericCommands::Get().Duplicate);
	NodeMenuSection.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
}

void UDialogueEdGraphSchema::GetNodeMenuActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
}

void UDialogueEdGraphSchema::GetSpeechNodeMenuActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	//Fetch the current graph 
	const UDialogueEdGraph* TargetGraph = Cast<UDialogueEdGraph>(ContextMenuBuilder.CurrentGraph);

	if (!TargetGraph)
	{
		return;
	}

	//Loop through the list of speakers 
	for (UDialogueSpeakerSocket* SpeakerSocket : TargetGraph->GetAllSpeakers())
	{
		if (SpeakerSocket->GetSpeakerName().IsNone())
		{
			continue;
		}

		//Add one create node action for each transition type 
		for (TObjectIterator<UClass> TypeIterator; TypeIterator; ++TypeIterator)
		{
			if (TypeIterator->IsChildOf(UDialogueTransition::StaticClass())
				&& !TypeIterator->HasAnyClassFlags(CLASS_Abstract))
			{
				TSubclassOf<UDialogueTransition> CurrentType = *TypeIterator;

				TSharedPtr<FDialogueSchemaAction_NewNode> NewNodeAction =
					MakeCreateSpeechNodeAction(
						SpeakerSocket,
						CurrentType,
						ContextMenuBuilder.OwnerOfTemporaries
					);
				
				//Create action and add to menu 
				ContextMenuBuilder.AddAction(NewNodeAction);
			}
		}
	}

}

TSharedPtr<FDialogueSchemaAction_NewNode> UDialogueEdGraphSchema::MakeCreateSpeechNodeAction(
	UDialogueSpeakerSocket* SpeakerSocket, TSubclassOf<UDialogueTransition> TransitionType, UObject* Outer) const
{
	check(SpeakerSocket && TransitionType && Outer);

	//Get context menu text
	UDialogueTransition* DefaultTransitionObj = TransitionType->GetDefaultObject<UDialogueTransition>();

	FText MenuCategory = LOCTEXT("SpeechNodeMenuCategory", "SpeechNodes");
	FText MenuText = FText::Format(
		LOCTEXT("SpeechNodeCreationText", "{0}, {1}"),
		FText::FromName(SpeakerSocket->GetSpeakerName()),
		DefaultTransitionObj->GetDisplayName()
	);
	FText MenuTooltip = DefaultTransitionObj->GetNodeCreationTooltip();

	//Get template node
	UGraphNodeDialogueSpeech* TemplateObject = UGraphNodeDialogueSpeech::MakeTemplate(Outer, SpeakerSocket, TransitionType);
	check(TemplateObject);

	//Assemble action 
	TSharedPtr<FDialogueSchemaAction_NewNode> NewAction(
		new FDialogueSchemaAction_NewNode(
			MenuCategory,
			MenuText,
			MenuTooltip,
			TemplateObject
		)
	);

	return NewAction;
}

void UDialogueEdGraphSchema::CheckForDisallowedConnection(const FConnectionArgs& InArgs, FPinConnectionResponse& OutResponse) const
{
	check(InArgs.PinA && InArgs.PinB);

	//Same pin direction
	if (InArgs.PinA->Direction == InArgs.PinB->Direction)
	{
		OutResponse.Response = CONNECT_RESPONSE_DISALLOW;
		OutResponse.Message = LOCTEXT(
			"SamePinDirConnectionError",
			"Cannot connect two input or two output pins"
		);
	}
	//Invalid or missing nodes
	else if (!InArgs.NodeA || !InArgs.NodeB)
	{
		OutResponse.Response = CONNECT_RESPONSE_DISALLOW;
		OutResponse.Message = LOCTEXT(
			"WrongNodeTypeConnectionError",
			"No connectable nodes found extending UGraphNodeDialogueBase"
		);
	}
	//Attempting to connect to self 
	else if (InArgs.NodeA == InArgs.NodeB)
	{
		OutResponse.Response = CONNECT_RESPONSE_DISALLOW;
		OutResponse.Message = LOCTEXT(
			"SelfConnectConnectionError",
			"Unable to connect node to itself"
		);
	}
	//Attempting to connect to direct parent 
	else if (NodeIsDirectParent(InArgs.NodeA, InArgs.NodeB))
	{
		OutResponse.Response = CONNECT_RESPONSE_DISALLOW;
		OutResponse.Message = LOCTEXT(
			"DirectParentConnectionError",
			"Unable to connect node to its direct parent"
		);
	}
}

bool UDialogueEdGraphSchema::NodeIsDirectParent(const UGraphNodeDialogueBase* NodeA, const UGraphNodeDialogueBase* NodeB) const
{
	check(NodeA && NodeB);

	//If either node is a direct parent of the other
	const TArray<UGraphNodeDialogueBase*> ParentNodesA = NodeA->GetDirectParents();
	const TArray<UGraphNodeDialogueBase*> ParentNodesB = NodeB->GetDirectParents();

	if (ParentNodesB.Contains(NodeA) || ParentNodesA.Contains(NodeB))
	{
		return true;
	}
	return false;
}

bool UDialogueEdGraphSchema::ConnectionHasConnectionLimit(const FConnectionArgs& InArgs, FPinConnectionResponse& OutResponse) const
{
	check(InArgs.NodeA && InArgs.NodeB);

	//If either node has a connection limit, respect it 
	bool bASinglyConnectable = InArgs.NodeA->GetOutputConnectionLimit() == EDialogueConnectionLimit::Single;
	bool bBSinglyConnectable = InArgs.NodeB->GetInputConnectionLimit() == EDialogueConnectionLimit::Single;

	if (bASinglyConnectable && bBSinglyConnectable)
	{
		OutResponse.Response = CONNECT_RESPONSE_BREAK_OTHERS_AB;
		OutResponse.Message = LOCTEXT(
			"AllowConnectionABExclusive",
			"Connect nodes"
		);
	}
	else if (bASinglyConnectable)
	{
		OutResponse.Response = CONNECT_RESPONSE_BREAK_OTHERS_A;
		OutResponse.Message = LOCTEXT(
			"AllowConnectionAExclusive",
			"Connect nodes"
		);
	}
	else if (bBSinglyConnectable)
	{
		OutResponse.Response = CONNECT_RESPONSE_BREAK_OTHERS_B;
		OutResponse.Message = LOCTEXT(
			"AllowConnectionBExclusive",
			"Connect nodes"
		);
	}

	return bASinglyConnectable || bBSinglyConnectable;
}
#undef LOCTEXT_NAMESPACE
