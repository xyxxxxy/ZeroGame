
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "LyraInventoryAttributeSet.generated.h"

UCLASS()
class LYRAINVENTORY_API ULyraInventoryAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	ULyraInventoryAttributeSet(const class FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty> & OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_InventorySize_Weapon)
	FGameplayAttributeData InventorySize_Weapon;

	UPROPERTY(ReplicatedUsing = OnRep_InventorySize_Prop)
	FGameplayAttributeData InventorySize_Prop;

	UFUNCTION()
	void OnRep_InventorySize_Weapon(FGameplayAttributeData OldInventorySize_Weapon);

	UFUNCTION()
	void OnRep_InventorySize_Prop(FGameplayAttributeData OldInventorySize_Prop);
	
	static FGameplayAttribute InventorySizeWeaponAttribute();
	static FGameplayAttribute InventorySizePropAttribute();
};
