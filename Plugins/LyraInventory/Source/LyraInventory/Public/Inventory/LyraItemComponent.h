

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "LyraItemComponent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class LYRAINVENTORY_API ULyraItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULyraItemComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Item")
	int32 StackSize = 1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FLyraInventoryTableRowFragmentsArray CustomItemInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "Item")
	TObjectPtr<ULyraInventoryItemInstance> ItemInstance;
};
