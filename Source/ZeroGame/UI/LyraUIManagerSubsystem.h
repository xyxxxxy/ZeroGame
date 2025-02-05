#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "LyraUIManagerSubsystem.generated.h"


/**
 * Game UI Manager Subsystem
 * DefaultGame.ini
 * [/Script/EditorProject.LyraUIManagerSubsystem]
 * DefaultUIPolicyClass=/Game/UI/CommonGame/B_GameUIPolicy.B_GameUIPolicy_C
 */
UCLASS(meta = (DisplayName = "Lyra UI Manager Subsystem", Category= Lyra ))
class ULyraUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:
	ULyraUIManagerSubsystem();

	//~USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	//~End of USubsystem interface

private:
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();

	FTSTicker::FDelegateHandle TickHandle;
};