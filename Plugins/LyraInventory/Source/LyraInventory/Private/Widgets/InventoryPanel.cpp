
#include "Widgets/InventoryPanel.h"
#include "Widgets/InventoryTileView.h"
#include "CommonInputSubsystem.h"
#include "CommonInputTypeEnum.h"

struct FFocusEvent;
struct FGeometry;

UInventoryPanel::UInventoryPanel()
{
        SetIsFocusable(true);
}

FReply UInventoryPanel::NativeOnFocusReceived(const FGeometry &InGeometry, const FFocusEvent &InFocusEvent)
{
        const UCommonInputSubsystem *InputSubsystem = GetInputSubsystem();
        if (InputSubsystem && InputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
        {
                if (TSharedPtr<SWidget> PrimarySlateWidget = TileView_Settings->GetCachedWidget())
                {
                        TileView_Settings->NavigateToIndex(0);
                        TileView_Settings->SetSelectedIndex(0);

                        return FReply::Handled();
                }
        }

        return FReply::Unhandled();
}