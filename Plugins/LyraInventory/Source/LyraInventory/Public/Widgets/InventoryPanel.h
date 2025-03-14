
#pragma once

#include "CommonUserWidget.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "Containers/Ticker.h"
#include "InventoryPanel.generated.h"

struct FFocusEvent;
struct FGeometry;
class USlotHandleObject;
class UInventoryTileView;
class ULyraInventoryComponent;

UCLASS(Abstract)
class LYRAINVENTORY_API UInventoryPanel : public UCommonUserWidget
{
        GENERATED_BODY()

public:
        UInventoryPanel();

        virtual void NativeOnInitialized() override;
        virtual void NativeConstruct() override;

        virtual FReply NativeOnFocusReceived(const FGeometry &InGeometry, const FFocusEvent &InFocusEvent) override;

        void SetFilterQuery(const FLyraInventoryQuery& InQuery);

protected:
        void InitialInventory();
        void RefreshList();
        
        USlotHandleObject* GetSelectedObject() const; 
        void SelectItems();
private:
        void HandleSettingItemHoveredChanged(UObject *Item, bool bHovered);
        void HandleSettingItemSelectionChanged(UObject *Item);

private:
        UPROPERTY(Transient)
        TArray<TObjectPtr<USlotHandleObject>> SlotHandleObjects;

        UPROPERTY(Transient)
        TObjectPtr<ULyraInventoryComponent> Inventory;

        FLyraInventoryQuery FilterQuery;

private:
        UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UInventoryTileView> TileView_Inventory;

        private:
	FTSTicker::FDelegateHandle RefreshHandle;
};