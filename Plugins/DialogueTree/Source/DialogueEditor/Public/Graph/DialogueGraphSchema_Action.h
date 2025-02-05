
#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "DialogueGraphSchema_Action.generated.h"

class UGraphNodeDialogue;

USTRUCT()
struct DIALOGUEEDITOR_API FDialogueSchemaAction_NewNode : public FEdGraphSchemaAction
{
	GENERATED_BODY()
public:
	FDialogueSchemaAction_NewNode();
	FDialogueSchemaAction_NewNode(FText InNodeCategory, FText InMenuDesc, FText InToolTip, UGraphNodeDialogue* InTemplateNode);
	
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

private:
	void SetTemplateNode(UGraphNodeDialogue* InNode);
	void PlaceNodeInGraph(UEdGraph* InParentGraph, const FVector2D InLocation, UEdGraphPin* FromPin);
	void SetNodeLocation(const FVector2D InLocation);
	
private:
	UPROPERTY()
	TObjectPtr<UGraphNodeDialogue> TemplateNode;
};



