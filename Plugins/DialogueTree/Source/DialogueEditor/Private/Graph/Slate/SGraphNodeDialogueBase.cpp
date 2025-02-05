
#include "Graph/Slate/SGraphNodeDialogueBase.h"

#include "DialogueEditorStyle.h"
#include "SlateOptMacros.h"
#include "SGraphPanel.h"
#include "Graph/Nodes/GraphNodeDialogue.h"
#include "Graph/Slate/SDialoguePins.h"


#define LOCTEXT_NAMESPACE "SGraphNodeDialogueBase"

SGraphNodeDialogueBase::SGraphNodeDialogueBase()
{
}

void SGraphNodeDialogueBase::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	check(InNode);
	GraphNode = InNode;
	SetCursor(EMouseCursor::Default);
	
	//Cache the downcast dialogue node since we'll be using it a lot
	DialogueNode = Cast<UGraphNodeDialogue>(GraphNode);
	check(DialogueNode);
	
	//Subscribe订阅 to visual update events on parent node
	FOnUpdateNode UpdateDelegate;
	UpdateDelegate.BindSP(this, &SGraphNodeDialogueBase::UpdateGraphNode);
	DialogueNode->BindOnUpdateVisuals(UpdateDelegate);
	
	//Build the visuals
	UpdateGraphNode();
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGraphNodeDialogueBase::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	InputPinBox.Reset(); 
	OutputPinBox.Reset();

	//Base Layout
	TSharedRef<SOverlay> NodeLayout = SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SImage)
			.Image(GetNodeBodyBrush())
			.ColorAndOpacity(FColor::Black)
		]
		+ SOverlay::Slot()
		.Padding(0.f, GetInputPinYPadding(), 0.f, 0.f)
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		[
			CreateInputPinArea()
		]
		+ SOverlay::Slot()
		[
			AssembleNodeContent()
		]
		+ SOverlay::Slot()
		.Padding(0.f, 0.f, 0.f, GetOutputPinYPadding())
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Center)
		[
			CreateOutputPinArea()
		];

	//Add node layout to node 
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			NodeLayout
		];

	//Create pin widgets
	CreatePinWidgets();

	if (DialogueNode)
	{
		//Prepass to update desired sizes
		SlatePrepass(GetPrepassLayoutScaleMultiplier());

		//Resize dialogue node
		DialogueNode->ResizeNode(GetDesiredSize());
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGraphNodeDialogueBase::SetOwner(const TSharedRef<SGraphPanel>& OwnerPanel)
{
	check(!OwnerGraphPanelPtr.IsValid());
	SetParentPanel(OwnerPanel);
	OwnerGraphPanelPtr = OwnerPanel;
	GraphNode->DEPRECATED_NodeWidget = SharedThis(this);

	//Recreate pins if necessary
	if (OwnerGraphPanelPtr.Pin()->GetPinVisibility() != SGraphEditor::Pin_Show && InputPinBox.IsValid() && OutputPinBox.IsValid())
	{
		this->InputPinBox->ClearChildren();
		this->OutputPinBox->ClearChildren();
		CreatePinWidgets();
	}
}

TSharedRef<SWidget> SGraphNodeDialogueBase::CreateNodeContentArea()
{
	return SNew(SSpacer).Size(DEFAULT_NODE_SIZE);
}

TSharedPtr<SGraphPin> SGraphNodeDialogueBase::CreatePinWidget(UEdGraphPin* Pin) const
{
	check(DialogueNode);

	if (Pin->Direction == EGPD_Input)
	{
		return SNew(SDialogueStandardPin, Pin);
	}

	TArray<UGraphNodeDialogue*> PinChildren;
	DialogueNode->GetPinChildren(Pin, PinChildren);

	if (PinChildren.Num() < 1)
	{
		return SNew(SDialogueExitPin, Pin);
	}

	return SNew(SDialogueStandardPin, Pin);
}

void SGraphNodeDialogueBase::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	
	if (PinToAdd->GetDirection() == EGPD_Input)
	{
		InputPinBox->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(PIN_BOX_PADDING)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Output pin
	{
		OutputPinBox->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(PIN_BOX_PADDING)
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}
}

TSharedRef<SWidget> SGraphNodeDialogueBase::CreateNodeTitleWidget()
{
	//Set the title text
	FText TitleText = GraphNode->GetNodeTitle(ENodeTitleType::FullTitle);


	TSharedRef<STextBlock> TitleTextBlock = SNew(STextBlock)
		.TextStyle(&FDialogueEditorStyle::GetTextStyle("NodeTitleStyle"))
		.Justification(TEXT_JUSTIFY)
		.Text(TitleText);

	return TitleTextBlock;
}

TSharedRef<SWidget> SGraphNodeDialogueBase::CreateHeaderWidget()
{
	return SNew(SOverlay)
	+ SOverlay::Slot()
	[
		SNew(SBorder)
		.BorderImage(FDialogueEditorStyle::GetBrush("NodeHeaderGloss"))
		.BorderBackgroundColor(GraphNode->GetNodeTitleColor())
	]
	+ SOverlay::Slot()
	[
		SNew(SBorder)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.BorderImage(FDialogueEditorStyle::GetBrush("NodeHeaderColorSpill"))
		.BorderBackgroundColor(GraphNode->GetNodeTitleColor())
	]
	+ SOverlay::Slot()
	.Padding(TITLE_PADDING)
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Center)
	[
		CreateNodeTitleWidget()
	];
}

TSharedRef<SWidget> SGraphNodeDialogueBase::CreateErrorWidget()
{
	return SNew(SHorizontalBox);
}

TSharedRef<SWidget> SGraphNodeDialogueBase::CreateInputPinArea()
{
	return SAssignNew(InputPinBox, SHorizontalBox);
}

TSharedRef<SWidget> SGraphNodeDialogueBase::CreateOutputPinArea()
{
	return SAssignNew(OutputPinBox, SHorizontalBox);
}

float SGraphNodeDialogueBase::GetInputPinYPadding() const
{
	return BASE_PIN_PUSH_AMOUNT;
}

float SGraphNodeDialogueBase::GetOutputPinYPadding() const
{
	return BASE_PIN_PUSH_AMOUNT;
}

TSharedRef<SWidget> SGraphNodeDialogueBase::AssembleNodeContent()
{
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		CreateHeaderWidget()
	]
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		CreateNodeContentArea()
	]
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		CreateErrorWidget()
	];
}

#undef LOCTEXT_NAMESPACE
