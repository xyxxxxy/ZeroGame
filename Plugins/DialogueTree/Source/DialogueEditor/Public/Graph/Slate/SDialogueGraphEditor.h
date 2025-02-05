
#pragma once

#include "GraphEditor.h"

class FDialogueAssetEditor;
class UDialogue;

class DIALOGUEEDITOR_API SDialogueGraphEditor : public SGraphEditor
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphEditor){}
	SLATE_ARGUMENT(FGraphEditorEvents, GraphEvents)
	SLATE_ARGUMENT(TSharedPtr<IDetailsView>, DetailsView)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedPtr<FDialogueAssetEditor> InEditor);

public:
	void OnSelectedNodesChanged(const TSet<UObject*>& SelectedObjects);

public:
	virtual void BindGraphCommands();

	static void UndoGraphAction();
	static bool CanEdit();

	bool IsTabFocused() const;

	void DeleteSelectedNodes();
	bool CanDeleteNodes();

protected:
	TWeakObjectPtr<UDialogue> Dialogue;
	TWeakPtr<FDialogueAssetEditor> Editor;
	TWeakPtr<IDetailsView> NodeDetailsView;
	
	TSharedPtr<FUICommandList> CommandList;
};
