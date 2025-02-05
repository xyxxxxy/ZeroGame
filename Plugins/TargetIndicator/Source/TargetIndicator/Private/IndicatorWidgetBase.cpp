#include "IndicatorWidgetBase.h"
#include "IndicatorManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Widget.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IndicatorWidgetBase)

#define LOCTEXT_NAMESPACE "IndicatorWidgetBase"

UIndicatorWidgetBase::UIndicatorWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UIndicatorWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UIndicatorWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	check(TargetActor);

	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		IndicatorManagerSubsystem = GameInstance->GetSubsystem<UIndicatorManagerSubsystem>();
	}
	check(IndicatorManagerSubsystem);
	IndicatorTimerDelegate.BindUObject(this, &UIndicatorWidgetBase::UpdateIndicator);
	GetWorld()->GetTimerManager().SetTimer(IndicatorTimerHandle, IndicatorTimerDelegate, UpdateFrequency, true);
}

void UIndicatorWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
	IndicatorTimerDelegate.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(IndicatorTimerHandle);
}

void UIndicatorWidgetBase::UpdateIndicator()
{
	switch (ShowTypes)
	{
		case EIndicatorShowTypes::Always:
			UpdateDistance();
			CheckDistance();
			UpdatePosition();
			break;

		case EIndicatorShowTypes::Clip:
			UpdateDistance();
			CheckDistance();
			SetRelativePosition();
			SetPrimitivePosition();
			ClippingPosition = PrimitivePosition / UWidgetLayoutLibrary::GetViewportScale(GetWorld());
			UCanvasPanelSlot* IndicatorSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TargetIndicator);
			IndicatorSlot->SetPosition(ClippingPosition);
			break;
	}

}

void UIndicatorWidgetBase::UpdateDistance()
{
	float OriginDistance = UKismetMathLibrary::Vector_Distance(GetOwningPlayer()->GetPawn()->GetActorLocation(),
	                                                           TargetActor->GetActorLocation());
	Distance = FMath::FloorToInt(OriginDistance / 100.f);
	FText DistanceText = FText::AsNumber(Distance);
	DistanceTextBlock->SetText(FText::Format(LOCTEXT("Distance", "{0}m"), DistanceText));
}

void UIndicatorWidgetBase::CheckDistance()
{
	if(Distance < LimitedDistance)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
}

void UIndicatorWidgetBase::UpdatePosition()
{
	SetRelativePosition();
	SetPrimitivePosition();

	if (SetClippingPosition(PrimitivePosition, RelativePosition, DefaultWindowsSize, ClippingPosition))
	{
		// ClippingPosition
		ClippingPosition = ClippingPosition / UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		UCanvasPanelSlot* IndicatorSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TargetIndicator);
		IndicatorSlot->SetPosition(ClippingPosition);
	}
	else
	{
		// PrimitivePosition
		ClippingPosition = PrimitivePosition / UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		UCanvasPanelSlot* IndicatorSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(TargetIndicator);
		IndicatorSlot->SetPosition(ClippingPosition);
	}
}

void UIndicatorWidgetBase::SetRelativePosition()
{
	// ------------------------------------------- Get ViewportSize ---------------------------------
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(DefaultWindowsSize);
	}
	RelativePosition = DefaultWindowsSize / 2;
}

void UIndicatorWidgetBase::SetPrimitivePosition()
{
	const FVector TargetActorLocation = TargetActor->GetActorLocation();
	const FVector IndicatorLocation = bUseTargetoffset ? TargetActorLocation + TargetOffset : TargetActorLocation;
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), IndicatorLocation, ScreenPosition))
	{
		PrimitivePosition = ScreenPosition;
		return;
	}

	const FVector CameraLocation = GetOwningPlayer()->PlayerCameraManager->GetCameraLocation();
	const FVector CameraToTargetLocation = TargetActorLocation - CameraLocation;

	// Get camera XoY values for mapping
	FVector CameraX = UKismetMathLibrary::ProjectVectorOnToVector(CameraToTargetLocation,
	                                                              TargetActor->GetActorRightVector());
	FVector CameraY = UKismetMathLibrary::ProjectVectorOnToVector(CameraToTargetLocation,
	                                                              TargetActor->GetActorUpVector());

	float X = CameraX.Length();
	float Y = CameraY.Length();

	const FRotator CameraToTargetRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetActorLocation);
	FRotator DeltaRotation = GetOwningPlayer()->PlayerCameraManager->GetCameraRotation() - CameraToTargetRotation;

	float RotatorY = DeltaRotation.Pitch;
	float RotatorZ = DeltaRotation.Yaw;

	// According to RotatorY and RotatorZ, the screen is divided into 4 areas: upper left, lower left, upper right, and lower right.
	// The origin of the camera coordinates is in the center of the viewport, with the horizontal axis being X and the vertical axis being Y.
	if (RotatorY >= 0.0f)
	{
		// upper left
		if (RotatorZ >= 0.0f)
		{
			X *= -1;
			Y *= -1;
		}
		// upper right
		else
		{
			Y *= -1;
		}
	}
	else
	{
		// lower left
		if (RotatorZ >= 0.0f)
		{
			X *= -1;
		}
		// lower right
		else
		{
		}
	}
	// Magnify
	FVector2D Position(X, Y);
	Position.Normalize();
	Position *= 10000;
	PrimitivePosition = RelativePosition + Position;
}

