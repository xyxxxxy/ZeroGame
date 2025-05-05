
#include "Tabs/DialogueViewportTabFactory.h"
#include "Asset/DialogueAssetEditor.h"
#include "Graph/Slate/SDialogueGraphEditor.h"
#include "Tabs/DialogueEditorTabs.h"


#define LOCTEXT_NAMESPACE "DialogueViewportTabFactory"

FDialogueViewportTabFactory::FDialogueViewportTabFactory(TSharedPtr<FDialogueAssetEditor> InEditor)
: FWorkflowTabFactory(FDialogueEditorTabs::ViewportTabID, InEditor)
{
	Editor = InEditor;
	TabLabel = LOCTEXT("DialogueViewportTabLabel", "Dialogue Graph");
	// What for? FWorkflowTabFactory::CreateViewMenuEntry(FMenuBuilder& MenuBuilder, const FUIAction& Action)
	ViewMenuDescription = LOCTEXT("DialogueViewportViewMenuDescription", "ViewportTabMenu");
	ViewMenuTooltip = LOCTEXT("DialogueViewportViewMenuTooltip","Show the ViewportMenu.");
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
	return LOCTEXT("DialogueViewportTabToolTipText", "A Viewport for doing Viewport thing.");
}

#undef LOCTEXT_NAMESPACE