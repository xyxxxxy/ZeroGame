
#include "Widgets/InventoryPanel.h"
#include "Widgets/InventoryTileView.h"
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
                        //ULyraInventoryComponent *InventoryComp = ULyraInventoryComponent::FindInventoryComponent(Pawn);
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
        //InitialInventory();
        SlotHandleObjects.Empty();
        TArray<FLyraInventoryItemSlotHandle> OutSlotHandles;
        Inventory->Query_GetAllSlotHandles(FilterQuery, OutSlotHandles);
        for(const FLyraInventoryItemSlotHandle& SlotHandle : OutSlotHandles)
        {
                USlotHandleObject* Object = NewObject<USlotHandleObject>(this);
                Object->Setpayload(FSlotHandleObjectPayload(SlotHandle));
                SlotHandleObjects.Add(Object);
        }
        TileView_Inventory->SetListItems(SlotHandleObjects);

}
void UInventoryPanel::SetFilterQuery(const FLyraInventoryQuery& InQuery)
{
        FilterQuery = InQuery;
        RefreshList();
}

USlotHandleObject* UInventoryPanel::GetSelectedObject() const
{
        return Cast<USlotHandleObject>(TileView_Inventory->GetSelectedItem());
}
void UInventoryPanel::HandleSettingItemHoveredChanged(UObject *Item, bool bHovered)
{
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, TEXT("HandleSettingItemHoveredChanged"));
}

void UInventoryPanel::HandleSettingItemSelectionChanged(UObject *Item)
{
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("HandleSettingItemSelectionChanged"));
}
