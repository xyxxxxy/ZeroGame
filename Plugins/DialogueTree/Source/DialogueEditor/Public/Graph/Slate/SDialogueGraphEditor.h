
#pragma once

#include "GraphEditor.h"
#include "Graph/DialogueEdGraph.h"

class FDialogueAssetEditor;
class UDialogue;
class UGraphNodeDialogue;
class DIALOGUEEDITOR_API SDialogueGraphEditor : public SGraphEditor
{
public:
	SLATE_BEGIN_ARGS(SDialogueGraphEditor){}
	SLATE_ARGUMENT(FGraphEditorEvents, GraphEvents)
	SLATE_ARGUMENT(TSharedPtr<IDetailsView>, DetailsView)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const TSharedPtr<FDialogueAssetEditor> InEditor);
	virtual void BindGraphCommands();

protected:
	void OnSelectedNodesChanged(const TSet<UObject*>& SelectedObjects);

public:
	FOnSelectionChanged OnSelectionChangedEvent;

private:
	static void UndoGraphAction();
	static void RedoGraphAction();

public:
	virtual bool IsTabFocused() const;
	static bool CanEdit();
	static bool IsPIE();
	static bool IsPlaySessionPaused();
	
protected:
	virtual void DeleteSelectedNodes();
	virtual void DeleteSelectedDuplicableNodes();
	virtual bool CanDeleteNodes() const;

	virtual void CopySelectedNodes() const;
	static void PrepareFlowGraphNodeForCopy(UGraphNodeDialogue& FlowGraphNode, const int32 ParentEdNodeIndex, FGraphPanelSelectionSet& NewSelectedNodes);
	virtual bool CanCopyNodes() const;

	virtual void CutSelectedNodes();
	virtual bool CanCutNodes() const;

	virtual void PasteNodes();
	virtual bool CanPasteNodes() const;

	void PasteNodesHere(const FVector2D& Location);
	static TSet<UEdGraphNode*> ImportNodesToPasteFromClipboard(UDialogueEdGraph& FlowGraph, FString& OutTextToImport);
	TArray<UGraphNodeDialogue*> DerivePasteTargetNodesFromSelectedNodes() const;
protected:
	TWeakObjectPtr<UDialogue> Dialogue;
	TWeakPtr<FDialogueAssetEditor> Editor;
	TSharedPtr<IDetailsView> NodeDetailsView;
	
	TSharedPtr<FUICommandList> CommandList;
};
