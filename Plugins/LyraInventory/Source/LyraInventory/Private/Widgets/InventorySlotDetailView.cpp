
#include "Widgets/InventorySlotDetailView.h"
#include "Inventory/SlotHandleObject.h"
#include "LyraInventoryLibrary.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "Inventory/LyraInventoryComponent.h"
#include "Styling/SlateBrush.h"
#include "CommonTextBlock.h"

UInventorySlotDetailView::UInventorySlotDetailView(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{

}

void UInventorySlotDetailView::FillItemDetails(USlotHandleObject *SlotHandleObject)
{
        // Clear
	Text_ItemName->SetText(FText::FromString(""));
	Text_ItemDescription->SetText(FText::FromString(""));
	Image_Item->SetBrush(Detail_defaultBrush);

	if (SlotHandleObject)
	{
		FLyraInventoryItemSlotHandle SlotHandle = SlotHandleObject->GetPayload().SlotHandle;
		// SlotHandle.
		FLyraInventoryItemSlot ItemSlot = SlotHandle.ParentInventory->GetItemSlot(SlotHandle);
		FLyraInventoryTableRowFragment_UI Fragment_UI;
		if (ULyraInventoryLibrary::FindFragment_UI(ItemSlot.ItemInstance, Fragment_UI))
		{
			Text_ItemName->SetText(FText::FromName(Fragment_UI.ItemName));
                        Text_ItemDescription->SetText(Fragment_UI.Description);
			// if (ItemSlot.ItemInstance->StackSize > 0)
			// {
			// 	// FText MyText = FText::Format(NSLOCTEXT("MyNamespace", "MyKey", "{0}"), ItemSlot.ItemInstance->StackSize);
			// 	FNumberFormattingOptions FormatOptions;
			// 	FormatOptions.UseGrouping = false; // 禁用千位分隔符
			// 	Text_ItemStack->SetText(FText::AsNumber(ItemSlot.ItemInstance->StackSize, &FormatOptions));
			// }
			Image_Item->SetBrush(Fragment_UI.Brush);
		}
	}
}