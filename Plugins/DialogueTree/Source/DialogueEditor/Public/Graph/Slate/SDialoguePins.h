
#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"

class SDialoguePinBase : public SGraphPin
{
public:
	SDialoguePinBase();
	
	SLATE_BEGIN_ARGS(SDialoguePinBase){}
	SLATE_ARGUMENT(FSlateColor,PinColor)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:
	virtual TSharedRef<SWidget> GetDefaultValueWidget() override;
	virtual const FSlateBrush* GetPinIcon() const override;
	virtual void CachePinIcons();
	virtual FSlateColor GetPinColor() const override;

protected:
	const FSlateBrush* DialoguePinImage;
};

class SDialogueStandardPin : public SDialoguePinBase
{
public:
	SLATE_BEGIN_ARGS(SDialogueStandardPin) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected: 
	virtual void CachePinIcons() override;
};

class SDialogueExitPin : public SDialoguePinBase
{
public:
	// Slate Arguments 
	SLATE_BEGIN_ARGS(SDialogueStandardPin) {}
	SLATE_END_ARGS()

	/** Slate Constructor */
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

protected:
	/** SDialoguePinBase Implementation */
	virtual void CachePinIcons() override;
	/** End SDialoguePinBase */
};