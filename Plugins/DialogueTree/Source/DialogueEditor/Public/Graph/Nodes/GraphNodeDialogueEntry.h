
#pragma once
#include "GraphNodeDialogue.h"

#include "GraphNodeDialogueEntry.generated.h"

UCLASS()
class UGraphNodeDialogueEntry : public UGraphNodeDialogue
{
	GENERATED_BODY()
public:
	/** UEdGraphNode Impl. */
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override;
	virtual bool CanDuplicateNode() const override;
	/** End UEdGraphNode */

	/** UGraphNodeDialogueBase Impl. */
	virtual FText GetContextMenuName() const override;
	/** End UGraphNodeDialogueBase */

	/** UGraphNodeDialogue Impl. */
	virtual void CreateAssetNode(UDialogue* InAsset) override;
	virtual FName GetBaseID() const override;
	/** End UGraphNodeDialogue */
};
