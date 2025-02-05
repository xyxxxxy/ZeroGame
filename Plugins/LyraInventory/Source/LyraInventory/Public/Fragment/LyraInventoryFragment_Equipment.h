
#pragma once

#include "CoreMinimal.h"
#include "Inventory/LyraInventoryItemFragment.h"
#include "Equipment/LyraEquipmentTypes.h"
#include "LyraInventoryFragment_Equipment.generated.h"

class UAbilitySystemComponent;

UCLASS()
class LYRAINVENTORY_API ULyraInventoryFragment_Equipment : public ULyraInventoryItemFragment
{
	GENERATED_BODY()
public:
	virtual bool GiveToAbilitySystem(UAbilitySystemComponent* ASC,FLyraAbilitySet_GrantHandles* OutHandleStorage,ULyraInventoryItemInstance* SourceObject) const;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Inventory")
	FLyraItemDefinition_AbilitySet EquipmentAbilitySet;
};
