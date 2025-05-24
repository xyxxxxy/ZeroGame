
#include "RadialWheelMenu.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SRadialWheelMenu.h"
#include "Blueprint/UserWidget.h"
void URadialWheelMenu::SynchronizeProperties()
{
    if(!MyRadialWheelMenu.IsValid())
        return;

    MyRadialWheelMenu->SetHeight(HeightOverride);
    MyRadialWheelMenu->SetWidth(WidthOverride);
    MyRadialWheelMenu->SetCount(Count);
    MyRadialWheelMenu->SetItemOffset(ItemOffset);
    MyRadialWheelMenu->SetRadiusCoefficient(InnerRadiusCoefficient);

    if(IsDesignTime())
    {
        MyRadialWheelMenu->InitializeCircularSlots(
            Count, 
            WidthOverride / 2, 
            InnerRadiusCoefficient * WidthOverride, 
            GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f))
        );
    }

    //UE_LOG(LogTemp, Warning, TEXT("URadialWheelMenu::SynchronizeProperties"));
    if (DynMaterial)
    {
        DynMaterial->SetScalarParameterValue("Count", Count);
        DynMaterial->SetScalarParameterValue("Index", Index);
        DynMaterial->SetScalarParameterValue("InnerRadiusCoefficient", InnerRadiusCoefficient);
    }
    Super::SynchronizeProperties();
}

void URadialWheelMenu::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyRadialWheelMenu.Reset();
    if (RefreshHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(RefreshHandle);
	}

    if (SlotHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(SlotHandle);
	}
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
            DynMaterial->SetScalarParameterValue("InnerRadiusCoefficient", InnerRadiusCoefficient);
            SlateBrush.SetResourceObject(DynMaterial);
        }
    }

    MyRadialWheelMenu = SNew(SRadialWheelMenu)
                            .Count(Count)
                            .RadiusCoefficient(InnerRadiusCoefficient)
                            .Width(WidthOverride)
                            .Height(HeightOverride)
                            .ItemOffset(ItemOffset)
                            .BackgroundImage(&SlateBrush)
                            ;

    SlotHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this](float DeltaTime)
    {
        //UE_LOG(LogTemp, Warning, TEXT("This will be printed in log after 1 tick"));
        if(!MyRadialWheelMenu.IsValid())
        {
            return true;
        }
        SlotHandle.Reset();
        MyRadialWheelMenu->InitializeCircularSlots(
            Count, 
            WidthOverride / 2, 
            InnerRadiusCoefficient * WidthOverride, 
            GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.5f, 0.5f))
        );
        // Returning false will remove the ticker from the ticker list
        return false;
    }));

    return MyRadialWheelMenu.ToSharedRef();
}

float URadialWheelMenu::GetWidthOverride() const
{
    return WidthOverride;
}
void URadialWheelMenu::SetIndex(int32 InIndex)
{
    if(Index != InIndex)
    {
        Index = InIndex;
        if(DynMaterial)
        {
            //GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::Printf(TEXT("URadialWheelMenu->SetIndex: %d"), Index));
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

void URadialWheelMenu::AddItem(UUserWidget* InWidget)
{
    // if(RefreshHandle.IsValid())
    // {
    //     return;
    // }

    if (InWidget == nullptr || ListItems.Contains(InWidget))
    {
        return;
    }

    RefreshHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([this, InWidget](float DeltaTime)
    {
        if(!MyRadialWheelMenu.IsValid())
        {
            return true;
        }

        if(MyRadialWheelMenu->IsInitialized())
        {
            if(ListItems.Num() + 1 > Count)
            {
                SetCount(Count + 1);
            }

            RefreshHandle.Reset();
            
            int32 TempIndex = ListItems.Add(InWidget);
                  
            //MyRadialWheelMenu->Test(InWidget->TakeWidget());
            MyRadialWheelMenu->InsertWidget(InWidget, TempIndex);
            return false;    
        }
        return true;
    }));

}

