
#pragma once

#include "Blueprint/UserWidgetPool.h"
#include "Blueprint/UserWidget.h"

#include "InventorySlotDetailView.generated.h"

class UCommonRichTextBlock;
class UCommonTextBlock;
struct FSlateBrush;
class UImage;
// class UObject;

// struct FStreamableHandle;
class ULyraInventoryComponent;
class USlotHandleObject;

/**
 *
 */
UCLASS(Abstract)
class LYRAINVENTORY_API UInventorySlotDetailView : public UUserWidget
{
        GENERATED_BODY()
public:
        UInventorySlotDetailView(const FObjectInitializer &ObjectInitializer);

        void FillItemDetails(USlotHandleObject *SlotHandleObject);

        // //UVisual interface
        // virtual void ReleaseSlateResources(bool bReleaseChildren) override;
        // //~ End UVisual Interface
protected:
        UPROPERTY(Transient)
        TObjectPtr<USlotHandleObject> CurrentSlotHandleObject;

        UPROPERTY(EditAnyWhere)
	FSlateBrush Detail_defaultBrush;

private:
        UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UCommonTextBlock> Text_ItemName;

        UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UCommonTextBlock> Text_ItemDescription;

        UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UImage> Image_Item;
};