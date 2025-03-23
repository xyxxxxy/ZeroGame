
#pragma once

#include "CoreMinimal.h"
#include "LyraInventoryProcessor.h"
#include "LyraInventoryProcessor_Equipment.generated.h"

class ULyraInventoryComponent;

UCLASS()
class ULyraInventoryProcessor_Equipment : public ULyraInventoryProcessor
{
        GENERATED_BODY()

public:
        ULyraInventoryProcessor_Equipment();

        // This Equipment processor only runs on slots matching this query
        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
        FGameplayTagQuery MatchingSlotQuery;

        // Query for finding a fragment that contains the ability info we need on the item that is in this slot
        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
        FGameplayTagQuery AbilityInfoFragmentQuery;

        void OnItemSlotChange_Implementation(const FLyraInventoryItemSlotHandle& SlotHandle, ULyraInventoryItemInstance* Item, ULyraInventoryItemInstance* PreviousItem) override;

        virtual void BindToASC_Implementation() override;
        virtual void UnbindFromASC_Implementation() override;
};
