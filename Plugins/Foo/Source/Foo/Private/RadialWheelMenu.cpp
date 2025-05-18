
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
        DynMaterial->SetScalarParameterValue("InnerRadius", InnerRadius);
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
            DynMaterial->SetScalarParameterValue("InnerRadius", InnerRadius);
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

void URadialWheelMenu::SetIndex(int32 InIndex)
{
    if(Index != InIndex)
    {
        Index = InIndex;
        if(DynMaterial)
        {
            DynMaterial->SetScalarParameterValue("Index", Index);
        }
    }
}

int32 URadialWheelMenu::GetIndex() const
{
    return Index;
}

void URadialWheelMenu::SetCount(int32 InCount)
{
    if(Count != InCount)
    {
        Count = InCount;
        if(DynMaterial)
        {
            DynMaterial->SetScalarParameterValue("Count", Count);
        }
    }
}

int32 URadialWheelMenu::GetCount() const
{
    return Count;
}