
#pragma once

#include "DialogueWidgetBase.h"
#include "SpeechDetails.h"
#include "OptionsWidget.generated.h"

UCLASS()
class UOptionsWidget : public UDialogueWidgetBase
{
    GENERATED_BODY()
public:
    UOptionsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
    virtual void NativeOnActivated() override;
    virtual void NativeOnDeactivated() override;

    virtual void SetDetails_Implementation(const TArray<FSpeechDetails>& InDetails) override;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
    TArray<FSpeechDetails> Details;

};