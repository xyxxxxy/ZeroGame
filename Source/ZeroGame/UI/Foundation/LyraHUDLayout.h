#pragma once

#include "CoreMinimal.h"
#include "UI/LyraActivatableWidget.h"
#include "LyraHUDLayout.generated.h"


/**
 * HUD Layout
 */
UCLASS(Abstract, BlueprintType, Blueprintable, meta = (DisplayName = "Lyra HUD Layout", Category = Lyra))
class ULyraHUDLayout : public ULyraActivatableWidget
{
	GENERATED_BODY()

public:
	ULyraHUDLayout(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UUserWidget interface
	virtual void NativeOnInitialized() override;
	//~End of UUserWidget interface

protected:
	/** Callback called whenever the player gives the "Show Main Menu" input */
	void HandleMainMenuAction();
	void Forward();
	void Back();

	/** Class to use as the Main Menu for this HUD Layout */
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ULyraActivatableWidget> MainMenuClass;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnForward"), Category = "Lyra")
	void K2_Forward();

protected:
	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ForwardInputActionData;

	FUIActionBindingHandle ForwardHandle;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;

	FUIActionBindingHandle BackHandle;
};