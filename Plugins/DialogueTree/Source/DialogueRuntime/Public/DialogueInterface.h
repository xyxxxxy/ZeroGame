
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpeechDetails.h"
#include "DialogueInterface.generated.h"

class ADialogueController;
// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

class DIALOGUERUNTIME_API IDialogueInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue")
	void SetController(ADialogueController* InController);
	//void SetController_Implementation(ADialogueController* InController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue")
	void SetDetails(const TArray<FSpeechDetails>& InDetails);
	//void SetDetails_Implementation(const TArray<FSpeechDetails>& Details);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue")
	void InitialOption(const FSpeechDetails& InDetails);	
};