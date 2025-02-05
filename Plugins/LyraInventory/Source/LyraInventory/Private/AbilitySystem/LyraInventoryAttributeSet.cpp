
#include "AbilitySystem/LyraInventoryAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

ULyraInventoryAttributeSet::ULyraInventoryAttributeSet(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
,InventorySize_Weapon(12), InventorySize_Prop(12)
{
}

void ULyraInventoryAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULyraInventoryAttributeSet, InventorySize_Weapon, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ULyraInventoryAttributeSet, InventorySize_Prop, COND_None, REPNOTIFY_Always);
}

void ULyraInventoryAttributeSet::OnRep_InventorySize_Weapon(FGameplayAttributeData OldInventorySize_Weapon)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULyraInventoryAttributeSet, InventorySize_Weapon, OldInventorySize_Weapon);
}

void ULyraInventoryAttributeSet::OnRep_InventorySize_Prop(FGameplayAttributeData OldInventorySize_Prop)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ULyraInventoryAttributeSet, InventorySize_Prop, OldInventorySize_Prop);
}

FGameplayAttribute ULyraInventoryAttributeSet::InventorySizeWeaponAttribute()
{
	static FProperty* Property = FindFProperty<FProperty>(
		ULyraInventoryAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(ULyraInventoryAttributeSet, InventorySize_Weapon));
	return FGameplayAttribute(Property);
}

FGameplayAttribute ULyraInventoryAttributeSet::InventorySizePropAttribute()
{
	static FProperty* Property = FindFProperty<FProperty>(
	ULyraInventoryAttributeSet::StaticClass(),
	GET_MEMBER_NAME_CHECKED(ULyraInventoryAttributeSet, InventorySize_Prop));
	return FGameplayAttribute(Property);
}
