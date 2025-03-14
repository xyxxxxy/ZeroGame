
#include "Widgets/InventoryListEntry.h"
#include "CommonInputSubsystem.h"
#include "Inventory/SlotHandleObject.h"
#include "LyraInventoryLibrary.h"
#include "Components/Image.h"
#include "Inventory/LyraInventoryComponent.h"
#include "CommonTextBlock.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Styling/SlateBrush.h"

struct FGeometry;

void UInventorySlotEntryBase::SetSlotHandleObjects(USlotHandleObject *InObject)
{
}

void UInventorySlotEntryBase::OnSettingChanged()
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

FReply UInventorySlotEntryBase::NativeOnFocusReceived(const FGeometry &InGeometry, const FFocusEvent &InFocusEvent)
{
	const UCommonInputSubsystem *InputSubsystem = GetInputSubsystem();
	if (InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget *PrimaryFocus = GetPrimaryGamepadFocusWidget())
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

void UInventorySlotEntry::SetSlotHandleObjects(USlotHandleObject *InObject)
{

	SlotHandleObject = InObject;

	Refresh();
}

void UInventorySlotEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInventorySlotEntry::Refresh()
{
	// Clear
	Text_ItemName->SetText(FText::FromString(""));
	Text_ItemStack->SetText(FText::FromString(""));
	Image_Item->SetBrush(defaultBrush);

	if (ensure(SlotHandleObject))
	{
		FLyraInventoryItemSlotHandle SlotHandle = SlotHandleObject->GetPayload().SlotHandle;
		// SlotHandle.
		FLyraInventoryItemSlot ItemSlot = SlotHandle.ParentInventory->GetItemSlot(SlotHandle);
		FLyraInventoryTableRowFragment_UI Fragment_UI;
		if (ULyraInventoryLibrary::FindFragment_UI(ItemSlot.ItemInstance, Fragment_UI))
		{
			Text_ItemName->SetText(FText::FromName(Fragment_UI.ItemName));
			if (ItemSlot.ItemInstance->StackSize > 0)
			{
				// FText MyText = FText::Format(NSLOCTEXT("MyNamespace", "MyKey", "{0}"), ItemSlot.ItemInstance->StackSize);
				FNumberFormattingOptions FormatOptions;
				FormatOptions.UseGrouping = false; // 禁用千位分隔符
				Text_ItemStack->SetText(FText::AsNumber(ItemSlot.ItemInstance->StackSize, &FormatOptions));
			}
			Image_Item->SetBrush(Fragment_UI.Brush);
		}
	}
}