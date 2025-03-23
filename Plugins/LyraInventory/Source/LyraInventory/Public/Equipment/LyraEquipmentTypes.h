
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "ActiveGameplayEffectHandle.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "LyraEquipmentTypes.generated.h"

class UGameplayAbility;
class UAttributeSet;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraAbilitySet_PassiveGameplayEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	TSubclassOf<UGameplayEffect> PassiveGameplayEffect = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	float EffectLevel = 1.f;
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraAbilitySet_ActiveGameplayEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	TSubclassOf<UGameplayEffect> ActiveGameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	float EffectLevel = 1.f;
};

USTRUCT(BlueprintType)
struct FLyraAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

// 存放于ULyraInventoryItemInstance，管理GA，GE，AS
USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraAbilitySet_GrantedHandles
{
	GENERATED_BODY()
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle &Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle &Handle);
	void TakeFromAbilitySystem(UAbilitySystemComponent *ASC);
	void AddAttributeSet(UAttributeSet *Set);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	UPROPERTY()
	TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

/**
 * ULyraAbilitySet
 *
 *	Non-mutable data asset used to grant gameplay abilities and gameplay effects.
 */
UCLASS(BlueprintType, Const)
class ULyraAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	ULyraAbilitySet(const FObjectInitializer &ObjectInitializer = FObjectInitializer::Get());

	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	void GiveToAbilitySystem(UAbilitySystemComponent *ASC, FLyraAbilitySet_GrantedHandles *OutGrantedHandles, UObject *SourceObject = nullptr) const;

protected:
	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FLyraAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<FLyraAbilitySet_PassiveGameplayEffect> GrantedPassiveGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<FLyraAbilitySet_ActiveGameplayEffect> GrantedActiveGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty = AttributeSet))
	TArray<FLyraAbilitySet_AttributeSet> GrantedAttributes;
};




//-----------------------------------------------------------------
//----------------------DEPRECATED-------------------------------
USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraItemDefinition_AbilitySet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<FLyraAbilitySet_GameplayAbility> GrantAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<FLyraAbilitySet_PassiveGameplayEffect> GrantEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<FLyraAbilitySet_AttributeSet> GrantAttributeSets;
};

// USTRUCT(BlueprintType)
// struct LYRAINVENTORY_API FLyraInventoryItemEntry
// {
// 	GENERATED_BODY()
// public:
// 	FLyraInventoryItemEntry() {}
// 	FLyraInventoryItemEntry(const FLyraInventoryItemSlotHandle InHandle)
// 	    : Handle(InHandle)
// 	{
// 	}

// 	UPROPERTY(BlueprintReadOnly, Category = Abilities)
// 	FLyraInventoryItemSlotHandle Handle;

// 	UPROPERTY(BlueprintReadOnly, Category = Abilities)
// 	FLyraAbilitySet_GrantedHandles AbilityGrantedHandles;

// 	bool operator==(const FLyraInventoryItemEntry &Entry) const { return Entry.Handle == this->Handle; }
// 	bool operator==(const FLyraInventoryItemEntry *Entry) const { return Entry->Handle == this->Handle; }
// };