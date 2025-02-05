
#include "Graph/Slate/SDialoguePins.h"
#include "DialogueEditorStyle.h"

SDialoguePinBase::SDialoguePinBase() : DialoguePinImage(nullptr)
{
}

void SDialoguePinBase::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	// SideToSideMargin 边距
	SGraphPin::Construct(SGraphPin::FArguments().SideToSideMargin(0.0f), InPin);
	CachePinIcons();
}

TSharedRef<SWidget> SDialoguePinBase::GetDefaultValueWidget()
{
	return SNew(SSpacer);
}

const FSlateBrush* SDialoguePinBase::GetPinIcon() const
{
	return DialoguePinImage;
}

void SDialoguePinBase::CachePinIcons()
{
}

FSlateColor SDialoguePinBase::GetPinColor() const
{
	return SGraphPin::GetPinColor();
}


void SDialogueStandardPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SDialoguePinBase::Construct(SDialoguePinBase::FArguments(), InPin);
}

void SDialogueStandardPin::CachePinIcons()
{
	check(FDialogueEditorStyle::Get().IsValid());
	DialoguePinImage = FDialogueEditorStyle::GetBrush("StandardPin");
}

void SDialogueExitPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SDialoguePinBase::Construct(SDialoguePinBase::FArguments(), InPin);
}

void SDialogueExitPin::CachePinIcons()
{
	check(FDialogueEditorStyle::Get().IsValid());
	DialoguePinImage = FDialogueEditorStyle::GetBrush("ExitPin");
}
