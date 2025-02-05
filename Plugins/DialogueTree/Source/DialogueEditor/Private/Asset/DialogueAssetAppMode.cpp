
#include "Asset/DialogueAssetAppMode.h"
#include "Asset/DialogueAssetEditor.h"
#include "Tabs/DialogueViewportTabFactory.h"
#include "Tabs/DialoguePropertiesTabFactory.h"
#include "Tabs/DialogueEditorTabs.h"
#include "Tabs/DialogueNodeDetailsTabFactory.h"

FDialogueAssetAppMode::FDialogueAssetAppMode( TSharedPtr<FDialogueAssetEditor> InEditor)
: FApplicationMode(TEXT("DialogueAssetAppMode"))
{
	Editor = InEditor;
	
	Tabs.RegisterFactory(MakeShareable(new FDialogueViewportTabFactory(InEditor)));
	Tabs.RegisterFactory(MakeShareable(new FDialoguePropertiesTabFactory(InEditor)));
	Tabs.RegisterFactory(MakeShareable(new FDialogueNodeDetailsTabFactory(InEditor)));
	
	TabLayout = FTabManager::NewLayout("DialogueAssetAppMode_Layout_v1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.75)
			->AddTab(FDialogueEditorTabs::ViewportTabID,ETabState::OpenedTab)
			->SetHideTabWell(true)
			)			
			->Split
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.25)
				->SetOrientation(Orient_Vertical)
				->Split
				(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.5)
				->AddTab(FDialogueEditorTabs::GraphPropertiesTabID,ETabState::OpenedTab)
				)
				->Split
				(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.5)
				->AddTab(FDialogueEditorTabs::NodeDetailsTabID,ETabState::OpenedTab)	
				)			
			)
		)			
	);
}

void FDialogueAssetAppMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	TSharedPtr<FDialogueAssetEditor> App = Editor.Pin();
	App->PushTabFactories(Tabs);
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FDialogueAssetAppMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FDialogueAssetAppMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
