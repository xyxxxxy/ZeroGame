#pragma once

#include "CoreMinimal.h"

#include "LyraItemEnum.generated.h"

UENUM(BlueprintType)
enum class ELyraItemCategory : uint8
{
	None,
	Prop,
	Equipment,
	Document,
};
