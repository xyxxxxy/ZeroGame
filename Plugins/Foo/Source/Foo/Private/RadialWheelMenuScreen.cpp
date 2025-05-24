
#include "RadialWheelMenuScreen.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "RadialWheelMenu.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "RadialItemInterface.h"
void URadialWheelMenuScreen::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    RadialWheelMenu->OnIndexSelected.AddDynamic(this, &ThisClass::OnSelectChangedNative);
    SetIsFocusable(true);
}

void URadialWheelMenuScreen::NativeConstruct()
{
    Super::NativeConstruct();

    if (APlayerController *PC = GetOwningPlayer())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

        InputMode.SetWidgetToFocus(TakeWidget());

        PC->SetInputMode(InputMode);

        PC->FlushPressedKeys();
        PC->SetShowMouseCursor(false);
    }
    // UWidgetBlueprintLibrary::SetInputMode_GameAndUI();
}

FReply URadialWheelMenuScreen::NativeOnAnalogValueChanged(const FGeometry &InGeometry, const FAnalogInputEvent &InAnalogEvent)
{
    FKey Key = InAnalogEvent.GetKey();
    float Value = InAnalogEvent.GetAnalogValue();

    if (Key == EKeys::Gamepad_RightX)
    {
        PositionX = Value;
        SetIndexByAnalogValue();
    }
    else if (Key == EKeys::Gamepad_RightY)
    {
        PositionY = Value;
        SetIndexByAnalogValue();
    }

    return FReply::Handled();
}

FReply URadialWheelMenuScreen::NativeOnMouseMove(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    // if(IsPlayerUsingGamePad())return FReply::Handled();
    // FVector2D MousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
    // FVector2D RadialCenterPosition = RadialWheelMenu.Get()->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f));
    // FVector2D RelativePos = MousePosition - RadialCenterPosition;
    // float InnerRadius = RadialWheelMenu.Get()->GetCachedGeometry().Size.X * RadialWheelMenu->GetInnerRadiusCoefficient();
    // float OuterRadius = RadialWheelMenu->GetWidthOverride() / 2;
    // float DistanceSquared = RelativePos.SizeSquared();
    // if(DistanceSquared > OuterRadius * OuterRadius || DistanceSquared < InnerRadius * InnerRadius)
    // {
    //     GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("NativeOnMouseMove->SetIndex: 0")));
    //     RadialWheelMenu->SetIndex(0);
    //     return FReply::Handled();
    // }

    // float Degrees = FMath::Fmod((UKismetMathLibrary::DegAtan2(-RelativePos.Y, RelativePos.X) + 360.0f), 360.0f);
    // //GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, FString::Printf(TEXT("Degrees: %f"), Degrees));
    // float SectorAngle = 360.f / RadialWheelMenu->GetCount();
    // int32 Index = FMath::CeilToInt32(Degrees / SectorAngle);
    // RadialWheelMenu->SetIndex(Index);
    return FReply::Handled();
}

FReply URadialWheelMenuScreen::NativeOnMouseWheel(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent)
{
    int32 CurrentIndex = RadialWheelMenu->GetIndex();

    if (CurrentIndex == 0)
    {
        RadialWheelMenu->SetIndex(1);
        return FReply::Handled();
    }

    if (InMouseEvent.GetWheelDelta() > 0.0f)
    {
        if (CurrentIndex >= RadialWheelMenu->GetCount())
        {
            RadialWheelMenu->SetIndex(1);
        }
        else
        {
            RadialWheelMenu->SetIndex(CurrentIndex + 1);
        }
    }
    else
    {
        if (CurrentIndex == 1)
        {
            RadialWheelMenu->SetIndex(RadialWheelMenu->GetCount());
        }
        else
        {
            RadialWheelMenu->SetIndex(CurrentIndex - 1);
        }
    }
    return FReply::Handled();
}

bool URadialWheelMenuScreen::IsPlayerUsingGamePad()
{
    APlayerController *PC = GetOwningPlayer();
    if (!PC)
        return false;

    if (UInputDeviceSubsystem *InputDeviceSubsystem = GEngine->GetEngineSubsystem<UInputDeviceSubsystem>())
    {
        const FPlatformUserId UserId = PC->GetPlatformUserId();
        FHardwareDeviceIdentifier DeviceIdentifier = InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(UserId);
        return (DeviceIdentifier.PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad);
    }
    return false;
}

void URadialWheelMenuScreen::SetIndexByAnalogValue()
{
    if (FMath::Sqrt(PositionX * PositionX + PositionY * PositionY) >= DeadZone)
    {
        float Degrees = FMath::Fmod((UKismetMathLibrary::DegAtan2(PositionY, PositionX) + 360.0f), 360.f);
        float SectorAngle = 360.f / RadialWheelMenu->GetCount();
        int32 Index = FMath::CeilToInt32(Degrees / SectorAngle);
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("NativeOnAnalogValueChanged->SetIndex: %d"), Index));

        int32 PreviouIndex = RadialWheelMenu->GetIndex();
        RadialWheelMenu->SetIndex(Index);
        if(PreviouIndex != RadialWheelMenu->GetIndex())
        {
            RadialWheelMenu->OnIndexSelected.Broadcast(PreviouIndex, Index);
        }
    }
    // else
    // {
    //     int32 PreviouIndex = RadialWheelMenu->GetIndex();
    //     const int32 DefaultIndex = 0;
    //     RadialWheelMenu->SetIndex(DefaultIndex);
    //     if(PreviouIndex != RadialWheelMenu->GetIndex())
    //     {
    //         GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("NativeOnAnalogValueChanged->SetIndex: %d"), DefaultIndex));
    //         RadialWheelMenu->OnIndexSelected.Broadcast(PreviouIndex, DefaultIndex);
    //     }
    // }
}

void URadialWheelMenuScreen::OnSelectChangedNative(int32 PreviousIndex, int32 CurrentIndex)
{
    if(UUserWidget* PreviouItem = RadialWheelMenu->ListItems[PreviousIndex])
    {
        if(PreviouItem->Implements<URadialItemInterface>())
        {
            IRadialItemInterface::Execute_OnDeselected(PreviouItem);
        }
    }

    if(UUserWidget* CurrentItem = RadialWheelMenu->ListItems[CurrentIndex])
    {
        if(CurrentItem->Implements<URadialItemInterface>())
        {
            IRadialItemInterface::Execute_OnSelected(CurrentItem);
        }
    }
    OnSelectChanged(PreviousIndex, CurrentIndex);
}