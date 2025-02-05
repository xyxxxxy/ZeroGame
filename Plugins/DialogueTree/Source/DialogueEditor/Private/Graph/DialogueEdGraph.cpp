
#include "Graph/DialogueEdGraph.h"
#include "Graph/Nodes/GraphNodeDialogue.h"
#include "Graph/Nodes/GraphNodeDialogueEntry.h"
#include "Dialogue.h"
#include "GraphEditAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueEdGraph)

UDialogueEdGraph::UDialogueEdGraph()
{
	AddOnGraphChangedHandler(FOnGraphChanged::FDelegate::CreateUObject(this,&UDialogueEdGraph::OnDialogueGraphChanged));
}

bool UDialogueEdGraph::Modify(bool bAlwaysMarkDirty)
{
	bool ModifyReturnValue = Super::Modify(bAlwaysMarkDirty);
	
	GetDialogue()->Modify();
	for (UEdGraphNode* Node : Nodes)
	{
		Node->Modify();
	}

	return ModifyReturnValue;
}

void UDialogueEdGraph::PostEditUndo()
{
	Super::PostEditUndo();
	NotifyGraphChanged();
}

void UDialogueEdGraph::PostInitProperties()
{
	Super::PostInitProperties();
	//Set up speaker roles changed event
	if (UDialogue* OuterDialogue = Cast<UDialogue>(GetOuter()))
	{
		OuterDialogue->OnSpeakerRolesChanged.BindUFunction(this,"OnSpeakerRolesChanged");
	}
}

void UDialogueEdGraph::SetGraphRoot(UGraphNodeDialogue* InRoot)
{
	Root = InRoot;
}

UDialogue* UDialogueEdGraph::GetDialogue() const
{
	return CastChecked<UDialogue>(GetOuter());
}

void UDialogueEdGraph::AddToNodeMap(UGraphNodeDialogue* InNode)
{
	check(InNode);
	NodeMap.Add(InNode->GetID(), InNode);
}

void UDialogueEdGraph::RemoveFromNodeMap(FName RemoveID)
{
	NodeMap.FindAndRemoveChecked(RemoveID);
}

bool UDialogueEdGraph::ContainsNode(FName InID) const
{
	return NodeMap.Contains(InID);
}

UGraphNodeDialogue* UDialogueEdGraph::GetNode(FName InID) const
{
	if (NodeMap.Contains(InID))
	{
		return NodeMap[InID];
	}

	return nullptr;
}

TArray<UGraphNodeDialogue*> UDialogueEdGraph::GetAllNodes() const
{
	TArray<UGraphNodeDialogue*> DialogueNodes;
	for (auto& Entry : NodeMap)
	{
		if (Entry.Value)
		{
			DialogueNodes.Add(Entry.Value);
		}
	}

	return DialogueNodes;
}

bool UDialogueEdGraph::HasSpeaker(FName InName) const
{
	return GetDialogue()->GetSpeakerRoles().Contains(InName);
}

TArray<UDialogueSpeakerSocket*> UDialogueEdGraph::GetAllSpeakers() const
{
	TArray<UDialogueSpeakerSocket*> AllSpeakers;
	for (auto& Entry : GetDialogue()->GetSpeakerRoles())
	{
		AllSpeakers.Add(Entry.Value.SpeakerSocket);
	}
	return AllSpeakers;
}

FColor UDialogueEdGraph::GetSpeakerColor(FName InName) const
{
	if (GetDialogue()->GetSpeakerRoles().Contains(InName))
	{
		return GetDialogue()->GetSpeakerRoles()[InName].GraphColor;
	}

	return FColor::White;
}

void UDialogueEdGraph::CompileAsset()
{
	UDialogue* Asset = GetDialogue();
	check(Asset && Root);
	
	Asset->PreCompileDialogue();
	
	ClearAssetNodes();
	
	if (!CanCompileAsset())
	{
		Asset->SetCompileStatus(EDialogueCompileStatus::Failed);
		return;
	}

	//Set up speakers and root
	Root->CreateAssetNode(Asset);
	Asset->SetRootNode(Root->GetAssetNode());

	//Compile asset tree
	CreateAssetNodes(Asset);
	UpdateAssetTreeRecursive(Root);
	FinalizeAssetNodes();

	//Mark compilation as successful 
	Asset->PostCompileDialogue();
	Asset->SetCompileStatus(EDialogueCompileStatus::Compiled);
}

bool UDialogueEdGraph::CanCompileAsset() const
{
	//Get all nodes
	TArray<UGraphNodeDialogue*> DialogueNodes;
	GetNodesOfClass<UGraphNodeDialogue>(DialogueNodes);

	//Verify all nodes can compile
	for (UGraphNodeDialogue* Node : DialogueNodes)
	{
		if (!Node->CanCompileNode())
		{
			return false;
		}
	}

	return true;
}

void UDialogueEdGraph::UpdateAllNodeVisuals()
{
	for (const auto& Entry : NodeMap)
	{
		Entry.Value->UpdateDialogueNode();
	}
}

void UDialogueEdGraph::ClearAssetNodes()
{
	for (UEdGraphNode* Node : Nodes)
	{
		if (UGraphNodeDialogue* DialogueNode = Cast<UGraphNodeDialogue>(Node))
		{
			DialogueNode->ClearAssetNode();
		}
	}
}

void UDialogueEdGraph::CreateAssetNodes(UDialogue* InAsset)
{
	for (auto& Entry : NodeMap)
	{
		check(Entry.Value);
		if(Entry.Value->IsA<UGraphNodeDialogueEntry>())continue;
		Entry.Value->CreateAssetNode(InAsset);
	}
}

void UDialogueEdGraph::FinalizeAssetNodes()
{
	for (auto& Entry : NodeMap)
	{
		check(Entry.Value);
		Entry.Value->FinalizeAssetNode();
	}
}

void UDialogueEdGraph::UpdateAssetTreeRecursive(UGraphNodeDialogue* InRoot)
{
	check(InRoot);

	//Link the asset root to its parents
	InRoot->LinkAssetNode();
	
	TArray<UGraphNodeDialogue*> OutChildren;
	InRoot->GetChildren(OutChildren);
	UGraphNodeDialogue::SortNodesLeftToRight(OutChildren);

	//Recur over children
	for (UGraphNodeDialogue* Child : OutChildren)
	{
		UpdateAssetTreeRecursive(Child);
	}
}

void UDialogueEdGraph::OnDialogueGraphChanged(const FEdGraphEditAction& EditAction)
{
	//If removing a node, pull that node from the node map
	if (EditAction.Action == GRAPHACTION_RemoveNode)
	{ 
		for (const UEdGraphNode* Removed : EditAction.Nodes)
		{
			if (const UGraphNodeDialogue* RemovedNode = Cast<UGraphNodeDialogue>(Removed))
			{
				NodeMap.Remove(RemovedNode->GetID());
			}
		}
	}
}

void UDialogueEdGraph::OnSpeakerRolesChanged()
{
	CanCompileAsset(); //Check for error banners
	UpdateAllNodeVisuals();
}


