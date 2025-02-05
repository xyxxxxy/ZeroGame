
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LyraAbilityTask_SimpleTarget.generated.h"

UCLASS()
class LYRAINVENTORY_API ULyraAbilityTask_SimpleTarget : public UAbilityTask
{
	GENERATED_BODY()
public:
	ULyraAbilityTask_SimpleTarget(const FObjectInitializer& ObjectInitializer);
	virtual void Activate() override;
	
	virtual FGameplayAbilityTargetDataHandle GenerateTargetHandle();
	virtual void HandleTargetData(const FGameplayAbilityTargetDataHandle& Data);
	virtual void HandleCancelled();
	virtual void OnTargetDataCallback(const FGameplayAbilityTargetDataHandle& Data,FGameplayTag ActivationTag);
	virtual void OnTargetDataCancelled();

protected:
	FGameplayAbilityTargetDataHandle ServerTargetData;
};