bool UIndicatorWidgetBase::SetClippingPosition(const FVector2D& Target, const FVector2D& Relative, const FVector2D& WindowsSize, FVector2D& Clip)
{
	float TargetX = Target.X;
	float TargetY = Target.Y;
	float RelativeX = Relative.X;
	float RelativeY = Relative.Y;
	float WindowsSizeX = WindowsSize.X;
	float WindowsSizeY = WindowsSize.Y;

	int32 Area = 0;

	if (TargetX <= 0.0f)
	{
		Area |= 4;

		// WN 5
		if (TargetY <= 0.0f)
		{
			Area |= 1;
		}
		// WS 6
		else if (TargetY >= WindowsSizeY)
		{
			Area |= 2;
		}
		// W 4
		else
		{
		}
	}
	else if (TargetX >= WindowsSizeX)
	{
		Area |= 8;

		// EN 9
		if (TargetY <= 0.0f)
		{
			Area |= 1;
		}
		// ES 10
		else if (TargetY >= WindowsSizeY)
		{
			Area |= 2;
		}
		// E 8
		else
		{
		}
	}
	else
	{
		// N 1
		if (TargetY <= 0.0f)
		{
			Area |= 1;
		}
		// S 2
		else if (TargetY >= WindowsSizeY)
		{
			Area |= 2;
		}
		else
		{
		}
	}

	// No clip
	if (Area == 0)
	{
		Clip = FVector2D::ZeroVector;
		return false;
	}

	float X = 0.0f, Y = 0.0f;
	// W
	if (Area == 4)
	{
		X = 0.0f;
		Y = RelativeY + RelativeX * (TargetY - RelativeY) / (RelativeX - TargetX);
	}
	// E
	else if (Area == 8)
	{
		X = WindowsSizeX;
		Y = RelativeY + RelativeX * (TargetY - RelativeY) / (TargetX - RelativeX);
	}
	// N
	else if (Area == 1)
	{
		Y = 0.0f;
		X = RelativeX + RelativeY * (TargetX - RelativeX) / (RelativeY - TargetY);
	}
	// S
	else if (Area == 2)
	{
		Y = WindowsSizeY;
		X = RelativeX + RelativeY * (TargetX - RelativeX) / (TargetY - RelativeY);
	}
	// WN
	else if (Area == 5)
	{
		X = 0.0f;
		Y = RelativeY + RelativeX * (TargetY - RelativeY) / (RelativeX - TargetX);
		if (Y < 0.0f)
		{
			Y = 0.0f;
			// ???
			X = TargetX - TargetY * (RelativeX - TargetX) / (RelativeY - TargetY);
		}
	}
	// WS
	else if (Area == 6)
	{
		X = 0.0f;
		Y = RelativeY + RelativeX * (TargetY - RelativeY) / (RelativeX - TargetX);
		if (Y > WindowsSizeY)
		{
			Y = WindowsSizeY;
			X = RelativeX + RelativeY * (TargetX - RelativeX) / (TargetY - RelativeY);
		}
	}
	// EN
	else if (Area == 9)
	{
		X = WindowsSizeX;
		Y = RelativeY + RelativeX * (TargetY - RelativeY) / (TargetX - RelativeX);
		if (Y < 0.0f)
		{
			Y = 0.0f;
			// ???
			X = TargetX - TargetY * (RelativeX - TargetX) / (RelativeY - TargetY);
		}
	}
	// ES
	else if (Area == 10)
	{
		X = WindowsSizeX;
		Y = RelativeY + RelativeX * (TargetY - RelativeY) / (TargetX - RelativeX);
		if (Y > WindowsSizeY)
		{
			Y = WindowsSizeY;
			X = RelativeX + RelativeY * (TargetX - RelativeX) / (TargetY - RelativeY);
		}
	}
	if (X > WindowsSizeX || X < 0.0f || Y > WindowsSizeY || Y < 0.0f)
	{
		Clip = FVector2D::ZeroVector;
		return false;
	}
	else
	{
		Clip.X = X;
		Clip.Y = Y;
		return true;
	}
}

#undef LOCTEXT_NAMESPACE
