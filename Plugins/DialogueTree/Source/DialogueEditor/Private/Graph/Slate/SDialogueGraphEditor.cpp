
#include "Graph/Slate/SDialogueGraphEditor.h"

#include "Asset/DialogueAssetEditor.h"
#include "Dialogue.h"
#include "Graph/Nodes/GraphNodeDialogue.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "EdGraphUtilities.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Graph/DialogueEdGraph.h"

#define LOCTEXT_NAMESPACE "SDialogueGraphEditor"

void SDialogueGraphEditor::Construct(const FArguments &InArgs, const TSharedPtr<FDialogueAssetEditor> InEditor)
{
	Editor = InEditor;
	Dialogue = Editor.Pin()->GetDialogue();
	NodeDetailsView = InArgs._DetailsView;

	InEditor->SetGraph(Dialogue->GetGraph());
	InEditor->SetGraphEditor(SharedThis(this));

	BindGraphCommands();

	SGraphEditor::FArguments Arguments;
	Arguments._AdditionalCommands = CommandList;
	Arguments._GraphToEdit = Dialogue->GetGraph();
	Arguments._GraphEvents = InArgs._GraphEvents;
	Arguments._AutoExpandActionMenu = true;

	Arguments._GraphEvents.OnSelectionChanged = FOnSelectionChanged::CreateSP(this, &SDialogueGraphEditor::OnSelectedNodesChanged);

	SGraphEditor::Construct(Arguments);
}

void SDialogueGraphEditor::OnSelectedNodesChanged(const TSet<UObject *> &SelectedObjects)
{
	TSharedPtr<IDetailsView> DetailsView = NodeDetailsView.ToSharedRef();
	if (SelectedObjects.Num() < 1)
	{
		DetailsView->SetObject(nullptr);
		return;
	}

	TArray<UObject *> SelectedNodes;
	for (auto &CurrentObject : SelectedObjects)
	{
		if (UGraphNodeDialogue *DialogueNode = Cast<UGraphNodeDialogue>(CurrentObject))
		{
			SelectedNodes.Add(CurrentObject);
		}
	}

	if (DetailsView.IsValid())
	{
		DetailsView->SetObjects(SelectedNodes);
	}

	OnSelectionChangedEvent.ExecuteIfBound(SelectedObjects);
}

void SDialogueGraphEditor::BindGraphCommands()
{
	FGraphEditorCommands::Register();
	FGenericCommands::Register();

	const FGenericCommands &GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl &GraphEditorCommands = FGraphEditorCommands::Get();

	CommandList = MakeShareable(new FUICommandList);

	CommandList->MapAction(GenericCommands.Undo,
						   FExecuteAction::CreateStatic(&SDialogueGraphEditor::UndoGraphAction),
						   FCanExecuteAction::CreateStatic(&SDialogueGraphEditor::CanEdit));

	CommandList->MapAction(GenericCommands.Redo,
						   FExecuteAction::CreateStatic(&SDialogueGraphEditor::RedoGraphAction),
						   FCanExecuteAction::CreateStatic(&SDialogueGraphEditor::CanEdit));

	CommandList->MapAction(GenericCommands.Delete,
						   FExecuteAction::CreateSP(this, &SDialogueGraphEditor::DeleteSelectedNodes),
						   FCanExecuteAction::CreateSP(this, &SDialogueGraphEditor::CanDeleteNodes));

	CommandList->MapAction(GenericCommands.Copy,
						   FExecuteAction::CreateSP(this, &SDialogueGraphEditor::CopySelectedNodes),
						   FCanExecuteAction::CreateSP(this, &SDialogueGraphEditor::CanCopyNodes));

	CommandList->MapAction(GenericCommands.Cut,
						   FExecuteAction::CreateSP(this, &SDialogueGraphEditor::CutSelectedNodes),
						   FCanExecuteAction::CreateSP(this, &SDialogueGraphEditor::CanCutNodes));

	CommandList->MapAction(GenericCommands.Paste,
						   FExecuteAction::CreateSP(this, &SDialogueGraphEditor::PasteNodes),
						   FCanExecuteAction::CreateSP(this, &SDialogueGraphEditor::CanPasteNodes));
}

bool SDialogueGraphEditor::IsTabFocused() const
{
	return true;
}

void SDialogueGraphEditor::UndoGraphAction()
{
	GEditor->UndoTransaction();
}

void SDialogueGraphEditor::RedoGraphAction()
{
	GEditor->RedoTransaction();
}

bool SDialogueGraphEditor::CanEdit()
{
	return GEditor->PlayWorld == nullptr;
}

bool SDialogueGraphEditor::IsPIE()
{
	return GEditor->PlayWorld != nullptr;
}

bool SDialogueGraphEditor::IsPlaySessionPaused()
{
	bool bPaused = true;

	// GUnrealEd
	// #include "UnrealEdGlobals.h"
	// #include "Editor/UnrealEdEngine.h"
	for (const FWorldContext &PieContext : GUnrealEd->GetWorldContexts())
	{
		const UWorld *PlayWorld = PieContext.World();
		if (PlayWorld && PlayWorld->IsGameWorld())
		{
			bPaused = bPaused && PlayWorld->bDebugPauseExecution;
		}
	}

	return bPaused;
}

