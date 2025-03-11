
#pragma once

#include "CommonUserWidget.h"

#include "InventoryPanel.generated.h"

struct FFocusEvent;
struct FGeometry;
class USlotHandleObject;
class UInventoryTileView;
UCLASS(Abstract)
class LYRAINVENTORY_API UInventoryPanel : public UCommonUserWidget
{
        GENERATED_BODY()

public:
        UInventoryPanel();

        virtual FReply NativeOnFocusReceived(const FGeometry &InGeometry, const FFocusEvent &InFocusEvent) override;

private:
        UPROPERTY(Transient)
        TArray<TObjectPtr<USlotHandleObject>> SlotHandleObjects;

private:
        UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UInventoryTileView> TileView_Settings;
};