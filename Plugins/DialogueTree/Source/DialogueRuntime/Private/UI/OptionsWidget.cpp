
#include "UI/OptionsWidget.h"

UOptionsWidget::UOptionsWidget(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UOptionsWidget::NativeOnActivated()
{
    Super::NativeOnActivated();
}

void UOptionsWidget::NativeOnDeactivated()
{
    Super::NativeOnDeactivated();
}

void UOptionsWidget::SetDetails_Implementation(const TArray<FSpeechDetails>& InDetails)
{
    Details = InDetails;
}