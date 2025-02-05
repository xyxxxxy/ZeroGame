
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LyraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class LYRAINVENTORY_API ULyraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	ULyraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Strength,Category = "Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(ULyraAttributeSet,Strength)

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;	
};
