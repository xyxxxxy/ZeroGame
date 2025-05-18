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


#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override
    {
        return FText::FromString("Radial");
    }
#endif

public:
    UFUNCTION(BlueprintCallable)
    void SetIndex(int32 InIndex);

    UFUNCTION(BlueprintCallable)
    int32 GetIndex() const;

    UFUNCTION(BlueprintCallable)
    void SetCount(int32 InCount);

    UFUNCTION(BlueprintCallable)
    int32 GetCount() const;

public:
    UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RadialWheelMenu")
    int32 Count = 6;
    
    UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RadialWheelMenu")
    int32 Index = 0;

    UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RadialWheelMenu")
    float WidthOverride = 100.f;

    UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RadialWheelMenu")
    float HeightOverride = 100.f;

    UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RadialWheelMenu")
    TObjectPtr<UMaterialInterface> BackgroundMaterial;

    UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "RadialWheelMenu")
    float InnerRadius = 0.25f;

private:

    // 用于创建动态的Material，并且我们在之前写Material时留下了参数
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynMaterial;

    // 用于存储创建Slate的对象
    TSharedPtr<SRadialWheelMenu> MyRadialWheelMenu;
    // 存储Image的画笔对象
    FSlateBrush SlateBrush;
};