
#include "Inventory/LyraItemComponent.h"
#include "LyraInventorySubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Inventory/LyraInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraItemComponent)

ULyraItemComponent::ULyraItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULyraItemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(const AActor* Actor = GetOwner())
	{
		if(const UGameInstance* GameInstance = Actor->GetGameInstance())
		{
			if(ULyraInventorySubsystem* InventorySubsystem = GameInstance->GetSubsystem<ULyraInventorySubsystem>())
			{
				FLyraItemInfoRowDefine RowDefine;
				InventorySubsystem->FindItemDefRowDefine(ItemID, RowDefine);
				CustomItemInfo = RowDefine.CustomItemInfo;
				//Set up ItemInstance
				ItemInstance = InventorySubsystem->GenerateItemInstance(CustomItemInfo, this);
				ItemInstance->StackSize = StackSize;
			}
		}
	}
}





