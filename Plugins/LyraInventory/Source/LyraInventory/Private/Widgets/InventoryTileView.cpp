
#include "Widgets/InventoryTileView.h"
#include "Widgets/InventorySlotEntry.h"
#include "Inventory/SlotHandleObject.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryTileView)

UInventoryTileView::UInventoryTileView(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

UUserWidget &UInventoryTileView::OnGenerateEntryWidgetInternal(UObject *Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase> &OwnerTable)
{

        USlotHandleObject *SlotHandleObjectItem = Cast<USlotHandleObject>(Item);

        TSubclassOf<UInventorySlotEntryBase> SettingEntryClass = TSubclassOf<UInventorySlotEntryBase>(DesiredEntryClass);
        // 生成EntryWidget
        UInventorySlotEntryBase &EntryWidget = GenerateTypedEntry<UInventorySlotEntryBase>(SettingEntryClass, OwnerTable);

        // Returns if the widget is currently being displayed in the designer, it may want to display different data
        // 在编辑器中不执行
        if (!IsDesignTime())
        {
                // OnGenerateEntryWidgetInternal  设置SlotHandleObjectItem
                EntryWidget.SetSlotHandleObjects(SlotHandleObjectItem);
                
        }
        return EntryWidget;
}