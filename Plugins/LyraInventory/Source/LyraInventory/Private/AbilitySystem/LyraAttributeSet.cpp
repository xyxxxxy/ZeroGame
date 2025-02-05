
#include "AbilitySystem/LyraAttributeSet.h"
#include "Net/UnrealNetwork.h"

ULyraAttributeSet::ULyraAttributeSet()
{
	InitStrength(100.f);
}

void ULyraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ULyraAttributeSet,Strength,COND_None,REPNOTIFY_Always);
}

void ULyraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetStrengthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,1000.f);
	}
}

void ULyraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void ULyraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
}
