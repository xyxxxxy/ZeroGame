
#include "AbilitySystem/Tasks/LyraAbilityTask_SimpleTarget.h"

#include "AbilitySystemComponent.h"

ULyraAbilityTask_SimpleTarget::ULyraAbilityTask_SimpleTarget(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
}

void ULyraAbilityTask_SimpleTarget::Activate()
{
	FScopedPredictionWindow ScopedWindow(AbilitySystemComponent.Get(),IsPredictingClient());

	FGameplayAbilityTargetDataHandle TargetDataHandle = GenerateTargetHandle();

	if(IsPredictingClient())
	{
		FGameplayTag ActivationTag;
		AbilitySystemComponent->CallServerSetReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			TargetDataHandle,
			ActivationTag,
			AbilitySystemComponent->ScopedPredictionKey);

		HandleTargetData(TargetDataHandle);
	}
	else
	{
		const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

		if(!bIsLocallyControlled)
		{
			AbilitySystemComponent->AbilityTargetDataSetDelegate(GetAbilitySpecHandle(),GetActivationPredictionKey())
			.AddUObject(this,&ULyraAbilityTask_SimpleTarget::OnTargetDataCallback);
			AbilitySystemComponent->AbilityTargetDataCancelledDelegate(GetAbilitySpecHandle(),GetActivationPredictionKey())
			.AddUObject(this,&ULyraAbilityTask_SimpleTarget::OnTargetDataCancelled);

			AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(),GetActivationPredictionKey());

			SetWaitingOnRemotePlayerData();
		}
		else
		{
			FGameplayTag ActivationTag;
			OnTargetDataCallback(TargetDataHandle,ActivationTag);
			EndTask();
		}
	}
}

FGameplayAbilityTargetDataHandle ULyraAbilityTask_SimpleTarget::GenerateTargetHandle()
{
	return  FGameplayAbilityTargetDataHandle();
}

void ULyraAbilityTask_SimpleTarget::HandleTargetData(const FGameplayAbilityTargetDataHandle& Data)
{
}

void ULyraAbilityTask_SimpleTarget::HandleCancelled()
{
}

void ULyraAbilityTask_SimpleTarget::OnTargetDataCallback(const FGameplayAbilityTargetDataHandle& Data,
	FGameplayTag ActivationTag)
{
	HandleTargetData(Data);

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());

	EndTask();
}

void ULyraAbilityTask_SimpleTarget::OnTargetDataCancelled()
{
	HandleCancelled();
	EndTask();
}
