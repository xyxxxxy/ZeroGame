
#pragma once

#include "CoreMinimal.h"

#include "DialogueSpeakerSocket.generated.h"

class UDialogueSpeakerComponent;
class UDialogue;

UCLASS()
class DIALOGUERUNTIME_API UDialogueSpeakerSocket : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category = "Dialogue")
	void SetSpeakerName(FName InSpeakerName);
	
	UFUNCTION(BlueprintPure, Category= "Dialogue")
	FName GetSpeakerName() const;
	
	UDialogueSpeakerComponent* GetSpeakerComponent(UDialogue* InDialogue) const;
	bool IsValidSocket() const;
public:
	UPROPERTY(EditAnywhere,Category = "Dialogue")
	FName SpeakerName;
};
