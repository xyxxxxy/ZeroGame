
#pragma once

#include "DialogueSpeakerSocket.h"
#include "EdGraph/EdGraph.h"
#include "DialogueEdGraph.generated.h"

class UGraphNodeDialogue;
class UDialogue;

UCLASS()
class DIALOGUEEDITOR_API UDialogueEdGraph : public UEdGraph
{
	GENERATED_BODY()
public:
	UDialogueEdGraph();
	
public:
	virtual bool Modify(bool bAlwaysMarkDirty) override;
	virtual void PostEditUndo() override;
	virtual void PostInitProperties() override;
	
	void SetGraphRoot(UGraphNodeDialogue* InRoot);
	UDialogue* GetDialogue() const;
	
	void AddToNodeMap(UGraphNodeDialogue* InNode);
	void RemoveFromNodeMap(FName RemoveID);
	
	bool ContainsNode(FName InID) const;
	UGraphNodeDialogue* GetNode(FName InID) const;
	TArray<UGraphNodeDialogue*> GetAllNodes() const;
	
	bool HasSpeaker(FName InName) const;
	TArray<UDialogueSpeakerSocket*> GetAllSpeakers() const;
	
	FColor GetSpeakerColor(FName InName) const;
	
	void CompileAsset();
	bool CanCompileAsset() const;
	
	void UpdateAllNodeVisuals();

private: 
	void ClearAssetNodes();
	void CreateAssetNodes(UDialogue* InAsset);
	void FinalizeAssetNodes();
	void UpdateAssetTreeRecursive(UGraphNodeDialogue* InRoot);
	
	void OnDialogueGraphChanged(const FEdGraphEditAction& EditAction);
	
	UFUNCTION()
	void OnSpeakerRolesChanged();

private:
	UPROPERTY()
	TObjectPtr<UGraphNodeDialogue> Root;
	
	UPROPERTY()
	TMap<FName, TObjectPtr<UGraphNodeDialogue>> NodeMap;
};