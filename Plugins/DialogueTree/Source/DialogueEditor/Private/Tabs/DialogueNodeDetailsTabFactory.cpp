
#include "Tabs/DialogueNodeDetailsTabFactory.h"
#include "Tabs/DialogueEditorTabs.h"
#include "Asset/DialogueAssetEditor.h"

#define LOCTEXT_NAMESPACE "DialogueNodeDetailsTabFactory"

FDialogueNodeDetailsTabFactory::FDialogueNodeDetailsTabFactory(TSharedPtr<FDialogueAssetEditor> InEditor)
: FWorkflowTabFactory(FDialogueEditorTabs::NodeDetailsTabID, InEditor)
{
	Editor = InEditor;
	TabLabel = LOCTEXT("DialogueNodeDetailsTabLabel", "Node Details");
	ViewMenuDescription = LOCTEXT("DialogueNodeDetailsViewMenuDescription", "NodeDetailsMenu");
	ViewMenuTooltip = LOCTEXT("DialogueNodeDetailsViewMenuTooltip", "Show the Node Details Menu.");
}

TSharedRef<SWidget> FDialogueNodeDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueAssetEditor> EditorPtr = Editor.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bHideSelectionTip = true;	// 设为 true 可隐藏默认提示文本"Select an object to view details."
		DetailsViewArgs.bShowCustomFilterOption = true;
		DetailsViewArgs.bAllowSearch = true;
		DetailsViewArgs.NotifyHook = EditorPtr.Get();	// FNotifyHook 提供接口用于捕获修改事件
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
	return LOCTEXT("DialogueNodeDetailsTabToolTipText", "The Node Details view for modifying Node Details.");
}

#undef LOCTEXT_NAMESPACE