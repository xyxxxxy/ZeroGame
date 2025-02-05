
#pragma once

#include "CoreMinimal.h"
#include "DialogueOption.h"

#include "DialogueNode.Generated.h"

class UDialogue;

UCLASS(Abstract)
class DIALOGUERUNTIME_API UDialogueNode : public UObject
{
	GENERATED_BODY()
	
public:
	UDialogue* GetDialogue() const;
	void SetDialogue(UDialogue* InDialogue);

	void AddChild(UDialogueNode* InNode);
	void AddParent(UDialogueNode* InNode);

	TArray<UDialogueNode*> GetChildren() const;
	TArray<UDialogueNode*> GetParent() const;
	
	virtual void EnterNode();
	virtual  void Skip(){};

	int32 GetNodeIndex() const;
	void SetNodeIndex(int32 Index);
	
	virtual FDialogueOption GetAsOption();
	virtual void SelectOption(int32 InOptionIndex);
	
protected:
	UPROPERTY()
	TObjectPtr<UDialogue> Dialogue;

	UPROPERTY()
	TArray<TObjectPtr<UDialogueNode>> Children;

	UPROPERTY()
	TArray<TObjectPtr<UDialogueNode>> Parent;
	
private:
	int32 NodeIndex;
};