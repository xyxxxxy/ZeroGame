#pragma once

#include "CoreMinimal.h"
#include "LyraHelpers.generated.h"


class UEnhancedInputLocalPlayerSubsystem;


/**
 * LyraHelpers
 *
 * Static helper methods for C++ and Blueprints
 */
UCLASS()
class ULyraHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    ULyraHelpers() {}

    /**
     * Get the EnhancedInput LocalPlayer Subsystem for the player represented by PlayerController
     * @param PlayerController
     * @return nullptr, or if available, the EnhancedInput LocalPlayer Subsystem for the player
     */
    UFUNCTION(BlueprintCallable)
    static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem(const APlayerController* PlayerController);

	/**
	 * Used by BP (LogObject) to log the message LogMessage to the Error log
	 * @param LogObject Plug a reference to self into this, to identify the BP that is logging
	 * @param LogMessage Whatever you want to write to the log
	 */
	UFUNCTION(BlueprintCallable)
	static void ErrorLog(UObject* LogObject, FString LogMessage);

	/**
	 * Used by BP (LogObject) to log the message LogMessage to the Warning log
	 * @param LogObject Plug a reference to self into this, to identify the BP that is logging
	 * @param LogMessage Whatever you want to write to the log
	 */
	UFUNCTION(BlueprintCallable)
	static void WarningLog(UObject* LogObject, FString LogMessage);

	/**
	 * Used by BP (LogObject) to log the message LogMessage to the output log
	 * @param LogObject Plug a reference to self into this, to identify the BP that is logging
	 * @param LogMessage Whatever you want to write to the log
	 */
	UFUNCTION(BlueprintCallable)
	static void LogMessage(UObject* LogObject, FString LogMessage);

	/**
	 * Used by BP (LogObject) to log the message LogMessage to the Display log
	 * @param LogObject Plug a reference to self into this, to identify the BP that is logging
	 * @param LogMessage Whatever you want to write to the log
	 */
	UFUNCTION(BlueprintCallable)
	static void DisplayLog(UObject* LogObject, FString LogMessage);

};