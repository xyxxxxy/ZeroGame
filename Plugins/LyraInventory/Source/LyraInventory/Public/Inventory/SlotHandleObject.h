#pragma once

#include "CoreMinimal.h"
#include "LyraInventoryItemTypes.h"
#include "SlotHandleObject.generated.h"

struct FLyraInventoryItemSlotHandle;

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FSlotHandleObjectPayload
{
	GENERATED_BODY()
public:
	FSlotHandleObjectPayload() {}
	FSlotHandleObjectPayload(const FLyraInventoryItemSlotHandle &InSlotHandle) : SlotHandle(InSlotHandle) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	FLyraInventoryItemSlotHandle SlotHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 w = 1;
};

UCLASS()
class LYRAINVENTORY_API USlotHandleObject : public UObject
{
	GENERATED_BODY()

public:
	USlotHandleObject(const FObjectInitializer &ObjectInitializer);

	FSlotHandleObjectPayload GetPayload() const;
	void Setpayload(const FSlotHandleObjectPayload &InPayload);

private:
	UPROPERTY(Transient)
	FSlotHandleObjectPayload Payload;
};