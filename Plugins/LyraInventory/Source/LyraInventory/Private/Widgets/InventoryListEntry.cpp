
#include "Widgets/InventoryListEntry.h"
#include "CommonInputSubsystem.h"
#include "Inventory/SlotHandleObject.h"

struct FGeometry;

void UInventorySlotEntryBase::SetSlotHandleObjects(USlotHandleObject* InObject)
{

}

void UInventorySlotEntryBase::NativeOnEntryReleased()
{
	StopAllAnimations();

	if (Background)
	{
		Background->StopAllAnimations();
	}

	// if (ensure(Setting))
	// {
	// 	Setting->OnSettingEditConditionChangedEvent.RemoveAll(this);
	// 	Setting->OnSettingChangedEvent.RemoveAll(this);
	// }

	// Setting = nullptr;
}

FReply UInventorySlotEntryBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
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



void UInventorySlotEntry::SetSlotHandleObjects(USlotHandleObject* InObject)
{
	if(IsValid(InObject))
	{
		SlotHandleObject = InObject;
	}
}

