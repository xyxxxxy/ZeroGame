
#include "Processors/LyraInventoryProcessor_Equipment.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "LyraInventoryLibrary.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "Inventory/LyraInventoryComponent.h"

ULyraInventoryProcessor_Equipment::ULyraInventoryProcessor_Equipment()
{
}

void ULyraInventoryProcessor_Equipment::OnItemSlotChange_Implementation(const FLyraInventoryItemSlotHandle &SlotHandle, ULyraInventoryItemInstance *Item, ULyraInventoryItemInstance *PreviousItem)
{
        UAbilitySystemComponent *ASC = GetOwningAbilitySystemComponent();
        check(ASC);
        if (IsValid(PreviousItem))
        {
                PreviousItem->GrantedHandles.TakeFromAbilitySystem(ASC);
        }

        if (IsValid(Item))
        {
                FLyraInventoryTableRowFragment_AbilitySet Fragment_AbilitySet;
                if (ULyraInventoryLibrary::FindFragment_AbilitySet(Item, Fragment_AbilitySet))
                {
                        Fragment_AbilitySet.AbilitySet->GiveToAbilitySystem(ASC, &Item->GrantedHandles, Item);
                }
        }
}

void ULyraInventoryProcessor_Equipment::BindToASC_Implementation()
{
        UAbilitySystemComponent *ASC = GetOwningAbilitySystemComponent();
        check(ASC);
        TArray<FLyraInventoryItemSlotHandle> SlotHandles;
        FLyraInventoryQuery Query;
        Query.SlotTypeQuery = MatchingSlotQuery;
        GetOwningInventory()->Query_GetAllSlotHandles(Query, SlotHandles);

        for (const FLyraInventoryItemSlotHandle &SlotHandle : SlotHandles)
        {
                if (ULyraInventoryItemInstance *Item = GetOwningInventory()->GetItemInstanceBySlot(SlotHandle))
                {
                        FLyraInventoryTableRowFragment_AbilitySet Fragment_AbilitySet;
                        if (ULyraInventoryLibrary::FindFragment_AbilitySet(Item, Fragment_AbilitySet))
                        {
                                Fragment_AbilitySet.AbilitySet->GiveToAbilitySystem(ASC, &Item->GrantedHandles, Item);
                        }
                }
        }
}
void ULyraInventoryProcessor_Equipment::UnbindFromASC_Implementation()
{
        UAbilitySystemComponent *ASC = GetOwningAbilitySystemComponent();
        check(ASC);
        TArray<FLyraInventoryItemSlotHandle> SlotHandles;
        FLyraInventoryQuery Query;
        Query.SlotTypeQuery = MatchingSlotQuery;
        GetOwningInventory()->Query_GetAllSlotHandles(Query, SlotHandles);

        for (const FLyraInventoryItemSlotHandle &SlotHandle : SlotHandles)
        {
                if (ULyraInventoryItemInstance *Item = GetOwningInventory()->GetItemInstanceBySlot(SlotHandle))
                {

                        Item->GrantedHandles.TakeFromAbilitySystem(ASC);
                }
        }
}