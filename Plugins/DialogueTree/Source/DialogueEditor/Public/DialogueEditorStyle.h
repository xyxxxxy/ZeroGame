
#pragma once

#include "EditorStyleSet.h"
#include "Styling/SlateStyle.h"

#include "DialogueEditorStyle.generated.h"
USTRUCT()
struct FRegisterBrushArgs
{
	GENERATED_BODY()
	
	FString PropertyName;
	FString ImageName;
	FVector2D ImageSize = FVector2D(22.f);
	ESlateBrushDrawType::Type DrawType = ESlateBrushDrawType::Image;
	FMargin DrawMargin = FMargin();
	FSlateColor TintColor = FSlateColor(FColor::White);
};

USTRUCT()
struct FRegisterTextStyleArgs
{
	GENERATED_BODY()

	/** The base style for the new text style */
	FTextBlockStyle BaseStyle = FAppStyle::GetWidgetStyle<FTextBlockStyle>("Graph.Node.NodeTitle");
	
	int32 Size = 10; 
	FVector2D ShadowOffset = FVector2D(2.f, 2.f);
	FLinearColor TextColor = FLinearColor::White;
};


class DIALOGUEEDITOR_API FDialogueEditorStyle
{
public:
	static TSharedPtr<FSlateStyleSet> Get();
	static FName GetStyleSetName();

	static void Initialize();
	static void ShutDown();

	static void RegisterIconBrushes();
	static const FSlateBrush* GetBrush(FName PropertyName,const ANSICHAR* Specifier = nullptr);

	static void RegisterTextStyles();
	static const FTextBlockStyle& GetTextStyle(FName PropertyName,const ANSICHAR* Specifier = nullptr);
	static FTextBlockStyle GetDerivativeTextStyle(FRegisterTextStyleArgs& InArgs);

	static const FVector2D GetVector(FName PropertyName,const ANSICHAR* Specifier = nullptr);

	
private:
	static TSharedPtr<FSlateStyleSet> StyleSet;
};