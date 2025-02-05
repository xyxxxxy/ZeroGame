
#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

class FDialogueAssetEditor;

class DIALOGUEEDITOR_API FDialogueNodeDetailsTabFactory : public FWorkflowTabFactory
{
public:
	FDialogueNodeDetailsTabFactory(TSharedPtr<FDialogueAssetEditor> InEditor);
	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<FDialogueAssetEditor> Editor;
};
