
#include "Graph/Nodes/GraphNodeDialogueBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GraphNodeDialogueBase)

#define LOCTEXT_NAMESPACE "GraphNodeDialogueBase"

UGraphNodeDialogueBase::UGraphNodeDialogueBase()
{
	bCanRenameNode = false;
}

void UGraphNodeDialogueBase::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);
}

EDialogueConnectionLimit UGraphNodeDialogueBase::GetInputConnectionLimit() const
{
	return EDialogueConnectionLimit::Unlimited;
}

EDialogueConnectionLimit UGraphNodeDialogueBase::GetOutputConnectionLimit() const
{
	return EDialogueConnectionLimit::Single;
}

TArray<UEdGraphPin*> UGraphNodeDialogueBase::GetInputPins() const
{
	return GetPinsWithDirection(EGPD_Input);
}

TArray<UEdGraphPin*> UGraphNodeDialogueBase::GetOutputPins() const
{
	return GetPinsWithDirection(EGPD_Output);
}

const TArray<UGraphNodeDialogueBase*> UGraphNodeDialogueBase::GetDirectParents() const
{
	TArray<UGraphNodeDialogueBase*> ParentNodes;

	for (UEdGraphPin* InputPin : GetInputPins())
	{
		ParentNodes.Append(GetPinConnectedNodes(InputPin));
	}

	return ParentNodes;
}

const TArray<UGraphNodeDialogueBase*> UGraphNodeDialogueBase::GetDirectChildren() const
{
	TArray<UGraphNodeDialogueBase*> ChildNodes;

	for (UEdGraphPin* OutputPin : GetOutputPins())
	{
		ChildNodes.Append(GetPinConnectedNodes(OutputPin));
	}

	return ChildNodes;
}

FText UGraphNodeDialogueBase::GetPinMenuLabel(int32 PinIndex) const
{
	check(Pins.IsValidIndex(PinIndex));
	UEdGraphPin* TargetPin = Pins[PinIndex];
	check(TargetPin);

	if (TargetPin->Direction == EGPD_Input)
	{
		return LOCTEXT("InputPinMenuLabel", "Input Pin");
	}
	else
	{
		return LOCTEXT("OutputPinMenuLabel", "Output Pin");
	}
}

FText UGraphNodeDialogueBase::GetContextMenuName() const
{
	return FText::GetEmpty();
}

const TArray<UGraphNodeDialogueBase*> UGraphNodeDialogueBase::GetPinConnectedNodes(UEdGraphPin* SourcePin) const
{
	TArray<UGraphNodeDialogueBase*> ConnectedNodes;
	TArray<UEdGraphPin*> ConnectedPins = SourcePin->LinkedTo;
	
	for (const UEdGraphPin* Pin : ConnectedPins)
	{
		if (UGraphNodeDialogueBase* Node = Cast<UGraphNodeDialogueBase>(Pin->GetOwningNode()))
		{
			ConnectedNodes.Add(Node);
		}
	}

	return ConnectedNodes;
}

TArray<UEdGraphPin*> UGraphNodeDialogueBase::GetPinsWithDirection(EEdGraphPinDirection Direction) const
{
	TArray<UEdGraphPin*> FoundPins;

	for (UEdGraphPin* CurrentPin : Pins)
	{
		if (CurrentPin->Direction == Direction)
		{
			FoundPins.Add(CurrentPin);
		}
	}

	return FoundPins;
}

#undef LOCTEXT_NAMESPACE