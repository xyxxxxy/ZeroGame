
#include "Widgets/InventoryScreen.h"
#include "Widgets/InventoryPanel.h"
#include "Input/CommonUIInputTypes.h"
#include "Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryScreen)

void UInventoryScreen::NativeOnInitialized()
{
        Super::NativeOnInitialized();

        ConfirmHandle = RegisterUIActionBinding(FBindUIActionArgs(ConfirmInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleConfirmAction)));
        BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
}

UWidget *UInventoryScreen::NativeGetDesiredFocusTarget() const
{
        if (UWidget *Target = BP_GetDesiredFocusTarget())
        {
                return Target;
        }

        return InventoryPanel;
}

void UInventoryScreen::FilterSlots(FName Id)
{
        if (IdToQuery.Contains(Id))
        {
                InventoryPanel->SetFilterQuery(IdToQuery[Id]);
        }
}

void UInventoryScreen::AddItemForIdToQuery(FName Id, const FLyraInventoryQuery &Query)
{
        if (!IdToQuery.Contains(Id))
        {
                IdToQuery.Emplace(Id, Query);
        }
}

void UInventoryScreen::RemoveNavigation()
{
        if (InventoryPanel)
        {
                InventoryPanel->RemoveNavigation();
        }
}

USlotHandleObject *UInventoryScreen::GetSlotHandleObject()
{
        if (InventoryPanel)
        {
                return InventoryPanel->GetSelectedObject();
        }
        return nullptr;
}

void UInventoryScreen::HandleConfirmAction()
{
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("HandleConfirmAction"));
        CreateSubWidget();
}

void UInventoryScreen::HandleBackAction()
{
        DeactivateWidget();
}