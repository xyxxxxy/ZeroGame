
#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundBase.h"
#include "GameplayTagContainer.h"

#include "SpeechDetails.generated.h"

USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FSpeechDetails
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FText SpeechText = FText();
	
	/** The name of the speaker */
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FName SpeakerName = NAME_None;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	USoundBase* SpeechAudio = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	float DeltaTime = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	float MinimumPlayTime = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FGameplayTagContainer BehaviorFlags;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bIgnoreContent = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	bool bCanSkip = true;
};