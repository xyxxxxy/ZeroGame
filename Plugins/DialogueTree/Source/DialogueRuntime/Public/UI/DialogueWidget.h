
#pragma once

#include "CoreMinimal.h"
#include "DialogueWidgetBase.h"
#include "SpeechDetails.h"
#include "DialogueController.h"
#include "DialogueInterface.h"
#include "DialogueWidget.generated.h"

class URichTextBlock;


UCLASS()
class UDialogueWidget : public UDialogueWidgetBase
{
	GENERATED_BODY()

public:
	UDialogueWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeOnInitialized() override;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

public:
	virtual void SetController_Implementation(ADialogueController* InController) override;

private:
	// TODO: Press 'X' for Auto Dialogue and Press 'A' to reach end of sentence.

	UFUNCTION()
	void StartNewStatement(const FSpeechDetails& InDetails);
	
	UFUNCTION()
	void PreSwitchToNextStatement();

	void OnSwitchToNextStatement();
	void DisplayEntireStatement();

	void DisplayDialogue();
	void FindFirstIndexAfterRich();

	void InitialValuables();

	void Forward();
	void Back();

protected:
	FTimerHandle DisplayTextTimerHandle;
	FTimerDelegate DisplayTextTimerDelegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget = true), Category = "Dialogue")
	URichTextBlock* DialogueTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText OriginText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float DisplayRate = 0.2f;

protected:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ForwardInputActionData;

	FUIActionBindingHandle ForwardHandle;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;

	FUIActionBindingHandle BackHandle;
	
private:
	int32 StrIndex = 0;
	FString CurrentStr;
	FString OriginStr;
	bool bHasEntered = false;
	bool bEntireStatement = false;
};