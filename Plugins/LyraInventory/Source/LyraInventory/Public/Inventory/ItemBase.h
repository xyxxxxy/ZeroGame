
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

class ULyraItemComponent;

UCLASS()
class LYRAINVENTORY_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemBase();
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Item)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Item)
	TObjectPtr<ULyraItemComponent> ItemComponent;
};
