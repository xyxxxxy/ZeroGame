
#include "Widgets/InventoryTileView.h"
#include "Widgets/InventoryListEntry.h"
#include "Inventory/SlotHandleObject.h"

UInventoryTileView::UInventoryTileView(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

UUserWidget &UInventoryTileView::OnGenerateEntryWidgetInternal(UObject *Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase> &OwnerTable)
{

        USlotHandleObject *SlotHandleObjectItem = Cast<USlotHandleObject>(Item);

        TSubclassOf<UInventorySlotEntryBase> SettingEntryClass = TSubclassOf<UInventorySlotEntryBase>(DesiredEntryClass);
        // 生成EntryWidget
        UInventorySlotEntryBase &EntryWidget = GenerateTypedEntry<UInventorySlotEntryBase>(SettingEntryClass, OwnerTable);

        if (!IsDesignTime())
        {
                // OnGenerateEntryWidgetInternal  设置SlotHandleObjectItem
                EntryWidget.SetSlotHandleObjects(SlotHandleObjectItem);
        }

        return EntryWidget;
}