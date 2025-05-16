#pragma once

#include "CoreMinimal.h"

#include "Components/Widget.h"
#include "RadialWheelMenu.Generated.h"

struct FSlateBrush;
class UMaterialInstanceDynamic;
class SRadialWheelMenu;

UCLASS(MinimalAPI)
class URadialWheelMenu : public UWidget
{
    GENERATED_BODY()
public:
    //~ Begin UWidget Interface
    virtual void SynchronizeProperties() override;
    //~ End UWidget Interface

    //~ Begin UVisual Interface
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
    //~ Begin UWidget Interface
    virtual TSharedRef<SWidget> RebuildWidget() override;

public:
    static int32 GetCurrentSectorIndex(int32 SectorCount, FVector2D MousePosition, float OuterRadius, float InnerRadius);

#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override
    {
        return FText::FromString("Radial");
    }
#endif

private:
    FReply OnMouseMove(const FGeometry& Geometry,const FPointerEvent& PointerEvent);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialWheelMenu")
    int32 Count = 8;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialWheelMenu")
    int32 Index = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialWheelMenu")
    float WidthOverride = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialWheelMenu")
    float HeightOverride = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialWheelMenu")
    TObjectPtr<UMaterialInterface> BackgroundMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RadialWheelMenu")
    int32 InnerRadius;
    // 用于创建动态的Material，并且我们在之前写Material时留下了参数
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynMaterial;

private:
    // 用于存储创建Slate的对象
    TSharedPtr<SRadialWheelMenu> MyRadialWheelMenu;
    // 存储Image的画笔对象
    FSlateBrush SlateBrush;
};