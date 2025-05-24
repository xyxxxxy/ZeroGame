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

    UFUNCTION(BlueprintImplementableEvent, Category = "RadialWheelMenu")
    void OnSelectChanged(int32 PreviousIndex, int32 CurrentIndex);
    void OnSelectChangedNative(int32 PreviousIndex, int32 CurrentIndex);

private:
    void SetIndexByAnalogValue();

public:
    UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"), Category = "RadialWheelMenu")
    float DeadZone = 0.5f;

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
    TObjectPtr<URadialWheelMenu> RadialWheelMenu;

    bool IsPlayerUsingGamePad();

private:
    float PositionX = 0.f;
    float PositionY = 0.f;
};