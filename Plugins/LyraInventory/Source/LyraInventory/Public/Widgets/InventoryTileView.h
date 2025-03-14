#pragma once

#include "CommonTileView.h"
#include "InventoryTileView.generated.h"

class STableViewBase;

/**
 * TileView specialized to navigate on focus for consoles & enable scrolling when not focused for touch
 */
UCLASS(meta = (DisableNativeTick, EntryClass = UInventorySlotEntryBase))
class LYRAINVENTORY_API UInventoryTileView : public UCommonTileView
{
	GENERATED_BODY()

public:
	UInventoryTileView(const FObjectInitializer& ObjectInitializer);

protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;	
};