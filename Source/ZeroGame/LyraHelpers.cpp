#include "LyraHelpers.h"

#include "EnhancedInputSubsystems.h"
#include "LyraLog.h"


UEnhancedInputLocalPlayerSubsystem* ULyraHelpers::GetEnhancedInputLocalPlayerSubsystem(const APlayerController* PlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem {nullptr};

	if (IsValid(PlayerController))
	{
		if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		}
	}

	return Subsystem;
}


// This allows Blueprints to send messages to the usual error log stream
void ULyraHelpers::ErrorLog(UObject* LogObject, FString LogMessage)
{
	Lyra_ERROR_LOG(TEXT("Error: [%s]: %s"), *GetNameSafe(LogObject), *LogMessage);
}


// This allows Blueprints to send messages to the usual warning log stream
void ULyraHelpers::WarningLog(UObject* LogObject, FString LogMessage)
{
	Lyra_WARNING_LOG(TEXT("Warning: [%s]: %s"), *GetNameSafe(LogObject), *LogMessage);
}


// This allows Blueprints to send messages to the usual log stream
void ULyraHelpers::LogMessage(UObject* LogObject, FString LogMessage)
{
	Lyra_LOG(TEXT("[%s]: %s"), *GetNameSafe(LogObject), *LogMessage);
}


// This allows Blueprints to send messages to the usual display log stream
void ULyraHelpers::DisplayLog(UObject* LogObject, FString LogMessage)
{
	Lyra_DISPLAY_LOG(TEXT("[%s]: %s"), *GetNameSafe(LogObject), *LogMessage);
}