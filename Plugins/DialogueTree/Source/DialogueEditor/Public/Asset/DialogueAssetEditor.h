
#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FDialogueEditorToolbar;
class UDialogue;

class FDialogueAssetEditor : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook, public FGCObject
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);
	
	UDialogue* GetDialogue() const;

	void CreateGraph();

	void SetGraph(UEdGraph* InGraph);
	UEdGraph* GetGraph() const;

	void SetGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor);
	TSharedPtr<SGraphEditor> GetGraphEditor() const;

public:
	// FAssetEditorToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;
	virtual bool OnRequestClose(EAssetEditorCloseReason InCloseReason) override;
	// ~ Interface

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;

	void SetNodeDetailView(TSharedPtr<IDetailsView> InDetailView);
	TSharedPtr<IDetailsView> GetNodeDetailView() const;

	void SpawnInitialNodes();

	void CreateToolbar();
	void BindToolbarCommands();
	

	
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	
	void OnCompile();
	
	

	
	void OnChangeSelection(const TSet<UObject*>& SelectedObjects);

private:
	TObjectPtr<UDialogue> Dialogue;
	TObjectPtr<UEdGraph> Graph;

	TSharedPtr<IDetailsView> NodeDetailsView;

	TWeakPtr<SGraphEditor> GraphEditor;

	TSharedPtr<FUICommandList> ToolKitCommands;
	TSharedPtr<FDialogueEditorToolbar> AssetToolbar;
};
