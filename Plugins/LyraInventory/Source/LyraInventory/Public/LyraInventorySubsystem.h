
#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LyraInventorySubsystem.generated.h"

class ULyraInventoryItemInstance;

UCLASS()
class LYRAINVENTORY_API ULyraInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = Outer), Category = "Lyra | Inventory")
	ULyraInventoryItemInstance* GenerateItemInstance(const FLyraInventoryTableRowFragmentsArray& ItemInfo, UObject* Outer);
	
	UFUNCTION(BlueprintCallable, Category = "Lyra | Inventory")
	void FindItemDefRowDefine(FName RowName, FLyraItemInfoRowDefine& OutItemDef) const;

	UFUNCTION(BlueprintCallable, Category = "Lyra | Inventory")
	UDataTable* GetItemDefDataTable() const;
};
