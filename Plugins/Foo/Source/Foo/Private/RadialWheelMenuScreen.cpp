
#include "RadialWheelMenuScreen.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "RadialWheelMenu.h"

void URadialWheelMenuScreen::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    SetIsFocusable(true);
}

void URadialWheelMenuScreen::NativeConstruct()
{
    Super::NativeConstruct();

    if(APlayerController* PC = GetOwningPlayer())
    {
        FInputModeUIOnly InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);


		InputMode.SetWidgetToFocus(TakeWidget());
		
		PC->SetInputMode(InputMode);

		PC->FlushPressedKeys();
        PC->SetShowMouseCursor(true);
        
    }
    //UWidgetBlueprintLibrary::SetInputMode_GameAndUI();

}

FReply URadialWheelMenuScreen::NativeOnAnalogValueChanged( const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent )
{
    FKey Key = InAnalogEvent.GetKey();
    float Value = InAnalogEvent.GetAnalogValue();

    if(Key == EKeys::Gamepad_RightX)
    {
        PositionX = Value;

        if(FMath::Sqrt(PositionX * PositionX + PositionY * PositionY) >= 0.5f)
        {
            float Degrees = FMath::Fmod((UKismetMathLibrary::DegAtan2(PositionY, PositionX) + 360.0f), 360.f);
            float SectorAngle = 360.f / RadialWheelMenu->GetCount();
            int32 Index = FMath::CeilToInt32(Degrees / SectorAngle);
            RadialWheelMenu->SetIndex(Index);
        }
        else
        {
            RadialWheelMenu->SetIndex(0);
            FReply::Handled();
        }
        
        
        //float Radians = FMath::Atan2(PositionY, PositionX);
        
    }
    else if(Key == EKeys::Gamepad_RightY)
    {
        PositionY = Value;

        if(FMath::Sqrt(PositionX * PositionX + PositionY * PositionY) >= 0.5f)
        {
            float Degrees = FMath::Fmod((UKismetMathLibrary::DegAtan2(PositionY, PositionX) + 360.0f), 360.0f);
            float SectorAngle = 360.f / RadialWheelMenu->GetCount();
            int32 Index = FMath::CeilToInt32(Degrees / SectorAngle);
            RadialWheelMenu->SetIndex(Index);
        }
        else
        {
            RadialWheelMenu->SetIndex(0);
            FReply::Handled();
        }
    }
    return FReply::Handled();
}

FReply URadialWheelMenuScreen::NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    return FReply::Handled();
}

FReply URadialWheelMenuScreen::NativeOnMouseWheel( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    return FReply::Handled();
}