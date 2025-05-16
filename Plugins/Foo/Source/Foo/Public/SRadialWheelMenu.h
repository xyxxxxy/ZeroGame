#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCanvas.h"

struct FSlateBrush;
class SRadialWheelMenu : public SCompoundWidget
{

    SLATE_DECLARE_WIDGET(SRadialWheelMenu, SCompoundWidget)
public:
    // 创建构造参数并且能初始化参数
    SLATE_BEGIN_ARGS(SRadialWheelMenu) {}
        SLATE_ATTRIBUTE(FOptionalSize, WidthOverrideAttr)
        SLATE_ATTRIBUTE(FOptionalSize, HeightOverrideAttr)
        SLATE_ATTRIBUTE(const FSlateBrush*, BackgroundImageAttr)
    SLATE_END_ARGS()

    void Construct(const FArguments &InArgs);

    SRadialWheelMenu();

protected:
    virtual FVector2D ComputeDesiredSize(float) const override;
    
private:
    // 当参数修改时能够刷新
    TSlateAttribute<FOptionalSize> WidthOverrideAttr;
    TSlateAttribute<FOptionalSize> HeightOverrideAttr;
    TSlateAttribute<const FSlateBrush*> BackgroundImageAttr;
    //TAttribute<const FSlateBrush *> test;

public:
    void SetWidthOverrideAttr(TAttribute<FOptionalSize> WidthOverride);

    void SetHeightOverrideAttr(TAttribute<FOptionalSize> HeightOverride);

    void SetBackgroundImageAttr(TAttribute<const FSlateBrush*> SlateBrush);

private:
    void UpdateWidth();
    void UpdateHeight();
    void UpdateBackgroundImage();
};