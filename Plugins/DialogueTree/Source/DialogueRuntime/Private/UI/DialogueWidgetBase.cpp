
#include "UI/DialogueWidgetBase.h"
#include "Input/CommonUIInputTypes.h"

UDialogueWidgetBase::UDialogueWidgetBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UDialogueWidgetBase::GetDesiredInputConfig() const
{
	FUIInputConfig ConfigOverride;

	switch (InputMode)
	{
	case EDialogueWidgetInputMode::Game:
		ConfigOverride = FUIInputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
		break;
	case EDialogueWidgetInputMode::GameAndMenu:
		ConfigOverride = FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::CaptureDuringMouseDown);
		break;
	case EDialogueWidgetInputMode::Menu:
		ConfigOverride = FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		break;
	case EDialogueWidgetInputMode::Default:
	default:
		// By default, no input change is desired, return an empty config
		return TOptional<FUIInputConfig>();
	}

	return ConfigOverride;	
}

void UDialogueWidgetBase::SetController_Implementation(ADialogueController* InController)
{
    DialogueController = InController;
}
void UDialogueWidgetBase::SetDetails_Implementation(const TArray<FSpeechDetails>& InDetails){}

void UDialogueWidgetBase::InitialOption_Implementation(const FSpeechDetails& InDetails){}