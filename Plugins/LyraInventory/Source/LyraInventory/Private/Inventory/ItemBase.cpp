
#include "Inventory/ItemBase.h"
#include "Components/StaticMeshComponent.h"
#include "Inventory/LyraItemComponent.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
	SetRootComponent(StaticMeshComp);
	
	ItemComponent = CreateDefaultSubobject<ULyraItemComponent>("ItemComponent");
}
