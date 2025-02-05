
#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "LyraEquipmentTypes.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Abilities)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Abilities)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraAbilitySet_GameEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Abilities)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = Abilities)
	float EffectLevel = 1.f;
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraAbilitySet_GrantHandles
{
	GENERATED_BODY()
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddEffectSpecHandle(const FActiveGameplayEffectHandle& Handle);
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);

protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraItemDefinition_AbilitySet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<FLyraAbilitySet_GameplayAbility> GrantAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<FLyraAbilitySet_GameEffect> GrantEffects;
};

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryItemEntry
{
	GENERATED_BODY()
public:
	FLyraInventoryItemEntry(){}
	FLyraInventoryItemEntry(const FLyraInventoryItemSlotHandle InHandle)
	:Handle(InHandle)
	{
		
	}

	UPROPERTY(BlueprintReadOnly, Category = Abilities)
	FLyraInventoryItemSlotHandle Handle;

	UPROPERTY(BlueprintReadOnly, Category = Abilities)
	FLyraAbilitySet_GrantHandles AbilityGrantHandles;

	bool operator==(const FLyraInventoryItemEntry& Entry) const{return Entry.Handle == this->Handle;}
	bool operator==(const FLyraInventoryItemEntry* Entry) const{return Entry->Handle == this->Handle;}
};
