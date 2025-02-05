
#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DialogueFactory.generated.h"

UCLASS()
class UDialogueFactory : public UFactory
{
	GENERATED_BODY()
public:
	UDialogueFactory(const FObjectInitializer& ObjectInitializer);
	virtual bool CanCreateNew() const override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};