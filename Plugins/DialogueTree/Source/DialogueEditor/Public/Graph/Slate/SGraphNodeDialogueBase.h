
#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UGraphNodeDialogue;
class UEdGraphNode;

class DIALOGUEEDITOR_API SGraphNodeDialogueBase : public SGraphNode
{
public:
	SGraphNodeDialogueBase();
	
	SLATE_BEGIN_ARGS(SGraphNodeDialogueBase) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

public:
	virtual void UpdateGraphNode() override;
	virtual void SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel) override;

protected:
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;

	// Node Style
	virtual TSharedRef<SWidget> CreateNodeTitleWidget();
	virtual TSharedRef<SWidget> CreateHeaderWidget();
	virtual TSharedRef<SWidget> CreateErrorWidget();
	
	virtual TSharedRef<SWidget> CreateInputPinArea();
	virtual TSharedRef<SWidget> CreateOutputPinArea();
	
	virtual float GetInputPinYPadding() const;
	virtual float GetOutputPinYPadding() const;

private:
	TSharedRef<SWidget> AssembleNodeContent();

protected: 
	TObjectPtr<UGraphNodeDialogue> DialogueNode;
	
	TSharedPtr<SHorizontalBox> InputPinBox;
	TSharedPtr<SHorizontalBox> OutputPinBox;

private:
	const FVector2D DEFAULT_NODE_SIZE = FVector2D(75.f, 35.f);
	const FVector2D PIN_BOX_PADDING = FVector2D(25.f, 0.f);
	const FMargin TITLE_PADDING = FMargin(5.f, 5.f, 5.f, 2.5f);
	const float BASE_PIN_PUSH_AMOUNT = -40.f;

protected:
	const ETextJustify::Type TEXT_JUSTIFY = ETextJustify::Left;
	const int32 BASE_FONT_SIZE = 10;
	const float WRAP_TEXT_AT = 250.f;
};