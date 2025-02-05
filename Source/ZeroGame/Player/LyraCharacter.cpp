
#include "LyraCharacter.h"

#include "UI/Foundation/LyraHUDLayout.h"
#include "CommonUIExtensions.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LyraGameplayTags.h"
#include "LyraInputConfig.h"
#include "LyraLog.h"
#include "Core/LyraGameState.h"
#include "LyraHelpers.h"
#include "Inventory/LyraInventoryComponent.h"

// Sets default values
ALyraCharacter::ALyraCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	InventoryComponent = CreateDefaultSubobject<ULyraInventoryComponent>(TEXT("InventoryComponent"));

	// Lyra is an RTS style input by default, so player pawn should never affect navigation
	bCanAffectNavigationGeneration = false;
	
}


void ALyraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// We expect that this *player* pawn will be possessed by players, but who knows...
	const APlayerController* PC = Cast<APlayerController>(NewController);
	if (ensure(PC))
	{
		// Add HUD Layout widget to the player's Game UI Layer
		Lyra_LOG(TEXT("Pushing Game HUD [%s] to UI"), *GetNameSafe(HUDLayoutClass));
		//HUDLayoutWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(PC->GetLocalPlayer(), LyraTag::UI_Layer_Game, HUDLayoutClass);

		// Add EnhancedInput IMCs
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULyraHelpers::GetEnhancedInputLocalPlayerSubsystem(PC))
		{
			Lyra_LOG(TEXT("Clearing all input mappings"));
			Subsystem->ClearAllMappings();

			// Apply the array of IMCs in descending priority
			int32 Priority = InputMappingContexts.Num();
			for (const TObjectPtr<const UInputMappingContext>& Imc : InputMappingContexts)
			{
				// Don't store empty IMCs in Data Assets!
				if (ensure(Imc != nullptr))
				{
					Lyra_LOG(TEXT("Adding IMC [%s] with priority %i"), *GetNameSafe(Imc), Priority);

					FModifyContextOptions Options = {};
					Options.bIgnoreAllPressedKeysUntilRelease = false;

					Subsystem->AddMappingContext(Imc, Priority, Options);
					Priority--;
				}
			}
		}
	}
}


void ALyraCharacter::UnPossessed()
{
	// Remove any HUD we added to the player's UI
	if (HUDLayoutWidget.IsValid())
	{
		Lyra_LOG(TEXT("Cleaning up HUD Layout Widget"));
		UCommonUIExtensions::PopContentFromLayer(HUDLayoutWidget.Get());
		HUDLayoutWidget.Reset();
	}

	// Remove any EnhancedInput IMCs
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULyraHelpers::GetEnhancedInputLocalPlayerSubsystem(GetController<APlayerController>()))
	{
		//Lyra_LOG(TEXT("Clearing input mappings"));
		Subsystem->ClearAllMappings();
	}

	// Unbind from EnhancedInput
	UnbindInputValueActions();

	Super::UnPossessed();
}


void ALyraCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// You **MUST** set InputActionMap or your pawn won't have any inputs!
	if (!ensure(IsValid(InputActionMap)))
	{
		Lyra_ERROR_LOG(TEXT("InputActionMap is not configured for player pawn; player pawn will have no inputs"));
		return;
	}

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (ensure(EnhancedInput))
	{
		constexpr bool bLogNotFound = true;

		// Look and Zoom should update every tick while Triggered
		BindInputValueAction(EnhancedInput, LyraTag::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look, bLogNotFound);
		BindInputValueAction(EnhancedInput, LyraTag::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, bLogNotFound);
		
		// Start/Stop Time only on completed input actions, not on pending actions
		BindInputValueAction(EnhancedInput, LyraTag::InputTag_OpenMenu, ETriggerEvent::Completed, this, &ThisClass::PushHUDWidget, bLogNotFound);
		BindInputValueAction(EnhancedInput, LyraTag::InputTag_Inventory, ETriggerEvent::Completed, this, &ThisClass::Input_Inventory, bLogNotFound);
	}
}


template <class UserClass, typename ... VarTypes>
void ALyraCharacter::BindInputValueAction(UEnhancedInputComponent* EnhancedInputComponent, FGameplayTag InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FEnhancedInputActionHandlerValueSignature::TMethodPtr<UserClass, VarTypes...> Func, bool bLogIfNotFound, VarTypes... Vars)
{
	// The pawn **MUST** have an InputActionMap configured so we can map input actions to gameplay tags
	if (ensure(InputActionMap))
	{
		if (const UInputAction* Action = InputActionMap->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
		{
			const FEnhancedInputActionEventBinding& Binding = EnhancedInputComponent->BindAction(Action, TriggerEvent, Object, Func, Vars...);
			InputEventBindingHandles.Add(Binding.GetHandle());

			Lyra_WARNING_LOG(TEXT("Bound InputAction [%s] to [%s] handle=%i"), *GetNameSafe(Action), *InputTag.ToString(), Binding.GetHandle());
		}
	}
}


void ALyraCharacter::UnbindInputValueActions()
{
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!ensure(EnhancedInput))
	{
		//Lyra_LOG(TEXT("Cannot unbind inputs without an EnhancedInputComponent"));
		return;
	}

	for (const int32& Handle : InputEventBindingHandles)
	{
		const bool bRemoved = EnhancedInput->RemoveActionEventBinding(Handle);
		if (!bRemoved)
		{
			//Lyra_VERBOSE_LOG(TEXT("Failed to remove action event binding %i"), Handle);
		}
	}

	InputEventBindingHandles.Reset();
}


void ALyraCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void ALyraCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// input is a Vector2D
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALyraCharacter::Input_Inventory(const FInputActionValue& InputActionValue)
{
	K2_PushInventoryWidget();
}

void ALyraCharacter::PushHUDWidget(const FInputActionValue& InputActionValue)
{
	const APlayerController* PC = Cast<APlayerController>(Controller);
	if(PC)
	{
//		HUDLayoutWidget = UCommonUIExtensions::PushContentToLayer_ForPlayer(PC->GetLocalPlayer(), LyraTag::UI_Layer_Game, HUDLayoutClass);
	}
}