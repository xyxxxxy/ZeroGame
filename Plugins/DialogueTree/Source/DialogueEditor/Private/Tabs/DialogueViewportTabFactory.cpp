
#include "Tabs/DialogueViewportTabFactory.h"
#include "Asset/DialogueAssetEditor.h"
#include "Graph/Slate/SDialogueGraphEditor.h"
#include "Tabs/DialogueEditorTabs.h"


#define LOCTEXT_NAMESPACE "DialogueViewportTabFactory"

FDialogueViewportTabFactory::FDialogueViewportTabFactory(TSharedPtr<FDialogueAssetEditor> InEditor)
:FWorkflowTabFactory(FDialogueEditorTabs::ViewportTabID,InEditor)
{
	Editor = InEditor;
	TabLabel = LOCTEXT("DialogueViewportTabLabel","Viewport");
	ViewMenuDescription = LOCTEXT("DialogueViewportViewMenuDescription","Display a Viewport for whatever you want to do.");
	ViewMenuTooltip = LOCTEXT("DialogueViewportViewMenuTooltip","Show the Viewport.");
}

TSharedRef<SWidget> FDialogueViewportTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueAssetEditor> EditorPtr = Editor.Pin();

	TSharedPtr<SGraphEditor> GraphEditor;
	SAssignNew(GraphEditor, SDialogueGraphEditor, EditorPtr)
		.DetailsView(EditorPtr->GetNodeDetailView());

	//EditorPtr->SetGraphEditor(GraphEditor);
	

	return SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.HAlign(HAlign_Fill)
				[
					GraphEditor.ToSharedRef()
				];
}

FText FDialogueViewportTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DialogueViewportTabToolTipText","A Viewport for doing Viewport thing.");
}

#undef LOCTEXT_NAMESPACE