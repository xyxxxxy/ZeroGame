
#include "Graph/Nodes/GraphNodeDialogueEntry.h"
#include "Dialogue.h"
#include "Nodes/DialogueEntryNode.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GraphNodeDialogueEntry)

#define LOCTEXT_NAMESPACE "GraphNodeDialogueEntry"

void UGraphNodeDialogueEntry::AllocateDefaultPins()
{
	CreatePin(EGPD_Output, "MultipleNodes", FName());
}

FText UGraphNodeDialogueEntry::GetTooltipText() const
{
	return LOCTEXT("Tooltip", "The starting point for the graph.");
}

FText UGraphNodeDialogueEntry::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("Title", "Entry");
}

bool UGraphNodeDialogueEntry::CanUserDeleteNode() const
{
	return false;
}

bool UGraphNodeDialogueEntry::CanDuplicateNode() const
{
	return false;
}

FText UGraphNodeDialogueEntry::GetContextMenuName() const
{
	return FText::FromString("Entry Node");
}

void UGraphNodeDialogueEntry::CreateAssetNode(UDialogue* InAsset)
{
	UDialogueEntryNode* NewNode = NewObject<UDialogueEntryNode>(InAsset);

	SetAssetNode(NewNode);
	InAsset->AddNode(NewNode);
	InAsset->SetRootNode(NewNode);
}

FName UGraphNodeDialogueEntry::GetBaseID() const
{
	return FName("Entry");
}

#undef LOCTEXT_NAMESPACE