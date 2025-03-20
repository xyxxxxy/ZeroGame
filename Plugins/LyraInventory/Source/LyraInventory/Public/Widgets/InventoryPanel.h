
#pragma once

#include "CommonUserWidget.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "Containers/Ticker.h"
#include "InventoryPanel.generated.h"

struct FFocusEvent;
struct FGeometry;
struct FTimerHandle;
class USlotHandleObject;
class UInventoryTileView;
class UInventorySlotDetailView;
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

        void SetFilterQuery(const FLyraInventoryQuery &InQuery);
        void RefreshList();
        USlotHandleObject *GetSelectedObject() const;
        void RemoveNavigation();

        UUserWidget* GetSelectedEntryWidget();

        // UInventoryTileView *GetInventoryTileView() const {return TileView_Inventory;}
        // UInventorySlotDetailView *GetInventorySlotDetailView() const{return DetailView_Item;}

public:
        DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusedSlotHandleObjectChanged, USlotHandleObject *);
        DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusedSlotHandleObjectChanged_BP, USlotHandleObject *, SlotHandleObject);

        FOnFocusedSlotHandleObjectChanged &OnFocusedSlotHandleObjectChanged() { return OnFocusedSlotHandleObjectChangedNative; }

protected:
        void InitialInventory();
        void FillItemDetails(USlotHandleObject *SlotHandleObject);
        void HandleSettingItemHoveredChanged(UObject *Item, bool bHovered);
        void HandleSettingItemSelectionChanged(UObject *Item);

        // void SelectItems();
private:
        void RefreshNavigation();
private:
        mutable FOnFocusedSlotHandleObjectChanged OnFocusedSlotHandleObjectChangedNative;

        UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Focused SlotHandle Object Changed"), Category = Inventory)
        FOnFocusedSlotHandleObjectChanged_BP OnFocusedSlotHandleObjectChanged_BP;

private:
        UPROPERTY(Transient)
        TArray<TObjectPtr<USlotHandleObject>> SlotHandleObjects;

        UPROPERTY(Transient)
        TObjectPtr<USlotHandleObject> LastHoveredOrSelectedObject;

        UPROPERTY(Transient)
        int32 LastHoveredOrSelectedObjectIndex = 0;

        UPROPERTY(Transient)
        TObjectPtr<ULyraInventoryComponent> Inventory;

        UPROPERTY(Transient)
        FLyraInventoryQuery FilterQuery;

        FTimerHandle NavigationHandle;

private:
        UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UInventoryTileView> TileView_Inventory;

        UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UInventorySlotDetailView> DetailView_Item;

private:
        FTSTicker::FDelegateHandle RefreshHandle;
};