
#include "Tabs/DialogueNodeDetailsTabFactory.h"

#include "Tabs/DialogueEditorTabs.h"
#include "Asset/DialogueAssetEditor.h"

#define LOCTEXT_NAMESPACE "DialogueNodeDetailsTabFactory"

FDialogueNodeDetailsTabFactory::FDialogueNodeDetailsTabFactory(TSharedPtr<FDialogueAssetEditor> InEditor)
:FWorkflowTabFactory(FDialogueEditorTabs::NodeDetailsTabID, InEditor)
{
	Editor = InEditor;
	TabLabel = LOCTEXT("DialogueNodeDetailsTabLabel","Node Details");
	ViewMenuDescription = LOCTEXT("DialogueNodeDetailsViewMenuDescription","Display the Node Details view for current dialogue asset.");
	ViewMenuTooltip = LOCTEXT("DialogueNodeDetailsViewMenuTooltip","Show the Node Details.");
}

TSharedRef<SWidget> FDialogueNodeDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueAssetEditor> EditorPtr = Editor.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bHideSelectionTip = false;
		DetailsViewArgs.bShowCustomFilterOption = true;
		DetailsViewArgs.bAllowSearch = true;
		DetailsViewArgs.NotifyHook = EditorPtr.Get();
	}
	EditorPtr->GetNodeDetailView() = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	//check(EditorPtr->GetNodeDetailView());
	EditorPtr->GetNodeDetailView()->SetObject(nullptr);
	
	return SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.HAlign(HAlign_Fill)
				[
					EditorPtr->GetNodeDetailView().ToSharedRef()
				];
}

FText FDialogueNodeDetailsTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DialogueNodeDetailsTabToolTipText","The Node Details view for modifying Node Details.");
}

#undef LOCTEXT_NAMESPACE