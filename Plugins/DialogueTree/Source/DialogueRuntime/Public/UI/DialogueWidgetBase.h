
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SpeechDetails.h"
#include "DialogueController.h"
#include "DialogueInterface.h"
#include "DialogueWidgetBase.generated.h"

UENUM(BlueprintType)
enum class EDialogueWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

UCLASS()
class UDialogueWidgetBase : public UCommonActivatableWidget, public IDialogueInterface
{
    GENERATED_BODY()

public:
    UDialogueWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

public:
    virtual void SetController_Implementation(ADialogueController* InController) override;
	virtual void SetDetails_Implementation(const TArray<FSpeechDetails>& InDetails) override;
	void InitialOption_Implementation(const FSpeechDetails& InDetails) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = Input)
	EDialogueWidgetInputMode InputMode = EDialogueWidgetInputMode::Default;
    
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<ADialogueController> DialogueController;
};