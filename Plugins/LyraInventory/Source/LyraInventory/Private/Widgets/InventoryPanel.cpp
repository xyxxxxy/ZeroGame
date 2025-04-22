
#include "Widgets/InventoryPanel.h"
#include "Widgets/InventoryTileView.h"
#include "Widgets/InventorySlotDetailView.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"
#include "Inventory/SlotHandleObject.h"
#include "Inventory/LyraInventoryComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"
#include "Engine/TimerHandle.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
struct FFocusEvent;
struct FGeometry;

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryPanel)

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


        if (LastHoveredOrSelectedObject)
        {
                LastHoveredOrSelectedObjectIndex = TileView_Inventory->GetIndexForItem(LastHoveredOrSelectedObject);
        }
        else
        {
                LastHoveredOrSelectedObjectIndex = 0;
        }

        
        TArray<FLyraInventoryItemSlotHandle> OutSlotHandles;
        Inventory->Query_GetAllSlotHandles(FilterQuery, OutSlotHandles);
        SlotHandleObjects.Empty(OutSlotHandles.Num() + 1);
        for (const FLyraInventoryItemSlotHandle &SlotHandle : OutSlotHandles)
        {
                // TODO : outer?
                USlotHandleObject *Object = NewObject<USlotHandleObject>(this);
                Object->Setpayload(FSlotHandleObjectPayload(SlotHandle));
                SlotHandleObjects.Add(Object);
        }

        TileView_Inventory->SetListItems(SlotHandleObjects);

        // UWidgetBlueprintLibrary::SetFocusToGameViewport();
        // TileView_Inventory->ClearSelection();

        int32 SlotNum = TileView_Inventory->GetNumItems();
        if (SlotNum > 0)
        {
                if (LastHoveredOrSelectedObjectIndex > SlotNum - 1)
                {
                        LastHoveredOrSelectedObjectIndex = SlotNum - 1;
                }
                // If the list directly has the focus, instead of a child widget, then it's likely the panel and items
                // were not yet available when we received focus, so lets go ahead and focus the first item now.
                // if (HasUserFocus(GetOwningPlayer()))
                //GetWorld()->GetTimerManager().SetTimer(NavigationHandle, this, &UInventoryPanel::RefreshNavigation, 1.0f, false);
                TileView_Inventory->NavigateToIndex(LastHoveredOrSelectedObjectIndex);
                TileView_Inventory->SetSelectedIndex(LastHoveredOrSelectedObjectIndex);
        }
        else
        {
                TileView_Inventory->NavigateToIndex(0);
                TileView_Inventory->SetSelectedIndex(0);
        }

        // if(TileView_Inventory->IsRefreshPending())
        // {
        //         GetWorld()->GetTimerManager().SetTimer(NavigationHandle, this, &UInventoryPanel::RefreshNavigation, 0.2f, false);

        // }
}

void UInventoryPanel::RefreshNavigation()
{
        // TileView_Inventory->ClearSelection();
        //TileView_Inventory->SetFocus();
        // TileView_Inventory->NavigateToIndex(LastHoveredOrSelectedObjectIndex);
        // TileView_Inventory->SetSelectedIndex(LastHoveredOrSelectedObjectIndex);
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

UUserWidget *UInventoryPanel::GetSelectedEntryWidget()
{
        if (TileView_Inventory)
        {
                return TileView_Inventory->GetEntryWidgetFromItem<UUserWidget>(LastHoveredOrSelectedObject);
        }
        return nullptr;
}

void UInventoryPanel::RefreshItem(USlotHandleObject* SlotHandleObject)
{
        TileView_Inventory->SetSelectedItem(SlotHandleObject);
}

void UInventoryPanel::RemoveNavigation()
{
        // for (UUserWidget *Widget : TileView_Inventory->GetDisplayedEntryWidgets())
        // {
        //         Widget->SetIsFocusable(false);
        // }
        // TileView_Inventory->NavigateToIndex(-1);
        // TileView_Inventory->SetSelectedIndex(-1);
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
        // GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("Current Slot Index :%d"), TileView_Inventory->GetIndexForItem(LastHoveredOrSelectedObject)));
        if (DetailView_Item)
        {
                DetailView_Item->FillItemDetails(SlotHandleObject);
        }
        OnFocusedSlotHandleObjectChanged().Broadcast(SlotHandleObject);
        OnFocusedSlotHandleObjectChanged_BP.Broadcast(SlotHandleObject);
}