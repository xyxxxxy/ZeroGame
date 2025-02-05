
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IndicatorWidgetBase.generated.h"

class UIndicatorManagerSubsystem;
class UTextBlock;
class UOverlay;

UENUM(BlueprintType)
enum class EIndicatorShowTypes : uint8 
{
    Always,
    Clip
};

UCLASS(Abstract)
class TARGETINDICATOR_API UIndicatorWidgetBase : public UUserWidget
{
    GENERATED_BODY()
public:
    UIndicatorWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:
    UPROPERTY(BlueprintReadOnly, Category = "Indicator")
    TObjectPtr<AActor> TargetActor;

protected:
    void UpdateIndicator();

    void UpdateDistance();
    void CheckDistance();

    void UpdatePosition();
    void SetRelativePosition();
    void SetPrimitivePosition();
    bool SetClippingPosition(const FVector2D& Target, const FVector2D& Relative, const FVector2D& WindowsSize, FVector2D& Clip);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget = true), Category = "Indicator")
    TObjectPtr<UOverlay> TargetIndicator;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget = true), Category = "Indicator")
    TObjectPtr<UTextBlock> DistanceTextBlock;

    UPROPERTY()
    TObjectPtr<UIndicatorManagerSubsystem> IndicatorManagerSubsystem;

    UPROPERTY(EditAnywhere, Category = "Indicator")
    EIndicatorShowTypes ShowTypes = EIndicatorShowTypes::Always;

    UPROPERTY(EditAnywhere, Category = "Indicator")
    float UpdateFrequency = 0.02f;

    UPROPERTY(EditAnywhere, Category = "Indicator")
    float LimitedDistance = 4.0f;

    UPROPERTY(EditAnyWhere, meta = (InlineEditConditionToggle = "TargetOffset"), Category = "Indicator")
    bool bUseTargetoffset = false;

    UPROPERTY(EditAnywhere, meta = (EditCondition = "bUseTargetoffset"), Category = "Indicator")
    FVector TargetOffset = FVector::ZeroVector;



    UPROPERTY(BlueprintReadWrite,Category = "Indicator")
    float Distance = 0.0f;



private:
    FTimerHandle IndicatorTimerHandle;
    FTimerDelegate IndicatorTimerDelegate;
    
    FVector2D RelativePosition = FVector2D::ZeroVector;
    FVector2D PrimitivePosition = FVector2D::ZeroVector;
    FVector2D ClippingPosition = FVector2D::ZeroVector;
    FVector2D DefaultWindowsSize = FVector2D::ZeroVector;   
};