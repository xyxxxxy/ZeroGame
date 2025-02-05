
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dialogue.h"
#include "SpeechDetails.h"


#include "DialogueController.generated.h"

class UDialogue;
class UDialogueSpeakerComponent;
class UDialogueWidget;
class UCommonActivatableWidget;
class UDialogueTransition;

typedef const TMap<FName, UDialogueSpeakerComponent*>& Speakers;

// Can not use TMap  error : Unrecognized type - type must be a UCLASS, USTRUCT or UENUM
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBPOnAfterWidgetPushDelegate, const TMap<FName, UDialogueSpeakerComponent*>&, InSpeakers);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBPOnDialogueEndDelegate);


DECLARE_MULTICAST_DELEGATE(FOnBeforeWidgetPushDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAfterWidgetPushDelegate, Speakers);
DECLARE_MULTICAST_DELEGATE(FOnDialogueEndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnDisplayOptionsDelegate);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatementStartDelegate, const FSpeechDetails&);
DECLARE_MULTICAST_DELEGATE(FOnStatementEndDelegate);



USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FDialogueNodeVisits
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FName DialogueFName;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Dialogue")
	TSet<int32> VisitedNodeIndices;
};

USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FDialogueRecords
{
	GENERATED_BODY()
	//..
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Dialogue")
	TMap<FName, FDialogueNodeVisits> Records;
};

UCLASS(Abstract, notplaceable, BlueprintType, Blueprintable)
class DIALOGUERUNTIME_API ADialogueController : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogueController();

public:
	UDialogue* GetDialogue() const { return CurrentDialogue; }
	void TransitionOut();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual void SelectOption(int32 InOptionIndex) const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual TMap<FName, UDialogueSpeakerComponent*> GetSpeakers() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogueWithNames(UDialogue* InDialogue, const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UDialogue* InDialogue, TArray<UDialogueSpeakerComponent*> InSpeakers);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void Skip() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetSpeaker(FName InName, UDialogueSpeakerComponent* InSpeaker);	
	


	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ClearNodeVisits();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialogue")
	FDialogueRecords GetDialogueRecords() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ClearDialogueRecords();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ImportDialogueRecords(FDialogueRecords InRecords);
	
	bool SpeakerInCurrentDialogue(UDialogueSpeakerComponent* TargetSpeaker) const;
	
	void MarkNodeVisited(UDialogue* TargetDialogue, int32 TargetNodeIndex);
	void MarkNodeUnvisited(UDialogue* TargetDialogue, int32 TargetNodeIndex);
	void ClearAllNodeVisitsForDialogue(UDialogue* TargetDialogue);
	bool WasNodeVisited(const UDialogue* TargetDialogue, int32 TargetNodeIndex) const;

	

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	bool CanOpenWidget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	void OpenWidget(const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	void CloseWidget();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	void DisplaySpeech(const FSpeechDetails& InSpeechDetails, UDialogueSpeakerComponent* InSpeaker);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintNativeEvent, Category = "Dialogue")
	void DisplayOptions(const TArray<FSpeechDetails>& InOptions);
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleMissingSpeaker(const FName& MissingName);

	

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetWidget(UUserWidget* InWidget);

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	UDialogueWidget* GetWidget() const;

	UDialogueTransition* GetTransition();


	FOnBeforeWidgetPushDelegate& OnBeforeWidgetPush() const { return OnBeforeWidgetPushDelegate; }
	FOnAfterWidgetPushDelegate& OnAfterWidgetPush() const { return OnAfterWidgetPushDelegate; }
	FOnDialogueEndDelegate& OnDialogueEnd() const { return OnDialogueEndDelegate; }
	FOnStatementStartDelegate& OnStatementStart() const { return OnStatementStartDelegate; }
	FOnStatementEndDelegate& OnStatementEnd() const { return OnStatementEndDelegate; }
	FOnDisplayOptionsDelegate& OnDisplayOptions() const { return OnDisplayOptionsDelegate; };
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UDialogue> CurrentDialogue;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Dialogue")
	TWeakObjectPtr<UCommonActivatableWidget> DialogueWidgetInstance;


	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void BeforePush();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void AfterPush(const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue", meta = (DisplayName = "On Before Widget Push"))
	void BP_OnBeforeWidgetPush();
	virtual void NativeOnBeforeWidgetPush();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue", meta = (DisplayName = "On After Widget Push"))
	void BP_OnAfterWidgetPush(const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers);
	virtual void NativeOnAfterWidgetPush(const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers);

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue", meta = (DisplayName = "On After Widget Close"))
	void BP_OnAfterWidgetClose();
	virtual void NativeOnAfterWidgetClose();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue", meta = (DisplayName = "On Display Options"))
	void BP_OnDisplayOptions();
	virtual void NativeOnDisplayOptions();	

private:
	FDialogueRecords DialogueRecords;

	// mutable FSimpleMulticastDelegate OnAfterWidgetPushEvent;
	// mutable FSimpleMulticastDelegate OnAfterWidgetCloseEvent;
	// mutable FSimpleMulticastDelegate OnStatementStartEvent;
	// mutable FSimpleMulticastDelegate OnStatementEndEvent;
	

	//UPROPERTY(BlueprintAssignable, Category = Events, meta = (AllowPrivateAccess = true, DisplayName = "On Widget Activated"))

private:
	//mutable
	mutable FOnBeforeWidgetPushDelegate OnBeforeWidgetPushDelegate;
	mutable FOnAfterWidgetPushDelegate OnAfterWidgetPushDelegate;
	mutable FOnDialogueEndDelegate OnDialogueEndDelegate;

	mutable FOnDisplayOptionsDelegate OnDisplayOptionsDelegate;
	
	mutable FOnStatementStartDelegate OnStatementStartDelegate;
	mutable FOnStatementEndDelegate OnStatementEndDelegate;


	
};