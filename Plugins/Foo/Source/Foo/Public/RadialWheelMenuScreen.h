#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "RadialWheelMenuScreen.Generated.h"

class URadialWheelMenu;

UCLASS(Blueprintable, BlueprintType)
class URadialWheelMenuScreen : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

public:
    virtual FReply NativeOnAnalogValueChanged( const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent ) override;
    virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
    virtual FReply NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

private:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
    TObjectPtr<URadialWheelMenu> RadialWheelMenu;

private:
    float PositionX = 0.f;
    float PositionY = 0.f;
};