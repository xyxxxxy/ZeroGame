
#include "Graph/Nodes/GraphNodeDialogue.h"
#include "Nodes/DialogueNode.h"
#include "Dialogue.h"
#include "Graph/DialogueEdGraph.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GraphNodeDialogue)

#define LOCTEXT_NAMESPACE "DialogueEdGraph"

UGraphNodeDialogue::UGraphNodeDialogue()
{
}

void UGraphNodeDialogue::PostEditUndo()
{
	Super::PostEditUndo();
	UpdateDialogueNode();
	MarkDialogueDirty();
}

void UGraphNodeDialogue::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateDialogueNode();
	MarkDialogueDirty();
}

FLinearColor UGraphNodeDialogue::GetNodeTitleColor() const
{
	return DEFAULT_GRAY;
}

FText UGraphNodeDialogue::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (!ID.IsNone())
	{
		return FText::FromName(ID);
	}

	return FText::FromName(GetFName());
}

void UGraphNodeDialogue::PinConnectionListChanged(UEdGraphPin* Pin)
{
	Super::PinConnectionListChanged(Pin);
	UpdateDialogueNode();
	MarkDialogueDirty();
}

void UGraphNodeDialogue::ResizeNode(const FVector2D& NewSize)
{
	//If we are not setting size for the first time, adjust position 
	if (NodeWidth > 1.f)
	{
		float WidthChange = NewSize.X - NodeWidth;
		NodePosX -= WidthChange / 2.f;
	}

	//Cache the new size
	NodeWidth = NewSize.X;
	NodeHeight = NewSize.Y;
}

FName UGraphNodeDialogue::GetID() const
{
	return ID;
}

FName UGraphNodeDialogue::GetBaseID() const
{
	return FName("DialogueNode");
}

void UGraphNodeDialogue::ResetID()
{
	check(DialogueGraph);

	DialogueGraph->RemoveFromNodeMap(ID);
	InitNodeInDialogueGraph(DialogueGraph);
}

UDialogueEdGraph* UGraphNodeDialogue::GetDialogueGraph() const
{
	return DialogueGraph;
}

void UGraphNodeDialogue::BindOnUpdateVisuals(const FOnUpdateNode& UpdateDelegate)
{
	OnUpdateVisuals = UpdateDelegate;
}

void UGraphNodeDialogue::InitNodeInDialogueGraph(UEdGraph* OwningGraph)
{
	check(OwningGraph);
	DialogueGraph = CastChecked<UDialogueEdGraph>(OwningGraph);
	FText BaseIDText = LOCTEXT("BaseIDText", "{0} {1}");

	//Set initial ID
	int32 Counter = 1;
	FName BaseID = GetBaseID();
	FText IDText = FText::FromName(BaseID);

	//Increment number until ID is unique 
	while (DialogueGraph->ContainsNode(FName(IDText.ToString())))
	{
		Counter++;
		FText CounterText = FText::AsNumber(Counter);

		IDText = FText::Format(
			BaseIDText,
			FText::FromName(BaseID),
			CounterText
		);
	}
	
	//Set the ID
	ID = FName(IDText.ToString());
	//Add node to map
	DialogueGraph->AddToNodeMap(this);
}

void UGraphNodeDialogue::SortNodesLeftToRight(TArray<UGraphNodeDialogue*>& Nodes)
{
	Nodes.Sort(
	[](const UGraphNodeDialogue& Node1,const UGraphNodeDialogue& Node2)
	{
		return Node1.NodePosX < Node2.NodePosX;
	}
);
}

void UGraphNodeDialogue::SetErrorFlag(bool InFlag)
{
	if (InFlag != bDialogueError)
	{
		bDialogueError = InFlag;
		UpdateDialogueNode();
	}
}

bool UGraphNodeDialogue::HasError() const
{
	return bDialogueError;
}

bool UGraphNodeDialogue::CanCompileNode()
{
	return true;
}

UDialogueNode* UGraphNodeDialogue::GetAssetNode() const
{
	return AssetNode;
}

void UGraphNodeDialogue::SetAssetNode(UDialogueNode* InDialogueNode)
{
	AssetNode = InDialogueNode;
}

void UGraphNodeDialogue::CreateAssetNode(UDialogue* InAsset)
{
}

void UGraphNodeDialogue::FinalizeAssetNode()
{
}

