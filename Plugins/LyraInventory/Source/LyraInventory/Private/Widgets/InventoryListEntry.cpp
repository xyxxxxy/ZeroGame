
#include "Widgets/InventoryListEntry.h"
#include "CommonInputSubsystem.h"

struct FGeometry;
FReply UGameSettingListEntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
        const UCommonInputSubsystem* InputSubsystem = GetInputSubsystem();
	if (InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* PrimaryFocus = GetPrimaryGamepadFocusWidget())
		{
			TSharedPtr<SWidget> WidgetToFocus = PrimaryFocus->GetCachedWidget();
			if (WidgetToFocus.IsValid())
			{
				return FReply::Handled().SetUserFocus(WidgetToFocus.ToSharedRef(), InFocusEvent.GetCause());
			}
		}
	}

	return FReply::Unhandled();
}