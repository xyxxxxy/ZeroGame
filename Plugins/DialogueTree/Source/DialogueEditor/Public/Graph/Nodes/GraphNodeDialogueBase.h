
#pragma once

#include "CoreMinimal.h"
#include "DialogueConnectionLimit.h"

#include "EdGraph/EdGraphNode.h"
#include "GraphNodeDialogueBase.generated.h"

UCLASS(Abstract)
class UGraphNodeDialogueBase : public UEdGraphNode
{
	GENERATED_BODY()
public: 
	UGraphNodeDialogueBase();

public:
	virtual void AutowireNewNode(UEdGraphPin* FromPin);
	
	virtual EDialogueConnectionLimit GetInputConnectionLimit() const;
	virtual EDialogueConnectionLimit GetOutputConnectionLimit() const;
	
	virtual TArray<UEdGraphPin*> GetInputPins() const;
	virtual TArray<UEdGraphPin*> GetOutputPins() const;
	
	virtual const TArray<UGraphNodeDialogueBase*> GetDirectParents() const;
	virtual const TArray<UGraphNodeDialogueBase*> GetDirectChildren() const;
	
	virtual FText GetPinMenuLabel(int32 PinIndex) const;
	virtual FText GetContextMenuName() const;

private:
	const TArray<UGraphNodeDialogueBase*> GetPinConnectedNodes(UEdGraphPin* SourcePin) const;
	TArray<UEdGraphPin*> GetPinsWithDirection(EEdGraphPinDirection Direction) const;
};