void UGraphNodeDialogue::LinkAssetNode()
{
	check(AssetNode);
	
	TArray<UGraphNodeDialogue*> Parents; 
	GetParents(Parents);
	
	for (UGraphNodeDialogue* Parent : Parents)
	{
		//Verify that the parent's asset node has been spawned
		if (Parent->GetAssetNode())
		{
			LinkToParent(Parent);
			Parent->LinkToChild(this);
		}
	}
}

void UGraphNodeDialogue::ClearAssetNode()
{
	AssetNode = nullptr;
}

void UGraphNodeDialogue::GetParents(TArray<UGraphNodeDialogue*>& OutNodes) const
{
	OutNodes.Empty();
	GetParents_Internal(GetDirectParents(), OutNodes);
}

void UGraphNodeDialogue::GetChildren(TArray<UGraphNodeDialogue*>& OutNodes) const
{
	OutNodes.Empty();
	GetChildren_Internal(GetDirectChildren(), OutNodes);
}

void UGraphNodeDialogue::GetPinChildren(UEdGraphPin* InPin, TArray<UGraphNodeDialogue*>& OutNodes) const
{
	OutNodes.Empty();

	TArray<UGraphNodeDialogueBase*> LinkedNodes;
	for (const UEdGraphPin* LinkedPin : InPin->LinkedTo)
	{
		if (UGraphNodeDialogueBase* BaseNode = Cast<UGraphNodeDialogueBase>(LinkedPin->GetOwningNode()))
		{
			LinkedNodes.Add(BaseNode);
		}
	}

	GetChildren_Internal(LinkedNodes, OutNodes);
}

void UGraphNodeDialogue::MarkDialogueDirty()
{
	if (!DialogueGraph)
	{
		return;
	}

	DialogueGraph->GetDialogue()->SetCompileStatus(EDialogueCompileStatus::UnCompiled);
}

void UGraphNodeDialogue::UpdateDialogueNode()
{
	OnUpdateVisuals.ExecuteIfBound();
}

void UGraphNodeDialogue::LinkToParent(UGraphNodeDialogue* InParent)
{
	UDialogueNode* ParentAssetNode = InParent->GetAssetNode();
	if (AssetNode && ParentAssetNode)
	{
		AssetNode->AddParent(ParentAssetNode);
	}
}

void UGraphNodeDialogue::LinkToChild(UGraphNodeDialogue* InChild)
{
	UDialogueNode* ChildAssetNode = InChild->GetAssetNode();
	if (AssetNode && InChild)
	{
		AssetNode->AddChild(ChildAssetNode);
	}
}

void UGraphNodeDialogue::GetParents_Internal(TArray<UGraphNodeDialogueBase*> InParents, TArray<UGraphNodeDialogue*>& OutNodes) const
{
	//Parents of to recur over递归 (nodes that don't descend from this)
	TArray<UGraphNodeDialogueBase*> RecurParents;

	//Go through all input parent nodes
	for (UGraphNodeDialogueBase* Node : InParents)
	{
		//If the parent is a GraphNodeDialogue
		if (UGraphNodeDialogue* DialogueNode = Cast<UGraphNodeDialogue>(Node))
		{
			OutNodes.Add(DialogueNode);
		}
		//If not a GraphNodeDialogue, recur over the parent's parents
		else
		{
			for (UGraphNodeDialogueBase* Parent : Node->GetDirectParents())
			{
				if (!RecurParents.Contains(Parent))
				{
					RecurParents.Add(Parent);
				}
			}
		}
	}

	//If we found any non-GraphNodeDialogue nodes
	if (RecurParents.Num() > 0)
	{
		GetParents_Internal(RecurParents, OutNodes);
	}
}

void UGraphNodeDialogue::GetChildren_Internal(TArray<UGraphNodeDialogueBase*> InChildren, TArray<UGraphNodeDialogue*>& OutNodes) const
{
	TArray<UGraphNodeDialogueBase*> RecurChildren;
	
	for (UGraphNodeDialogueBase* Node : InChildren)
	{
		if (UGraphNodeDialogue* DialogueNode = Cast<UGraphNodeDialogue>(Node))
		{
			OutNodes.Add(DialogueNode);
		}
		else
		{
			for (UGraphNodeDialogueBase* Child : Node->GetDirectChildren())
			{
				if (RecurChildren.Contains(Child) == false)
				{
					RecurChildren.Add(Child);
				}
			}
		}
	}
	
	if (RecurChildren.Num() > 0)
	{
		GetChildren_Internal(RecurChildren, OutNodes);
	}
}


#undef LOCTEXT_NAMESPACE

