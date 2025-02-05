
#include "DialogueEditorStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

TSharedPtr<FSlateStyleSet> FDialogueEditorStyle::StyleSet = nullptr;

TSharedPtr<FSlateStyleSet> FDialogueEditorStyle::Get()
{
	return StyleSet;
}

FName FDialogueEditorStyle::GetStyleSetName()
{
	static FName DialogueEditorStyleName(TEXT("DialogueEditorStyle"));
	return DialogueEditorStyleName;
}

void FDialogueEditorStyle::Initialize()
{
	StyleSet = MakeShareable(new FSlateStyleSet(TEXT("DialogueEditorStyle")));

	RegisterIconBrushes();
	RegisterTextStyles();

	StyleSet->Set(TEXT("PinSize"), FVector2D(15.f));

	
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	
}

void FDialogueEditorStyle::ShutDown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
	ensure(StyleSet.IsUnique());
	StyleSet.Reset();
}

void FDialogueEditorStyle::RegisterIconBrushes()
{
	const FVector2D Icon128(128.0f,128.0f);
	const FVector2D Icon64(64.0f,64.0f);
	const FVector2D Icon32(32.0f,32.0f);
	
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("DialogueTree");
	
	const FString PluginRoot = Plugin->GetBaseDir();
	StyleSet->SetContentRoot(PluginRoot / TEXT("Resources"));

	StyleSet->Set("ClassIcon.Dialogue",new IMAGE_BRUSH("Icons/Icon1",Icon128));
	StyleSet->Set("ClassThumbnail.Dialogue",new IMAGE_BRUSH("Icons/Icon2",Icon128));
	
	StyleSet->Set("StandardPin",new IMAGE_BRUSH("Icons/CubePin_Red",Icon32));
	StyleSet->Set("ExitPin",new IMAGE_BRUSH("Icons/CubePin_Grey",Icon32));

	StyleSet->Set("InputTransitionIcon",new IMAGE_BRUSH("Icons/ArrowIcon_Input",Icon32));
	StyleSet->Set("AutoTransitionIcon",new IMAGE_BRUSH("Icons/ArrowIcon_Auto",Icon32));

	StyleSet->Set("NodeHeaderGloss",new IMAGE_BRUSH("Icons/NodeHeaderGloss",Icon32));
	StyleSet->Set("NodeHeaderColorSpill",new IMAGE_BRUSH("Icons/NodeHeaderColorSpill",Icon32));
}

const FSlateBrush* FDialogueEditorStyle::GetBrush(FName PropertyName, const ANSICHAR* Specifier)
{
	return Get()->GetBrush(PropertyName,Specifier);
}

void FDialogueEditorStyle::RegisterTextStyles()
{
	check(StyleSet.IsValid());
	
	//Register title text style 
	FRegisterTextStyleArgs TitleTextArgs;
	TitleTextArgs.Size = 16;

	StyleSet->Set("NodeTitleStyle", GetDerivativeTextStyle(TitleTextArgs));

	//Register subtitle text style 
	FRegisterTextStyleArgs SubtitleTextArgs;
	SubtitleTextArgs.Size = 12;

	StyleSet->Set("NodeSubtitleStyle", GetDerivativeTextStyle(SubtitleTextArgs));
}

const FTextBlockStyle& FDialogueEditorStyle::GetTextStyle(FName PropertyName, const ANSICHAR* Specifier)
{
	return StyleSet->GetWidgetStyle<FTextBlockStyle>(PropertyName, Specifier);
}

FTextBlockStyle FDialogueEditorStyle::GetDerivativeTextStyle(FRegisterTextStyleArgs& InArgs)
{
	FSlateFontInfo Font = InArgs.BaseStyle.Font;
	Font.Size = 12;

	return FTextBlockStyle(InArgs.BaseStyle)
		.SetFont(Font)
		.SetColorAndOpacity(InArgs.TextColor)
		.SetShadowOffset(InArgs.ShadowOffset)
		.SetShadowColorAndOpacity(FLinearColor(0.f, 0.f, 0.f, 0.7f));
}

const FVector2D FDialogueEditorStyle::GetVector(FName PropertyName, const ANSICHAR* Specifier)
{
	return StyleSet->GetVector(PropertyName,Specifier);
}

#undef IMAGE_BRUSH
