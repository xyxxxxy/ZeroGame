
#include "LyraInventorySubsystem.h"
#include "LyraInventorySettings.h"
#include "Inventory/LyraInventoryItemInstance.h"
#include "LyraInventory/LyraLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraInventorySubsystem)

bool ULyraInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void ULyraInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogLyraInventorySystem,Warning,TEXT("Subsystem is initialized: %s"),*GetNameSafe(this));

	const ULyraInventorySettings* InventorySettings = GetDefault<ULyraInventorySettings>();
	//check(InventorySettings->ItemDefDataTable);
	UDataTable* DataTable = InventorySettings->ItemDefDataTable.LoadSynchronous();
}

void ULyraInventorySubsystem::Deinitialize()
{
	Super::Deinitialize();
	UE_LOG(LogLyraInventorySystem,Warning,TEXT("Subsystem is deinitialized: %s"),*GetNameSafe(this));
}

ULyraInventoryItemInstance* ULyraInventorySubsystem::GenerateItemInstance(
	const FLyraInventoryTableRowFragmentsArray& ItemInfo, UObject* Outer)
{
	if(ItemInfo.ItemStructs.IsEmpty())return nullptr;
	
	const FLyraInventoryTableRowFragmentsHandle Handle = FLyraInventoryTableRowFragmentsHandle(new FLyraInventoryTableRowFragmentsArray(ItemInfo));

	ULyraInventoryItemInstance* ItemInstance = NewObject<ULyraInventoryItemInstance>(Outer);
	
	ItemInstance->SetItemStructsHandle(Handle);
	return ItemInstance;
}

void ULyraInventorySubsystem::FindItemDefRowDefine(FName RowName, FLyraItemInfoRowDefine& OutItemDef) const
{
	const UDataTable* DataTable = GetItemDefDataTable();
	if(!IsValid(DataTable))return;
	
	OutItemDef = *DataTable->FindRow<FLyraItemInfoRowDefine>(RowName, FString("ItemDef"), true);
}

UDataTable* ULyraInventorySubsystem::GetItemDefDataTable() const
{
	static const ULyraInventorySettings* InventorySettings = GetDefault<ULyraInventorySettings>();
	UDataTable* DataTable = InventorySettings->ItemDefDataTable.Get();
	if(!IsValid(DataTable))
	{
		UE_LOG(LogLyraInventorySystem, Error, TEXT("Can't find DataTable for ItemDef!"));
		return nullptr;
	}
	return DataTable;
}


