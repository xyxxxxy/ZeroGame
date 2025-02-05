
#include "Graph/Slate/SGraphNodeDialogueSpeech.h"

#include "DialogueEditorStyle.h"
#include "Graph/Nodes/GraphNodeDialogueSpeech.h"
#include "DialogueSpeakerSocket.h"
#include "Transitions/InputDialogueTransition.h"


#define LOCTEXT_NAMESPACE "SGraphNodeDialogueSpeech"

void SGraphNodeDialogueSpeech::Construct(const FArguments &InArgs, UEdGraphNode *InNode)
{
	SpeechNode = Cast<UGraphNodeDialogueSpeech>(InNode);
	check(SpeechNode);

	SGraphNodeDialogueBase::Construct(SGraphNodeDialogueBase::FArguments(), InNode);
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::CreateNodeContentArea()
{
	check(SpeechNode);

	FText SpeechText = SpeechNode->GetSpeechText();
	if (SpeechText.IsEmptyOrWhitespace())
	{
		return SGraphNodeDialogueBase::CreateNodeContentArea();
	}
	else
	{
		return SNew(SOverlay) 
			+ SOverlay::Slot()
			.Padding(10.f, 7.f, 10.f, 7.f)
			[
				SNew(STextBlock)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", BASE_FONT_SIZE))
				.Justification(TEXT_JUSTIFY)
				.Text(SpeechText)
				.WrapTextAt(WRAP_TEXT_AT)
			]
			+ SOverlay::Slot()
			[
				SGraphNodeDialogueBase::CreateNodeContentArea()
			];
	}
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::CreateNodeTitleWidget()
{
	//Get graph
	check(SpeechNode);

	//Assemble widget 
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(TITLE_BOX_PADDING)
		.FillWidth(1.f)
		[
			GetTitleTextBox()
		]
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(ICON_PADDING)
		[
			SNew(SImage)
			.Image(GetTransitionIcon())
		];
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::GetTitleTextBox() const
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		[
			GetSpeakerNameWidget()
		]
		+ SVerticalBox::Slot()
		.Padding(SUBTITLE_PADDING)
		.HAlign(HAlign_Left)
		.AutoHeight()
		[
			GetNodeSubtitleWidget()
		];
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::GetSpeakerNameWidget() const
{
	return SNew(STextBlock)
		.Justification(TEXT_JUSTIFY)
		.TextStyle(&FDialogueEditorStyle::GetTextStyle(TEXT("NodeTitleStyle")))
		.Text(GetSpeakerNameText());
}

FText SGraphNodeDialogueSpeech::GetSpeakerNameText() const
{
	check(SpeechNode);
	UDialogueSpeakerSocket *SpeakerSocket = SpeechNode->GetSpeakerSocket();

	FText SpeakerNameText = LOCTEXT("ErrorNameText", "Error: Speaker Removed");

	if (SpeakerSocket && !SpeakerSocket->GetSpeakerName().IsNone())
	{
		SpeakerNameText = FText::FromName(SpeakerSocket->GetSpeakerName());
	}

	return SpeakerNameText;
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::GetNodeSubtitleWidget() const
{
	return SNew(STextBlock)
		.Justification(TEXT_JUSTIFY)
		.Text(GetNodeSubtitleText())
		.WrapTextAt(WRAP_TEXT_AT - SUBTITLE_WRAP_DIFFERENCE);
}

FText SGraphNodeDialogueSpeech::GetNodeSubtitleText() const
{
	check(SpeechNode);
	return FText::FromName(SpeechNode->GetID());
}

const FSlateBrush *SGraphNodeDialogueSpeech::GetTransitionIcon() const
{
	check(SpeechNode);
	UClass* TransitionType = SpeechNode->GetTransitionType();
	if (TransitionType == UInputDialogueTransition::StaticClass())
	{
		return FDialogueEditorStyle::GetBrush("InputTransitionIcon");
	}
	else
	{
		return FDialogueEditorStyle::GetBrush("AutoTransitionIcon");
	}	
}

#undef LOCTEXT_NAMESPACE
