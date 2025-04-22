
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
	
	// 获取输入引脚
	virtual TArray<UEdGraphPin*> GetInputPins() const;
	// 获取输出引脚
	virtual TArray<UEdGraphPin*> GetOutputPins() const;
	
	virtual const TArray<UGraphNodeDialogueBase*> GetDirectParents() const;
	virtual const TArray<UGraphNodeDialogueBase*> GetDirectChildren() const;
	
	virtual FText GetPinMenuLabel(int32 PinIndex) const;
	virtual FText GetContextMenuName() const;

private:
	//获取与指定引脚（SourcePin）连接的所有UGraphNodeDialogueBase节点
	const TArray<UGraphNodeDialogueBase*> GetPinConnectedNodes(UEdGraphPin* SourcePin) const;
	TArray<UEdGraphPin*> GetPinsWithDirection(EEdGraphPinDirection Direction) const;
};
