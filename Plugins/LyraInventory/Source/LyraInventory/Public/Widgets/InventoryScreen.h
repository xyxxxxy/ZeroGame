
#pragma once

#include "CommonActivatableWidget.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "Engine/DataTable.h"
#include "InventoryScreen.generated.h"

class USlotHandleObject;
class UInventoryPanel;
UCLASS(Blueprintable, meta = (Category = Inventory))
class UInventoryScreen : public UCommonActivatableWidget
{
        GENERATED_BODY()
public:
        UInventoryScreen() {}

protected:
        virtual void NativeOnInitialized() override;
        virtual UWidget* NativeGetDesiredFocusTarget() const override;

        UFUNCTION(BlueprintCallable)
        void FilterSlots(FName Id);

        UFUNCTION(BlueprintCallable)
        void AddItemForIdToQuery(FName Id, const FLyraInventoryQuery &Query);

        UFUNCTION(BlueprintCallable)
        void RemoveNavigation();

        // UInventoryPanel* GetInventoryPanel() const{return InventoryPanel;}

        USlotHandleObject *GetSlotHandleObject();

        void HandleConfirmAction();
        void HandleBackAction();
        // void HandleApplyAction();
        // void HandleCancelChangesAction();

        UFUNCTION(BlueprintImplementableEvent)
        void CreateSubWidget();

protected:
        // UPROPERTY(BlueprintReadOnly, Category = Input, meta = (BindWidget, OptionalWidget = true, AllowPrivateAccess = true))
        // TObjectPtr<ULyraTabListWidgetBase> TopSettingsTabs;

        UPROPERTY(EditDefaultsOnly)
        FDataTableRowHandle ConfirmInputActionData;

        UPROPERTY(EditDefaultsOnly)
        FDataTableRowHandle BackInputActionData;


        FUIActionBindingHandle ConfirmHandle;
        FUIActionBindingHandle BackHandle;


private:
        UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UInventoryPanel> InventoryPanel;

        UPROPERTY(Transient)
        TMap<FName, FLyraInventoryQuery> IdToQuery;
};