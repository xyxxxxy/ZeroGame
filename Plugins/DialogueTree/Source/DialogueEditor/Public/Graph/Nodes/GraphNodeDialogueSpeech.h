
#pragma once

#include "GraphNodeDialogue.h"
#include "GameplayTagContainer.h"
#include "GraphNodeDialogueSpeech.generated.h"

class UDialogueTransition;
class UDialogueSpeakerSocket;

USTRUCT()
struct DIALOGUEEDITOR_API FPickableDialogueSpeaker
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	UDialogueSpeakerSocket* SpeakerSocket;
};

UCLASS()
class DIALOGUEEDITOR_API UGraphNodeDialogueSpeech : public UGraphNodeDialogue
{
	GENERATED_BODY()
public:
	static UGraphNodeDialogueSpeech* MakeTemplate(UObject* Outer,UDialogueSpeakerSocket* InSpeaker, TSubclassOf<UDialogueTransition> TransitionType);

public: 
	/** UObject Impl. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	/** End UObject */

	/** UEdGraphNode Implementation */
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FName GetBaseID() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	/** End UEdGraphNode */

	/** UGraphNodeDialogueBase Implementation */
	virtual FText GetContextMenuName() const override;
	virtual EDialogueConnectionLimit GetOutputConnectionLimit() const override;
	/** End UGraphNodeDialogueBase */

	/** UGraphNodeDialogue Implementation */
	virtual void CreateAssetNode(class UDialogue* InAsset) override;
	virtual bool CanCompileNode() override;
	/** End UGraphNodeDialogue */

public:
	FText GetSpeechText() const;
	
	UClass* GetTransitionType() const;
	void SetTransitionType(TSubclassOf<UDialogueTransition> InType);
	
	void SetSpeakerSocket(UDialogueSpeakerSocket* InSpeaker);
	UDialogueSpeakerSocket* GetSpeakerSocket() const;

private:
	void OnChangeSpeechTitle();
	
	void OnChangeTransitionType();

private:
	/** The speaker associated with this speech */
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	FPickableDialogueSpeaker Speaker;
	
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	FName SpeechTitle;
	
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	bool bIgnoreContent = false;
	
	UPROPERTY(EditAnywhere, Category = "SpeechContent", meta = (MultiLine = true))
	FText SpeechText;
	
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	USoundBase* SpeechAudio;
	
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	float MinimumPlayTime = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "SpeechContent")
	bool bCanSkip = true;
	
	UPROPERTY(EditAnywhere, Category = "BehaviorFlags")
	FGameplayTagContainer BehaviorFlags;
	
	UPROPERTY(EditAnywhere, NoClear, Category = "Transition")
	TSubclassOf<UDialogueTransition> TransitionType;
};
