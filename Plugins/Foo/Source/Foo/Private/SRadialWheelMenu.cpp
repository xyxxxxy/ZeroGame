
#include "SRadialWheelMenu.h"
#include "Styling/SlateBrush.h"

SLATE_IMPLEMENT_WIDGET(SRadialWheelMenu)

void SRadialWheelMenu::PrivateRegisterAttributes(FSlateAttributeInitializer &AttributeInitializer)
{
    SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "WidthOverrideAttr", WidthOverrideAttr, EInvalidateWidgetReason::Layout)
        .OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget &Widget)
            {
                static_cast<SRadialWheelMenu &>(Widget).UpdateWidth();
            }));

    SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "HeightOverrideAttr", HeightOverrideAttr, EInvalidateWidgetReason::Layout)
        .OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget &Widget)
            {
                static_cast<SRadialWheelMenu &>(Widget).UpdateHeight();
            }));

    SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "BackgroundImageAttr", BackgroundImageAttr, EInvalidateWidgetReason::Layout)
        .OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget &Widget)
            {
                static_cast<SRadialWheelMenu &>(Widget).UpdateBackgroundImage();
            }));

	// SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, HeightOverrideAttr, EInvalidateWidgetReason::Layout);
	// SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, BackgroundImageAttr, EInvalidateWidgetReason::Paint);
}

SRadialWheelMenu::SRadialWheelMenu()
    : WidthOverrideAttr(*this)
    , HeightOverrideAttr(*this)
    , BackgroundImageAttr(*this)
{
}

void SRadialWheelMenu::Construct(const FArguments &InArgs)
{
	// WidthOverrideAttr = InArgs._WidthOverride;
	// HeightOverrideAttr = InArgs._HeightOverride;
	// BackgroundImageAttr = InArgs._BackgroundImage;


    SetWidthOverrideAttr(InArgs._WidthOverrideAttr);
    SetHeightOverrideAttr(InArgs._HeightOverrideAttr);
    SetBackgroundImageAttr(InArgs._BackgroundImageAttr);
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(WidthOverrideAttr.Get())// 绑定动态属性改变时保证会重新渲染 
		.HeightOverride(HeightOverrideAttr.Get())
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(BackgroundImageAttr.Get())
				.ColorAndOpacity(FSlateColor(FColor::White))
			]
		]
	];
}

FVector2D SRadialWheelMenu::ComputeDesiredSize(float) const
{
	EVisibility ChildVisibility = ChildSlot.GetWidget()->GetVisibility();

	if ( ChildVisibility != EVisibility::Collapsed )
	{
		const FOptionalSize CurrentWidthOverride = WidthOverrideAttr.Get();
		const FOptionalSize CurrentHeightOverride = HeightOverrideAttr.Get();

		return FVector2D(
			( CurrentWidthOverride.IsSet() ) ? CurrentWidthOverride.Get() : 0.0f,
			( CurrentHeightOverride.IsSet() ) ? CurrentHeightOverride.Get() : 0.0f
		);
	}
	
	return FVector2D::ZeroVector;
}

void SRadialWheelMenu::SetWidthOverrideAttr(TAttribute<FOptionalSize> WidthOverride)
{
    WidthOverrideAttr.Assign(*this, MoveTemp(WidthOverride));
}

void SRadialWheelMenu::SetHeightOverrideAttr(TAttribute<FOptionalSize> HeightOverride)
{
    HeightOverrideAttr.Assign(*this, MoveTemp(HeightOverride));
}

void SRadialWheelMenu::SetBackgroundImageAttr(TAttribute<const FSlateBrush*> SlateBrush)
{
    BackgroundImageAttr.Assign(*this, MoveTemp(SlateBrush));
}


void SRadialWheelMenu::UpdateWidth()
{
    SetWidthOverrideAttr(WidthOverrideAttr.Get());
}

void SRadialWheelMenu::UpdateHeight()
{
    SetHeightOverrideAttr(HeightOverrideAttr.Get());        
}

void SRadialWheelMenu::UpdateBackgroundImage()
{
    SetBackgroundImageAttr(BackgroundImageAttr.Get());
}