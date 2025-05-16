
#include "RadialWheelMenu.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SRadialWheelMenu.h"

void URadialWheelMenu::SynchronizeProperties()
{
    if(!MyRadialWheelMenu.IsValid())
        return;

    MyRadialWheelMenu->SetHeightOverrideAttr(HeightOverride);
    MyRadialWheelMenu->SetWidthOverrideAttr(WidthOverride);
    //UE_LOG(LogTemp, Warning, TEXT("URadialWheelMenu::SynchronizeProperties"));
    if (DynMaterial)
    {
        DynMaterial->SetScalarParameterValue("Count", Count);
        DynMaterial->SetScalarParameterValue("Index", Index);
    }
    Super::SynchronizeProperties();
}

void URadialWheelMenu::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyRadialWheelMenu.Reset();
}

TSharedRef<SWidget> URadialWheelMenu::RebuildWidget()
{
    //UE_LOG(LogTemp, Warning, TEXT("URadialWheelMenu::RebuildWidget"));

    if(!DynMaterial)
    {
        if (BackgroundMaterial)
        {
            DynMaterial = UMaterialInstanceDynamic::Create(BackgroundMaterial, this);
            DynMaterial->SetScalarParameterValue("Count", Count);
            DynMaterial->SetScalarParameterValue("Index", Index);
            SlateBrush.SetResourceObject(DynMaterial);
        }
    }


    MyRadialWheelMenu = SNew(SRadialWheelMenu)
                            .WidthOverrideAttr(WidthOverride)
                            .HeightOverrideAttr(HeightOverride)
                            .BackgroundImageAttr(&SlateBrush)
                            ;

    
    //MyRadialWheelMenu->SetOnMouseMove(FPointerEventHandler::CreateUObject(this, &URadialWheelMenu::OnMouseMove));

    return MyRadialWheelMenu.ToSharedRef();
}

FReply URadialWheelMenu::OnMouseMove(const FGeometry &Geometry, const FPointerEvent &PointerEvent)
{
    // 直接捕获鼠标在屏幕上的位置的话返回的是在整个显示视口的位置
    // 所以需要转换成当前控件的位置也就SizeBob的位置
    FDeprecateSlateVector2D MousePostion = Geometry.AbsoluteToLocal(PointerEvent.GetScreenSpacePosition());
    // 创建一个静态方法来获取Index
    // 需要参数OuterRadius，InnerRadius来计算是否在圆内
    Index = GetCurrentSectorIndex(Count, MousePostion, WidthOverride / 2, InnerRadius);
    DynMaterial->SetScalarParameterValue("Index", Index);
    return FReply::Handled();
}

int32 URadialWheelMenu::GetCurrentSectorIndex(int32 SectorCount, FVector2D MousePosition, float OuterRadius, float InnerRadius)
{
    return 0;
}