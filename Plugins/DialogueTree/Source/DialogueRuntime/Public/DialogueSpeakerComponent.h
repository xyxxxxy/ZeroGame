
#pragma once

#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DialogueSpeakerComponent.generated.h"

class UDialogue;
class ADialogueController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnBehaviorFlagsChanged,
	FGameplayTagContainer,
	InFlags
);

/**
* Helper struct used to condense a speaker component and the actor which owns
* it into a single parameter value for easier access. 
*/
USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FSpeakerActorEntry
{
	GENERATED_BODY()

	/** The actor which owns the speaker component */
	UPROPERTY(BlueprintReadOnly, Category="Dialogue")
	AActor* Actor = nullptr;

	/** The speaker component under consideration */
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	class UDialogueSpeakerComponent* SpeakerComponent = nullptr;
};


UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DIALOGUERUNTIME_API UDialogueSpeakerComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	UDialogueSpeakerComponent();
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetDisplayName(FText InDisplayName);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetDialogueName(FName InDialogueName);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialogue")
	FText GetDisplayName() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialogue")
	FName GetDialogueName() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetOwnedDialogue(UDialogue* InDialogue);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialogue")
	UDialogue* GetOwnedDialogue();
	
	UFUNCTION(BlueprintPure, Category="Dialogue")
	FGameplayTagContainer GetBehaviorFlags();
	
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void EndCurrentDialogue();
	
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	void TrySkipSpeech();
	
	void SetBehaviorFlags(FGameplayTagContainer InTags);
	void ClearBehaviorFlags();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartOwnedDialogueWithNames(
		TMap<FName, UDialogueSpeakerComponent*> InSpeakers);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartOwnedDialogue(TArray<UDialogueSpeakerComponent*> InSpeakers);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogueWithNames(UDialogue* InDialogue, TMap<FName, UDialogueSpeakerComponent*> InSpeakers);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UDialogue* InDialogue, TArray<UDialogueSpeakerComponent*> InSpeakers);

public:
	FSpeakerActorEntry ToSpeakerActorEntry();

private:
	void BroadcastBehaviorFlags();

protected:
	/** The name to display for this speaker in dialogue */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText DisplayName;

	/** The "key" name used to match the speaker to its dialogue role */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FName DialogueName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<UDialogue> OwnedDialogue;
	
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = true))
	FGameplayTagContainer BehaviorFlags;

public:
	UPROPERTY(BlueprintReadOnly, Category="Dialogue")
	TObjectPtr<ADialogueController> DialogueController;
	
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnBehaviorFlagsChanged OnBehaviorFlagsChanged;
};