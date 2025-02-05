
#pragma once

#include "Graph/Nodes/GraphNodeDialogueBase.h"
#include "GraphNodeDialogue.generated.h"

class UDialogueEdGraph;
class UDialogueNode;

DECLARE_DELEGATE(FOnUpdateNode);

UCLASS(Abstract)
class DIALOGUEEDITOR_API UGraphNodeDialogue : public UGraphNodeDialogueBase
{
	GENERATED_BODY()
public:
	UGraphNodeDialogue();

public:
	virtual void PostEditUndo() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	/** UEdGraphNode Impl. */
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void ResizeNode(const FVector2D& NewSize) override;
	/** UEnd EdGraphNode */
	
	FName GetID() const;
	virtual FName GetBaseID() const;
	void ResetID();
	
	UDialogueEdGraph* GetDialogueGraph() const;
	
	void BindOnUpdateVisuals(const FOnUpdateNode& UpdateDelegate);
	
	void InitNodeInDialogueGraph(UEdGraph* OwningGraph);
	static void SortNodesLeftToRight(TArray<UGraphNodeDialogue*>& Nodes);
	
	void SetErrorFlag(bool InFlag);
	bool HasError() const;
	
	virtual bool CanCompileNode();
	UDialogueNode* GetAssetNode() const;
	void SetAssetNode(UDialogueNode* InDialogueNode);
	virtual void CreateAssetNode(class UDialogue* InAsset);

	/**
	* Virtual. Performs last touches on the asset node after all asset nodes
	* have been created and linked together. 
	*/
	virtual void FinalizeAssetNode();
	void LinkAssetNode();
	void ClearAssetNode();
	

	
	void GetPinChildren(UEdGraphPin* InPin, TArray<UGraphNodeDialogue*>& OutNodes) const;
	
	void MarkDialogueDirty();
	virtual void UpdateDialogueNode();

	void GetParents(TArray<UGraphNodeDialogue*>& OutNodes) const;
	void GetChildren(TArray<UGraphNodeDialogue*>& OutNodes) const;

protected:
	void LinkToParent(UGraphNodeDialogue* InParent);
	void LinkToChild(UGraphNodeDialogue* InChild);

private:
	void GetParents_Internal(TArray<UGraphNodeDialogueBase*> InParents, TArray<UGraphNodeDialogue*>& OutNodes) const;
	void GetChildren_Internal(TArray<UGraphNodeDialogueBase*> InChildren, TArray<UGraphNodeDialogue*>& OutNodes) const;

private:
	UPROPERTY()
	TObjectPtr<UDialogueNode> AssetNode;
	
	/** A flag indicating that the node has a compilation error */
	UPROPERTY()
	bool bDialogueError = false;
	
	UPROPERTY()
	TObjectPtr<UDialogueEdGraph> DialogueGraph;
	
	UPROPERTY()
	FName ID;

	/** Event delegate for when the node changes */
	FOnUpdateNode OnUpdateVisuals;
	
	const FLinearColor DEFAULT_GRAY = FLinearColor(105.f, 105.f, 105.f);
};