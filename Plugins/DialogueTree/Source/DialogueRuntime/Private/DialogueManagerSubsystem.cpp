
#include "DialogueManagerSubsystem.h"

#include "DialogueController.h"
#include "DialogueRuntimeLogChannels.h"
#include "DialogueSettings.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueManagerSubsystem)

void UDialogueManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDialogueManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDialogueManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	TArray<AActor*> ExtantController;
	UGameplayStatics::GetAllActorsOfClass(&InWorld,ADialogueController::StaticClass(),ExtantController);
	for(auto& Controller : ExtantController)
	{
		UE_LOG(LogDialogueRuntime,Warning,TEXT("Removing existing dialogue controller from world. "
										 "Note that dialogue controllers manually placed in the level will not be used and can be safely deleted."));
		Controller->Destroy();
	}

	TSubclassOf<ADialogueController> ControllerType;
	
	if(UDialogueSettings* DialogueSettings = GetMutableDefault<UDialogueSettings>())
	{
		ControllerType = DialogueSettings->DialogueControllerType;
	}

	if(ControllerType)
	{
		DialogueController = InWorld.SpawnActor<ADialogueController>(ControllerType);
	}

	if(!DialogueController)
	{
		UE_LOG(LogDialogueRuntime,Warning,TEXT("Failed to spawn dialogue controller. "
										 "Please specify a controller type under ProjectSettings>>DialogueTree>>DialogueControllerType"));
	}
	
}

ADialogueController* UDialogueManagerSubsystem::GetCurrentController() const
{
	return DialogueController;
}

const UDialogueSettings* UDialogueManagerSubsystem::GetSettings()
{
	return GetDefault<UDialogueSettings>();
}