void SDialogueGraphEditor::DeleteSelectedNodes()
{
	// Set up transaction
	const FScopedTransaction Transaction(LOCTEXT("DeleteSelectedNode", "Delete Selected Node"));
	GetCurrentGraph()->Modify();
	Dialogue->Modify();

	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	ClearSelectionSet();

	// Delete any nodes that allow deletion
	for (UObject *Item : SelectedNodes)
	{
		// Convert selected item to a node
		UEdGraphNode *Node = Cast<UEdGraphNode>(Item);

		if (Node && Node->CanUserDeleteNode())
		{
			Node->Modify();
			Node->GetSchema()->BreakNodeLinks(*Node);
			Node->DestroyNode();

			// OnSelectedNodesChanged(GetSelectedNodes());
		}
	}
}

bool SDialogueGraphEditor::CanDeleteNodes() const
{

	if (CanEdit() && IsTabFocused())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			if (const UEdGraphNode *Node = Cast<UEdGraphNode>(*NodeIt))
			{
				if (!Node->CanUserDeleteNode())
				{
					return false;
				}
			}
		}

		return SelectedNodes.Num() > 0;
	}

	return false;
}

void SDialogueGraphEditor::DeleteSelectedDuplicableNodes()
{
	// Cache off the old selection
	const FGraphPanelSelectionSet OldSelectedNodes = GetSelectedNodes();

	// Clear the selection and only select the nodes that can be duplicated
	FGraphPanelSelectionSet RemainingNodes;
	ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIt(OldSelectedNodes); SelectedIt; ++SelectedIt)
	{
		if (UEdGraphNode *Node = Cast<UEdGraphNode>(*SelectedIt))
		{
			if (Node->CanDuplicateNode())
			{
				SetNodeSelection(Node, true);
			}
			else
			{
				RemainingNodes.Add(Node);
			}
		}
	}

	// Delete the duplicable nodes
	DeleteSelectedNodes();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIt(RemainingNodes); SelectedIt; ++SelectedIt)
	{
		if (UEdGraphNode *Node = Cast<UEdGraphNode>(*SelectedIt))
		{
			SetNodeSelection(Node, true);
		}
	}
}

void SDialogueGraphEditor::CopySelectedNodes() const
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	FGraphPanelSelectionSet NewSelectedNodes;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UGraphNodeDialogue *DialogueGraphNode = Cast<UGraphNodeDialogue>(*SelectedIter))
		{
			constexpr int32 RootEdNodeParentIndex = INDEX_NONE;
			PrepareFlowGraphNodeForCopy(*DialogueGraphNode, RootEdNodeParentIndex, NewSelectedNodes);
		}
		else
		{
			NewSelectedNodes.Add(*SelectedIter);
		}
	}

	FString ExportedText;

	FEdGraphUtilities::ExportNodesToText(NewSelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

	for (FGraphPanelSelectionSet::TIterator SelectedIter(NewSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UGraphNodeDialogue *DialogueGraphNode = Cast<UGraphNodeDialogue>(*SelectedIter))
		{
			// TODO: UGraphNodeDialogue->PostCopyNode()
			// DialogueGraphNode->PostCopyNode();
		}
	}
}

void SDialogueGraphEditor::PrepareFlowGraphNodeForCopy(UGraphNodeDialogue &FlowGraphNode, const int32 ParentEdNodeIndex, FGraphPanelSelectionSet &NewSelectedNodes)
{
}

bool SDialogueGraphEditor::CanCopyNodes() const
{
	if (CanEdit() && IsTabFocused())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator SelectedIt(SelectedNodes); SelectedIt; ++SelectedIt)
		{
			const UEdGraphNode *Node = Cast<UEdGraphNode>(*SelectedIt);
			if (Node && Node->CanDuplicateNode())
			{
				return true;
			}
		}
	}

	return false;
}

void SDialogueGraphEditor::CutSelectedNodes()
{
	CopySelectedNodes();

	// Cut should only delete nodes that can be duplicated
	DeleteSelectedDuplicableNodes();
}

bool SDialogueGraphEditor::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void SDialogueGraphEditor::PasteNodes()
{
	PasteNodesHere(GetPasteLocation());
}

bool SDialogueGraphEditor::CanPasteNodes() const
{
	return true;
}

void SDialogueGraphEditor::PasteNodesHere(const FVector2D &Location)
{
	// Undo/Redo support
	const FScopedTransaction Transaction(LOCTEXT("PasteNode", "Paste Node"));
	UDialogueEdGraph *DialogueGraph = CastChecked<UDialogueEdGraph>(Dialogue->GetGraph());
	DialogueGraph->Modify();
	Dialogue->Modify();

	// DialogueGraph->LockUpdates();
}

TSet<UEdGraphNode *> SDialogueGraphEditor::ImportNodesToPasteFromClipboard(UDialogueEdGraph &FlowGraph, FString &OutTextToImport)
{
	// Grab the text to paste from the clipboard.
	FPlatformApplicationMisc::ClipboardPaste(OutTextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> NodesToPaste;
	FEdGraphUtilities::ImportNodesFromText(&FlowGraph, OutTextToImport, /*out*/ NodesToPaste);

	return NodesToPaste;
}

TArray<UGraphNodeDialogue *> SDialogueGraphEditor::DerivePasteTargetNodesFromSelectedNodes() const
{
	TArray<UGraphNodeDialogue*> PasteTargetNodes;
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UGraphNodeDialogue* Node = Cast<UGraphNodeDialogue>(*SelectedIter);
		if (IsValid(Node))
		{
			PasteTargetNodes.Add(Node);
		}
	}

	return PasteTargetNodes;
}

#undef LOCTEXT_NAMESPACE