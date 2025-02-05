
#pragma once

#include "DialogueTransition.h"
#include "DialogueOption.h"
#include "InputDialogueTransition.generated.h"

UCLASS()
class DIALOGUERUNTIME_API UInputDialogueTransition : public UDialogueTransition
{
    GENERATED_BODY()
public:
	/** DialogueTransition Implementation */
	virtual void PreTransition() override;
	virtual void TransitionOut() override;
	virtual void SelectOption(int32 InOptionIndex);
	virtual FText GetDisplayName() const override;
	virtual FText GetNodeCreationTooltip() const override;
	virtual EDialogueConnectionLimit GetConnectionLimit() const override;
	virtual void TryTransitionOut() override;
	/** End DialogueTranstion */

private:
	/**
	* Displays the options for the user to select. 
	*/
	UFUNCTION()
	void ShowOptions();

	/**
	* Retrieves and caches the options for the transition. 
	*/
	UFUNCTION()
	void GetOptions();

private: 
	/** The available options for the player to choose */
	UPROPERTY()
	TArray<FDialogueOption> Options;  

	FDelegateHandle ShowOptionsHandle;  

};