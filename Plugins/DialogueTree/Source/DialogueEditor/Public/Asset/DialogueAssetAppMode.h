
#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"

class FWorkflowAllowedTabSet;
class FDialogueAssetEditor;

class FDialogueAssetAppMode : public FApplicationMode
{
public:
	FDialogueAssetAppMode(TSharedPtr<FDialogueAssetEditor> InEditor);
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<FDialogueAssetEditor> Editor;
	FWorkflowAllowedTabSet Tabs;
};
