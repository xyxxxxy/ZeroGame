#include "Widgets/InventoryTileView.h"

UInventoryTileView::UInventoryTileView(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

UUserWidget &UInventoryTileView::OnGenerateEntryWidgetInternal(UObject *Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase> &OwnerTable)
{
        return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
}