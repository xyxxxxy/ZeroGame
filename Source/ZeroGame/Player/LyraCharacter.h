#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTags.h"
#include "InputActionValue.h"
#include "InputTriggers.h"
#include "ModularCharacter.h"
#include "LyraCharacter.generated.h"


struct FEnhancedInputActionEventBinding;
class ULyraInputActionMap;
class UInputMappingContext;
class UCommonActivatableWidget;
class ULyraHUDLayout;
class ULyraInventoryComponent;

/**
 * Player Pawn
 */
UCLASS(meta=(Category=Lyra))
class ALyraCharacter : public AModularCharacter
{
	GENERATED_BODY()

public:
	ALyraCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~APawn interface
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//~End of APawn interface

protected:
	template<class UserClass, typename... VarTypes>
	void BindInputValueAction(UEnhancedInputComponent* EnhancedInputComponent, FGameplayTag InputTag, ETriggerEvent TriggerEvent, UserClass* Object, typename FEnhancedInputActionHandlerValueSignature::template TMethodPtr< UserClass, VarTypes... > Func, bool bLogIfNotFound, VarTypes... Vars);

	void UnbindInputValueActions();

	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Move(const FInputActionValue& InputActionValue);

	void Input_Inventory(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "PushInventoryWidget"))
	void K2_PushInventoryWidget();

	void PushHUDWidget(const FInputActionValue& InputActionValue);

protected:
	/** The HUD Layout widget to use (must be derived from Lyra HUD Layout) */
	UPROPERTY(EditDefaultsOnly, DisplayName="HUD Layout Class", Category = Widget, AdvancedDisplay)
	TSubclassOf<ULyraHUDLayout> HUDLayoutClass;

	/** Used to keep track of the widget that was created to be our HUD */
	UPROPERTY(Transient, VisibleInstanceOnly)
	TWeakObjectPtr<UCommonActivatableWidget> HUDLayoutWidget;

	/** Array of IMC to use while this Pawn is possessed */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TArray<TObjectPtr<const UInputMappingContext>> InputMappingContexts;

	/** Map of InputActions to C++ GameplayTags */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<const ULyraInputActionMap> InputActionMap;

	
private:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Inventory")
	TObjectPtr<ULyraInventoryComponent> InventoryComponent;

	UPROPERTY(Transient, VisibleInstanceOnly)
	TSet<int32> InputEventBindingHandles;
	
};