
#include "Widgets/InventoryScreen.h"
#include "Widgets/InventoryPanel.h"

void UInventoryScreen::FilterSlots(FName Id)
{
        if(IdToQuery.Contains(Id))
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