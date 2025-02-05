
#pragma once

#include "CoreMinimal.h"
#include "SpeechDetails.h"
#include "DialogueOption.h"
#include "Dialogue.generated.h"

class ADialogueController;
class UDialogueSpeakerComponent;
class UDialogueSpeakerSocket;
class UDialogueNode;

DECLARE_DELEGATE(FSpeakerRolesChangedSignature);

UENUM()
enum class EDialogueCompileStatus
{
	Compiled,
	UnCompiled,
	Failed
};

USTRUCT()
struct DIALOGUERUNTIME_API FSpeakerField
{
	GENERATED_BODY()
public:
	UPROPERTY(NoClear,meta = (NoResetToDefault))
	UDialogueSpeakerSocket* SpeakerSocket = nullptr;

	UPROPERTY(EditAnywhere,Category = "Dialogue",meta = (NoResetToDefault))
	FColor GraphColor = FColor::White;
};

USTRUCT()
struct DIALOGUERUNTIME_API FDefaultDialogueColors
{
	GENERATED_BODY()

public:
	FDefaultDialogueColors() {};

	FDefaultDialogueColors(const TArray<FColor>& InColors)
	: DefaultColors(InColors)
	{};

public:
	FColor PopColor();

private:
	UPROPERTY()
	TArray<FColor> DefaultColors
	{
		FColor(34, 139, 34), //Forest green
		FColor(65.f, 105.f, 225.f), //Royal blue
		FColor(178.f, 34.f, 34.f), //Firebrick Red
		FColor(255, 255, 0), //Yellow
		FColor(255, 165, 0), //Orange
		FColor(0, 255, 255), //Cyan
		FColor(199, 21, 133), //Violet
		FColor(147, 112, 219), //Purple
		FColor(255, 192, 203), //Pink
		FColor(152, 251, 152) //Pale green
	};
	
	UPROPERTY()
	uint32 ColorIndex = 0;
};

UCLASS(BlueprintType)
class DIALOGUERUNTIME_API UDialogue : public UObject
{
	GENERATED_BODY()
public:
	UDialogue();

public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetSpeaker(FName InName, UDialogueSpeakerComponent* InSpeaker);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	UDialogueSpeakerComponent* GetSpeaker(FName InName) const;
	
	void AddSpeakerEntry(FName InName);
	//
	void OpenDialogue(ADialogueController* InController, TMap<FName, UDialogueSpeakerComponent*> InSpeakers);
	void ClearController();
	//
	void DisplaySpeech(const FSpeechDetails& InDetails);
	void DisplayOptions(TArray<FDialogueOption> InOptions) const;
	//
	void SelectOption(int32 InOptionIndex) const;
	void Skip() const;
	void EndDialogue();
	
	void TraverseNode(UDialogueNode* InNode);
	
	EDialogueCompileStatus GetCompileStatus() const;
	
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	TMap<FName, UDialogueSpeakerComponent*> GetAllSpeakers() const;
	
	UFUNCTION(BlueprintCallable, Category="Dialogue")
	bool SpeakerIsPresent(const FName SpeakerName) const;
	
	bool WasNodeVisited(UDialogueNode* TargetNode) const;
	void MarkNodeVisited(UDialogueNode* TargetNode, bool bToBeVisited);
	void ClearAllNodeVisits();

#if WITH_EDITOR
	void AddNode(UDialogueNode* InNode);
	void RemoveNode(UDialogueNode* InNode);
	void SetRootNode(UDialogueNode* InNode);
	UDialogueNode* GetRootNode() const;

	void SetGraph(UEdGraph* InGraph);
	UEdGraph* GetGraph();

	const TMap<FName, FSpeakerField>& GetSpeakerRoles() const;
	void ClearDialogue();
	void PreCompileDialogue();
	void PostCompileDialogue();
	void SetCompileStatus(EDialogueCompileStatus InStatus);
#endif

private:
	/* set SpeakerSocket when setting SpeakerRoles */
	void AddDefaultSpeakers();
	void OnChangeSpeakers(const EPropertyChangeType::Type& ChangeType);
	void OnAddSpeaker();
	void OnRemoveSpeaker();
	void OnChangeSingleSpeaker();
	/* set SpeakerSocket when setting SpeakerRoles */
	
	bool CanPlay(ADialogueController* InController, FString& OutErrorMessage) const;
	void FillSpeakers(TMap<FName, UDialogueSpeakerComponent*> InSpeakers);

public:
	UDialogueNode* GetActiveNode() const;
	ADialogueController* GetDialogueController() const { return DialogueController; }

private:
	UPROPERTY(EditAnywhere, NoClear, Category = "Dialogue", meta=(NoResetToDefault))
	TMap<FName, FSpeakerField> SpeakerRoles;
	
	UPROPERTY()
	TArray<TObjectPtr<UDialogueNode>> DialogueNodes;

	UPROPERTY()
	TObjectPtr<UDialogueNode> ActiveNode;

	UPROPERTY()
	TObjectPtr<UDialogueNode> RootNode;
	
	UPROPERTY()
	TMap<FName, UDialogueSpeakerComponent*> Speakers;
	
	UPROPERTY()
	TObjectPtr<ADialogueController> DialogueController;
	
	UPROPERTY()
	EDialogueCompileStatus CompileStatus = EDialogueCompileStatus::UnCompiled;
	
	UPROPERTY()
	FDefaultDialogueColors DefaultSpeakerColors;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UEdGraph> Graph;
#endif

public:
	FSpeakerRolesChangedSignature OnSpeakerRolesChanged;
};