
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueManagerSubsystem.generated.h"

class UDialogueSettings;
class ADialogueController;

UCLASS()
class DIALOGUERUNTIME_API UDialogueManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintPure,Category = "Dialogue")
	ADialogueController* GetCurrentController() const;

	UFUNCTION(BlueprintPure,Category = "Dialogue")
	const UDialogueSettings* GetSettings();

private:
	const FString DefaultControllerTypeName = "BP_BasicDialogueController_C";
	TObjectPtr<ADialogueController> DialogueController;
	
};
