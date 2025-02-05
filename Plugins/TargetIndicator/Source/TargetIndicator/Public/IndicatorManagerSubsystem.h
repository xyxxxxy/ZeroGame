
#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "IndicatorManagerSubsystem.generated.h"

class UIndicatorWidgetBase;

UCLASS()
class TARGETINDICATOR_API UIndicatorManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UIndicatorManagerSubsystem(){}

    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Indicator")
    void StartIndicator(const TArray<AActor*>& TargetActors, TSubclassOf<UIndicatorWidgetBase> DefaultWidgetClass);

    UFUNCTION(BlueprintCallable, Category = "Indicator")
    void RemoveIndicator(AActor* TargetActor);

    UFUNCTION(BlueprintCallable, Category = "Indicator")
    void RemoveAllIndicator();

private:
    UPROPERTY(Transient)
    TMap<TObjectPtr<AActor>, TObjectPtr<UIndicatorWidgetBase>> Widgets;
};