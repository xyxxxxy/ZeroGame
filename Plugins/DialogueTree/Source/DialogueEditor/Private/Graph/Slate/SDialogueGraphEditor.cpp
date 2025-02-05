
#include "Graph/Slate/SDialogueGraphEditor.h"

#include "Asset/DialogueAssetEditor.h"
#include "Dialogue.h"
#include "Graph/Nodes/GraphNodeDialogue.h"
#include "GraphEditorActions.h"
#include "Framework/Commands/GenericCommands.h"

void SDialogueGraphEditor::Construct(const FArguments& InArgs, const TSharedPtr<FDialogueAssetEditor> InEditor)
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

	Arguments._GraphEvents.OnSelectionChanged = FOnSelectionChanged::CreateSP(this,&SDialogueGraphEditor::OnSelectedNodesChanged);

	SGraphEditor::Construct(Arguments);
}

void SDialogueGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& SelectedObjects)
{
	TSharedPtr<IDetailsView> DetailsView = NodeDetailsView.Pin();
	if(SelectedObjects.Num() < 1)
	{
		DetailsView->SetObject(nullptr);
		return;
	}

	TArray<UObject*> SelectedNodes;
	for(auto& CurrentObject : SelectedObjects)
	{
		if(UGraphNodeDialogue* DialogueNode = Cast<UGraphNodeDialogue>(CurrentObject))
		{
			SelectedNodes.Add(CurrentObject);
		}
	}
	check(DetailsView);
	DetailsView->SetObjects(SelectedNodes);
	
}

void SDialogueGraphEditor::BindGraphCommands()
{
	FGraphEditorCommands::Register();
	FGenericCommands::Register();

	const FGenericCommands& GenericCommands = FGenericCommands::Get();
	const FGraphEditorCommandsImpl& GraphEditorCommands = FGraphEditorCommands::Get();

	CommandList = MakeShareable(new FUICommandList);

	CommandList->MapAction(
		GenericCommands.Undo, 
		FExecuteAction::CreateStatic(&SDialogueGraphEditor::UndoGraphAction), 
		FCanExecuteAction::CreateStatic(&SDialogueGraphEditor::CanEdit)
	);	
	CommandList->MapAction(
		GenericCommands.Delete, 
		FExecuteAction::CreateSP(this, &SDialogueGraphEditor::DeleteSelectedNodes), 
		FCanExecuteAction::CreateSP(this, &SDialogueGraphEditor::CanDeleteNodes)
	);	
}

void SDialogueGraphEditor::UndoGraphAction()
{
	GEditor->UndoTransaction();
}

bool SDialogueGraphEditor::CanEdit()
{
	return GEditor->PlayWorld == nullptr;
}

bool SDialogueGraphEditor::IsTabFocused() const
{
	return true;
}

void SDialogueGraphEditor::DeleteSelectedNodes()
{
    //Set up transaction
    const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
    GetCurrentGraph()->Modify();

    FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
    ClearSelectionSet();

    //Delete any nodes that allow deletion 
    for (UObject* Item : SelectedNodes)
    {
        //Convert selected item to a node 
        UEdGraphNode* Node = Cast<UEdGraphNode>(Item);

        if (Node && Node->CanUserDeleteNode())
        {
            Node->Modify();
            Node->GetSchema()->BreakNodeLinks(*Node);
            Node->DestroyNode();
			
			//OnSelectedNodesChanged(GetSelectedNodes());
        }
    }
}

bool SDialogueGraphEditor::CanDeleteNodes()
{
	if (CanEdit() && IsTabFocused())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			if (const UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
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
