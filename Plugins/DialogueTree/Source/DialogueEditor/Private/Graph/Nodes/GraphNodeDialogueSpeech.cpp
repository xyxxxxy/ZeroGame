
#include "Graph/Nodes/GraphNodeDialogueSpeech.h"
#include "DialogueSpeakerSocket.h"
#include "SpeechDetails.h"
#include "Dialogue.h"
#include "Nodes/DialogueSpeechNode.h"
#include "Graph/DialogueEdGraph.h"
#include "Transitions/DialogueTransition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GraphNodeDialogueSpeech)

#define LOCTEXT_NAMESPACE "SpeechGraphNode"

UGraphNodeDialogueSpeech* UGraphNodeDialogueSpeech::MakeTemplate(UObject* Outer, UDialogueSpeakerSocket* InSpeaker,
	TSubclassOf<UDialogueTransition> TransitionType)
{
	check(InSpeaker && Outer && TransitionType);

	UGraphNodeDialogueSpeech* NewSpeech = NewObject<UGraphNodeDialogueSpeech>(Outer);
	NewSpeech->SetSpeakerSocket(InSpeaker);
	NewSpeech->SetTransitionType(TransitionType);

	return NewSpeech;
}

void UGraphNodeDialogueSpeech::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = PropertyChangedEvent.Property->GetFName();

	if (PropertyName.IsEqual("SpeechTitle"))
	{
		OnChangeSpeechTitle();
	}
	else if (PropertyName.IsEqual("TransitionType"))
	{
		OnChangeTransitionType();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UGraphNodeDialogueSpeech::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName());
	CreatePin(EGPD_Output, "MultipleNodes", FName());
}

FText UGraphNodeDialogueSpeech::GetTooltipText() const
{
	return LOCTEXT("Tooltip", "Represents a unit of dialogue to be performed by a Speaker");
}

FName UGraphNodeDialogueSpeech::GetBaseID() const
{
	if (SpeechTitle.IsNone())
	{
		return FName("SPEECH [NONE TITLE]");
	}
	FString Result = SpeechTitle.ToString();
	Result = FString(TEXT("[")) + Result + FString(TEXT("]"));
	return FName(Result);
}

FText UGraphNodeDialogueSpeech::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Speech"));
}

FLinearColor UGraphNodeDialogueSpeech::GetNodeTitleColor() const
{
	UDialogueEdGraph* Graph = GetDialogueGraph();
	if(!Graph || !Speaker.SpeakerSocket)
	{
		return FColor::White;
	}

	FName SpeakerName = Speaker.SpeakerSocket->GetSpeakerName();
	return Graph->GetSpeakerColor(SpeakerName);
}

FText UGraphNodeDialogueSpeech::GetContextMenuName() const
{
	return FText::FromString("Speech Node");
}

EDialogueConnectionLimit UGraphNodeDialogueSpeech::GetOutputConnectionLimit() const
{
	check(TransitionType);
	UObject* DefaultObj = TransitionType->GetDefaultObject();
	UDialogueTransition* DefaultTransition = Cast<UDialogueTransition>(DefaultObj);
	check(DefaultTransition);

	return DefaultTransition->GetConnectionLimit();
}

void UGraphNodeDialogueSpeech::CreateAssetNode(UDialogue* InAsset)
{
	check(Speaker.SpeakerSocket);
	check(TransitionType);

	//Create node
	UDialogueSpeechNode* NewNode = NewObject<UDialogueSpeechNode>(InAsset);
	SetAssetNode(NewNode);
    
	//Init data 
	FSpeechDetails SpeechDetails;
	SpeechDetails.SpeakerName = Speaker.SpeakerSocket->GetSpeakerName();
	SpeechDetails.bIgnoreContent = bIgnoreContent;

	SpeechDetails.SpeechText = SpeechText;

	SpeechDetails.SpeechAudio = SpeechAudio;
	SpeechDetails.MinimumPlayTime = MinimumPlayTime;
	SpeechDetails.bCanSkip = bCanSkip;
	SpeechDetails.BehaviorFlags = BehaviorFlags;

	NewNode->InitSpeechData(SpeechDetails, TransitionType);

	//Add node to the dialogue
	InAsset->AddNode(NewNode);
}

bool UGraphNodeDialogueSpeech::CanCompileNode()
{
	return Super::CanCompileNode();
}

FText UGraphNodeDialogueSpeech::GetSpeechText() const
{
	return SpeechText;
}

UClass* UGraphNodeDialogueSpeech::GetTransitionType() const
{
	return TransitionType;
}

void UGraphNodeDialogueSpeech::SetTransitionType(TSubclassOf<UDialogueTransition> InType)
{
	TransitionType = InType;
}

void UGraphNodeDialogueSpeech::SetSpeakerSocket(UDialogueSpeakerSocket* InSpeaker)
{
	Speaker.SpeakerSocket = InSpeaker;
}

UDialogueSpeakerSocket* UGraphNodeDialogueSpeech::GetSpeakerSocket() const
{
	if (Speaker.SpeakerSocket)
	{
		return Speaker.SpeakerSocket;
	}
    
	return nullptr;
}

void UGraphNodeDialogueSpeech::OnChangeSpeechTitle()
{
	ResetID();
}

void UGraphNodeDialogueSpeech::OnChangeTransitionType()
{
}

#undef LOCTEXT_NAMESPACE