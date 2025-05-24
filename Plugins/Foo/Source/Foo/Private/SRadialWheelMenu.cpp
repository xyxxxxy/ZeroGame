
#include "SRadialWheelMenu.h"
#include "Styling/SlateBrush.h"
#include "Blueprint/UserWidget.h"

SLATE_IMPLEMENT_WIDGET(SRadialWheelMenu)

void SRadialWheelMenu::PrivateRegisterAttributes(FSlateAttributeInitializer &AttributeInitializer)
{
    // SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "Width", Width, EInvalidateWidgetReason::Layout)
    //     .OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget &Widget)
    //         {
    //             static_cast<SRadialWheelMenu &>(Widget).UpdateWidth();
    //         }));

    // SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "Height", Height, EInvalidateWidgetReason::Layout)
    //     .OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget &Widget)
    //         {
    //             static_cast<SRadialWheelMenu &>(Widget).UpdateHeight();
    //         }));

    // SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "BackgroundImage", BackgroundImage, EInvalidateWidgetReason::Layout)
    //     .OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget &Widget)
    //         {
    //             static_cast<SRadialWheelMenu &>(Widget).UpdateBackgroundImage();
    //         }));

	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, Count, EInvalidateWidgetReason::Layout);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, RadiusCoefficient, EInvalidateWidgetReason::Layout);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, Width, EInvalidateWidgetReason::Layout);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, Height, EInvalidateWidgetReason::Layout);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, ItemOffset, EInvalidateWidgetReason::Layout);
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION(AttributeInitializer, BackgroundImage, EInvalidateWidgetReason::Paint);
}

SRadialWheelMenu::SRadialWheelMenu()
    : Count(*this)
	, RadiusCoefficient(*this) 
	, Width(*this)
    , Height(*this)
	, ItemOffset(*this)
    , BackgroundImage(*this)
{
}

SRadialWheelMenu::~SRadialWheelMenu()
{
	MyCanvas.Reset();
}

void SRadialWheelMenu::Construct(const FArguments &InArgs)
{
	MyCanvas = SNew(SCanvas);

	SetCount(InArgs._Count);
	SetRadiusCoefficient(InArgs._RadiusCoefficient);
    SetWidth(InArgs._Width);
    SetHeight(InArgs._Height);
	SetItemOffset(InArgs._ItemOffset);
    SetBackgroundImage(InArgs._BackgroundImage);

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(Width.Get())// 绑定动态属性改变时保证会重新渲染 
		.HeightOverride(Height.Get())
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(BackgroundImage.Get())
				.ColorAndOpacity(FSlateColor(FColor::White))
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				MyCanvas.ToSharedRef()
			]
		]
	];
}

FVector2D SRadialWheelMenu::ComputeDesiredSize(float) const
{
	EVisibility ChildVisibility = ChildSlot.GetWidget()->GetVisibility();

	if ( ChildVisibility != EVisibility::Collapsed )
	{
		const FOptionalSize CurrentWidthOverride = Width.Get();
		const FOptionalSize CurrentHeightOverride = Height.Get();

		return FVector2D(
			( CurrentWidthOverride.IsSet() ) ? CurrentWidthOverride.Get() : 0.0f,
			( CurrentHeightOverride.IsSet() ) ? CurrentHeightOverride.Get() : 0.0f
		);
	}
	
	return FVector2D::ZeroVector;
}

void SRadialWheelMenu::SetCount(TAttribute<int32> InCount)
{
	Count.Assign(*this, MoveTemp(InCount));
}

void SRadialWheelMenu::SetRadiusCoefficient(TAttribute<float> InRadiusCoefficient)
{
	RadiusCoefficient.Assign(*this, MoveTemp(InRadiusCoefficient));
}

void SRadialWheelMenu::SetWidth(TAttribute<FOptionalSize> InWidth)
{
    Width.Assign(*this, MoveTemp(InWidth));
}

