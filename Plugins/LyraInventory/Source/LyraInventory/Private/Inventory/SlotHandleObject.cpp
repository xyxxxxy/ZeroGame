
#include "Inventory/SlotHandleObject.h"

USlotHandleObject::USlotHandleObject(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

FSlotHandleObjectPayload USlotHandleObject::GetPayload() const
{
        return Payload;
}

void USlotHandleObject::Setpayload(const FSlotHandleObjectPayload &InPayload)
{
        if (InPayload.SlotHandle != Payload.SlotHandle)
        {
                Payload = InPayload;
        }
}