#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCanvas.h"
#include "Widgets/SCompoundWidget.h"

struct FSlateBrush;
class SRadialWheelMenu : public SCompoundWidget
{

    SLATE_DECLARE_WIDGET(SRadialWheelMenu, SCompoundWidget)
public:
    // 创建构造参数并且能初始化参数
    SLATE_BEGIN_ARGS(SRadialWheelMenu) {}
        SLATE_ATTRIBUTE(int32, Count)
        SLATE_ATTRIBUTE(float, RadiusCoefficient)
        SLATE_ATTRIBUTE(FOptionalSize, Width)
        SLATE_ATTRIBUTE(FOptionalSize, Height)
        SLATE_ATTRIBUTE(FVector2D, ItemOffset)
        SLATE_ATTRIBUTE(const FSlateBrush*, BackgroundImage)
    SLATE_END_ARGS()

    void Construct(const FArguments &InArgs);

    SRadialWheelMenu();
    ~SRadialWheelMenu();

protected:
    virtual FVector2D ComputeDesiredSize(float) const override;
    
private:
    // 当参数修改时能够刷新
    TSlateAttribute<int32> Count;
    TSlateAttribute<float> RadiusCoefficient;
    TSlateAttribute<FOptionalSize> Width;
    TSlateAttribute<FOptionalSize> Height;
    TSlateAttribute<FVector2D> ItemOffset;
    TSlateAttribute<const FSlateBrush*> BackgroundImage;

    

public:
    void SetCount(TAttribute<int32> InCount);
    void SetRadiusCoefficient(TAttribute<float> InRadiusCoefficient);
    void SetWidth(TAttribute<FOptionalSize> InWidth);
    void SetHeight(TAttribute<FOptionalSize> InHeight);
    void SetItemOffset(TAttribute<FVector2D> InItemOffset);
    void SetBackgroundImage(TAttribute<const FSlateBrush*> InSlateBrush);

    void InitializeCircularSlots(int32 NumSectors, float OuterRadius, float InnerRadius, const FVector2D& Center);
    void InsertWidget(UUserWidget* InWidget, int32 Index);

    bool IsInitialized() const
    {
        return bIsInitialized;
    }

    void Test(const TSharedRef<SWidget>& InWidget) {}

private:
    // void UpdateWidth();
    // void UpdateHeight();
    // void UpdateBackgroundImage();

    TSharedPtr<SCanvas> MyCanvas;

    bool bIsInitialized = false;

private:
    TArray<SCanvas::FSlot*> CanvasSlots;
};