void SRadialWheelMenu::SetHeight(TAttribute<FOptionalSize> InHeight)
{
    Height.Assign(*this, MoveTemp(InHeight));
}

void SRadialWheelMenu::SetItemOffset(TAttribute<FVector2D> InItemOffset)
{
	ItemOffset.Assign(*this, MoveTemp(InItemOffset));
}

void SRadialWheelMenu::SetBackgroundImage(TAttribute<const FSlateBrush*> InSlateBrush)
{
    BackgroundImage.Assign(*this, MoveTemp(InSlateBrush));
}

void SRadialWheelMenu::InitializeCircularSlots(int32 NumSectors, float OuterRadius, float InnerRadius, const FVector2D& Center)
{
	MyCanvas->ClearChildren();
	CanvasSlots.Reset();

	constexpr float HeightRatio = 0.3f;
	const float AngleStep = 360.0f / NumSectors;
	const float SectorAngle = FMath::DegreesToRadians(AngleStep);
	const float MaxWidth = 2 * (OuterRadius * (1.f - HeightRatio)) * FMath::Sin(SectorAngle / 2); // 根据弦长公式计算最大宽度
	const FVector2D WidgetSize(
		FMath::Min(MaxWidth - 5.0f, 100.0f), // 留5px边距，设置最大100px防止过大
		(OuterRadius - InnerRadius) * (1.f - HeightRatio) // 高度设为半径的80%
	);

	// UE_LOG(LogTemp, Warning, TEXT("InnerRadius: %f"), InnerRadius);
	// UE_LOG(LogTemp, Warning, TEXT("OuterRadius: %f"), OuterRadius);
	// UE_LOG(LogTemp, Warning, TEXT("Center: %f, %f"), Center.X, Center.Y);
	for (int32 i = 0; i < NumSectors; ++i)
	{
		// 计算角度（转换为弧度）
		const float Angle = FMath::DegreesToRadians(AngleStep * (i + 0.5));
		//UE_LOG(LogTemp, Warning, TEXT("Angle: %f"), Angle);
		// 计算坐标位置（中心点对齐）
		const float MidRadius = (OuterRadius + InnerRadius) / 2;
		FVector2D Position(
			Center.X + (MidRadius + ItemOffset.Get().X) * FMath::Cos(Angle) - WidgetSize.X / 2,
			Center.Y - (MidRadius + ItemOffset.Get().Y) * FMath::Sin(Angle) - WidgetSize.Y / 2
		);

		Position -= this->GetCachedGeometry().GetLocalPositionAtCoordinates(FVector2D(0.0f, 0.0f));
		//Position -= this->GetTickSpaceGeometry().GetLocalPositionAtCoordinates(FVector2D(0.0f, 0.0f));

		//UE_LOG(LogTemp, Warning, TEXT("Position: %f, %f"), Position.X, Position.Y);
		// 创建占位插槽
		auto NewSlot = MyCanvas->AddSlot()
		                        .Position(Position)
		                        .Size(WidgetSize)
		                        .VAlign(VAlign_Fill)
		                        .HAlign(HAlign_Fill)
								[
									SNew(SBorder)
									.VAlign(VAlign_Center)
									.HAlign(HAlign_Center)
									.ColorAndOpacity(FLinearColor(1.0f, 0.0f, 0.7f, 1.0f))
									[
										SNew(STextBlock)
										.Text(FText::FromString(FString::FromInt(i + 1)))
										.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 24))
									]
								].GetSlot();
		
		CanvasSlots.Add(NewSlot);
	}
	bIsInitialized = true;
}

void SRadialWheelMenu::InsertWidget(UUserWidget* InWidget, int32 Index)
{
	if (CanvasSlots.IsValidIndex(Index) && CanvasSlots[Index] != nullptr)
	{
		CanvasSlots[Index]->AttachWidget(InWidget->TakeWidget());
	}
}