
#include "Widgets/InventoryPanel.h"
#include "Widgets/InventoryTileView.h"
#include "Widgets/InventorySlotDetailView.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "Inventory/SlotHandleObject.h"
#include "Inventory/LyraInventoryComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"

struct FFocusEvent;
struct FGeometry;

UInventoryPanel::UInventoryPanel()
{
        SetIsFocusable(true);
}

void UInventoryPanel::NativeOnInitialized()
{
        Super::OnInitialized();

        TileView_Inventory->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::HandleSettingItemHoveredChanged);
        TileView_Inventory->OnItemSelectionChanged().AddUObject(this, &ThisClass::HandleSettingItemSelectionChanged);

        InitialInventory();
}

void UInventoryPanel::NativeConstruct()
{
        Super::NativeConstruct();
}

FReply UInventoryPanel::NativeOnFocusReceived(const FGeometry &InGeometry, const FFocusEvent &InFocusEvent)
{
        const UCommonInputSubsystem *InputSubsystem = GetInputSubsystem();
        if (InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
        {
                if (TSharedPtr<SWidget> PrimarySlateWidget = TileView_Inventory->GetCachedWidget())
                {
                        TileView_Inventory->NavigateToIndex(0);
                        TileView_Inventory->SetSelectedIndex(0);

                        return FReply::Handled();
                }
        }
        return FReply::Unhandled();
}

void UInventoryPanel::InitialInventory()
{
        if (APlayerController *PC = GetOwningPlayer())
        {
                if (APawn *Pawn = PC->GetPawn())
                {
                        ULyraInventoryComponent *InventoryComp = Cast<ULyraInventoryComponent>(Pawn->GetComponentByClass(ULyraInventoryComponent::StaticClass()));
                        // ULyraInventoryComponent *InventoryComp = ULyraInventoryComponent::FindInventoryComponent(Pawn);
                        ensure(InventoryComp);
                        if (Inventory != InventoryComp)
                        {
                                Inventory = InventoryComp;
                        }
                }
        }
}
void UInventoryPanel::RefreshList()
{
        // if (RefreshHandle.IsValid())
        // {
        // 	return;
        // }
        // InitialInventory();

        // SetIsFocusable(false);
        SlotHandleObjects.Empty();
        TArray<FLyraInventoryItemSlotHandle> OutSlotHandles;
        Inventory->Query_GetAllSlotHandles(FilterQuery, OutSlotHandles);

        for (const FLyraInventoryItemSlotHandle &SlotHandle : OutSlotHandles)
        {
                USlotHandleObject *Object = NewObject<USlotHandleObject>(this);
                Object->Setpayload(FSlotHandleObjectPayload(SlotHandle));
                SlotHandleObjects.Add(Object);
        }

        TileView_Inventory->SetListItems(SlotHandleObjects);

        // If the list directly has the focus, instead of a child widget, then it's likely the panel and items
        // were not yet available when we received focus, so lets go ahead and focus the first item now.
        // if (HasUserFocus(GetOwningPlayer()))
        // SetIsFocusable(true);
        for (UUserWidget *Widget : TileView_Inventory->GetDisplayedEntryWidgets())
        {
                Widget->SetIsFocusable(true);
        }
        TileView_Inventory->NavigateToIndex(0);
        TileView_Inventory->SetSelectedIndex(0);
}
void UInventoryPanel::SetFilterQuery(const FLyraInventoryQuery &InQuery)
{
        FilterQuery = InQuery;
        RefreshList();
}

USlotHandleObject *UInventoryPanel::GetSelectedObject() const
{
        return Cast<USlotHandleObject>(TileView_Inventory->GetSelectedItem());
}

void UInventoryPanel::RemoveNavigation()
{
        for (UUserWidget *Widget : TileView_Inventory->GetDisplayedEntryWidgets())
        {
                Widget->SetIsFocusable(false);
        }
        // TileView_Inventory->NavigateToIndex(0);
        // TileView_Inventory->SetSelectedIndex(0);
}

void UInventoryPanel::HandleSettingItemHoveredChanged(UObject *Item, bool bHovered)
{
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("HandleSettingItemHoveredChanged"));

        USlotHandleObject *SlotHandleObject = bHovered ? Cast<USlotHandleObject>(Item) : ToRawPtr(LastHoveredOrSelectedObject);
        if (bHovered && SlotHandleObject)
        {
                LastHoveredOrSelectedObject = SlotHandleObject;
        }
        FillItemDetails(SlotHandleObject);
}

void UInventoryPanel::HandleSettingItemSelectionChanged(UObject *Item)
{
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("HandleSettingItemSelectionChanged"));

        USlotHandleObject *SlotHandleObject = Cast<USlotHandleObject>(Item);
        if (SlotHandleObject)
        {
                LastHoveredOrSelectedObject = SlotHandleObject;
        }

        FillItemDetails(SlotHandleObject);
}

void UInventoryPanel::FillItemDetails(USlotHandleObject *SlotHandleObject)
{
        if (DetailView_Item)
        {
                DetailView_Item->FillItemDetails(SlotHandleObject);
        }
        OnFocusedSlotHandleObjectChanged().Broadcast(SlotHandleObject);
        OnFocusedSlotHandleObjectChanged_BP.Broadcast(SlotHandleObject);
}