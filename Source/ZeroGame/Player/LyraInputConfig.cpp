#include "LyraInputConfig.h"

//#include "XcgsLog.h"


ULyraInputActionMap::ULyraInputActionMap(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


const UInputAction* ULyraInputActionMap::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	const UInputAction* Result {nullptr};

	for (const FLyraInputAction& Action : NativeInputActions)
	{
		const bool bMatch = Action.InputAction && (Action.InputTag == InputTag);
		if (bMatch && !Result)
		{
			// This tag matches; this is our result
			Result = Action.InputAction;
		}
		else if (bMatch)
		{
			// This tag matches, but we already have a result! Data error
			//XCGS_ERROR_LOG(TEXT("Found multiple instances of InputTag [%s]; using only the first"), *InputTag.ToString());
		}
	}

	if (!Result && bLogNotFound)
	{
		//XCGS_WARNING_LOG(TEXT("No InputAction found for InputTag [%s]"), *InputTag.ToString());
	}

	return Result;
}