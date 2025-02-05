#include "LyraActivatableWidget.h"


ULyraActivatableWidget::ULyraActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


TOptional<FUIInputConfig> ULyraActivatableWidget::GetDesiredInputConfig() const
{
	FUIInputConfig ConfigOverride;

	switch (InputMode)
	{
	case ELyraWidgetInputMode::Game:
		ConfigOverride = FUIInputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
		break;
	case ELyraWidgetInputMode::GameAndMenu:
		ConfigOverride = FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::CaptureDuringMouseDown);
		break;
	case ELyraWidgetInputMode::Menu:
		ConfigOverride = FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
		break;
	case ELyraWidgetInputMode::Default:
	default:
		// By default, no input change is desired, return an empty config
		return TOptional<FUIInputConfig>();
	}

	return ConfigOverride;
}