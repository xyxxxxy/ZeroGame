#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "CommonUserWidget.h"

#include "InventoryListEntry.generated.h"

struct FGeometry;
struct FFocusEvent;
class USlotHandleObject;

UCLASS(Abstract, NotBlueprintable, meta = (Category = "Inventory", DisableNativeTick))
class LYRAINVENTORY_API UInventorySlotEntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

// public:
// 	virtual void SetSetting(UGameSetting* InSetting);
// 	virtual void SetDisplayNameOverride(const FText& OverrideName);
//         virtual void OnSettingChanged();
// 	virtual void HandleEditConditionChanged(UGameSetting* InSetting);
// 	virtual void RefreshEditableState(const FGameSettingEditableState& InEditableState);

public:
	virtual void SetSlotHandleObjects(USlotHandleObject* InObject);
protected:
	virtual void NativeOnEntryReleased() override;

	
protected:
	// Focus transitioning to subwidgets for the gamepad
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetPrimaryGamepadFocusWidget();

protected:
	bool bSuspendChangeUpdates = false;

	UPROPERTY(Transient)
	TObjectPtr<USlotHandleObject> SlotHandleObject;

	FText DisplayNameOverride = FText::GetEmpty();

private:
// 	void HandleSettingChanged(UGameSetting* InSetting, EGameSettingChangeReason Reason);
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UUserWidget> Background;
};


UCLASS(Abstract, Blueprintable, meta = (Category = "Inventory", DisableNativeTick))
class LYRAINVENTORY_API UInventorySlotEntry : public UInventorySlotEntryBase
{
	GENERATED_BODY()
public:
	virtual void SetSlotHandleObjects(USlotHandleObject* InObject) override;

};

