#include "LyraHUDLayout.h"

#include "CommonUIExtensions.h"
#include "Input/CommonUIInputTypes.h"
#include "LyraGameplayTags.h"


ULyraHUDLayout::ULyraHUDLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


void ULyraHUDLayout::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// This registers a "UI Action Binding" with CommonUI, such that when the UI_Action_MainMenu UI Action occurs,
	// the delegate is called which forwards the event to our own HandleMainMenuAction
	
	ForwardHandle = RegisterUIActionBinding(FBindUIActionArgs(ForwardInputActionData,true,FSimpleDelegate::CreateUObject(this,&ULyraHUDLayout::Forward)));
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData,true,FSimpleDelegate::CreateUObject(this,&ULyraHUDLayout::Back)));

	constexpr bool bShouldDisplayInActionBar = false;
//	RegisterUIActionBinding(FBindUIActionArgs(FUIActionTag::ConvertChecked(LyraTag::UI_Action_MainMenu), bShouldDisplayInActionBar, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleMainMenuAction)));
}


void ULyraHUDLayout::HandleMainMenuAction()
{
	if (ensure(!MainMenuClass.IsNull()))
	{
//		UCommonUIExtensions::PushStreamedContentToLayer_ForPlayer(GetOwningLocalPlayer(), LyraTag::UI_Layer_Menu, MainMenuClass);
	}
}

void ULyraHUDLayout::Forward()
{
	K2_Forward();
}

void ULyraHUDLayout::Back()
{
	DeactivateWidget();
}