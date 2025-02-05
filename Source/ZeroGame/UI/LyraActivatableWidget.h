#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "LyraActivatableWidget.generated.h"


/**
 * Input Modes that LyraActivatableWidgets can activate
 */
UENUM(BlueprintType)
enum class ELyraWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};


/**
 * Activatable Widget
 *
 * An activatable widget that optionally overrides the input mode when activated
 */
UCLASS(Abstract, Blueprintable, meta=(Category=Lyra))
class ULyraActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	ULyraActivatableWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface

protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	ELyraWidgetInputMode InputMode = ELyraWidgetInputMode::Default;

};