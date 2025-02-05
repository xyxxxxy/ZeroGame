
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "UObject/UObjectThreadContext.h"
#include "Processors/LyraInventoryProcessor.h"
#include "LyraInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FProcessorArray
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Instanced, Category = Inventory)
	TArray<TObjectPtr<ULyraInventoryProcessor>> Processors;
	
};

USTRUCT()
struct LYRAINVENTORY_API FInventoryItemSlotHandlesArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLyraInventoryItemSlotHandle> AllSlotHandles;
	
};



UCLASS(Blueprintable, BlueprintType, ClassGroup = (Inventory), meta = (BlueprintSpawnableComponent))
class LYRAINVENTORY_API ULyraInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	friend class ULyraInventoryProcessor;

	ULyraInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION(BlueprintPure, Category = Abilities)
	virtual UAbilitySystemComponent* GetOwnerAbilitySystemComponent();

	UFUNCTION(BlueprintPure, Category = Inventory)
	static ULyraInventoryComponent* FindInventoryComponent(AActor* Actor) { return Actor ? Actor->FindComponentByClass<ULyraInventoryComponent>() : nullptr; }
	
	virtual void InitializeComponent() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BindToASC();
	virtual void UnbindFromASC();
	
	using ForEachProcessorFunc = TFunctionRef<void(ULyraInventoryProcessor*)>;
	void ForEachProcessor(ForEachProcessorFunc Func);

	template<class TProcessor>
	TProcessor* AddDefaultProcessor(UObject* Owner, ELyraItemCategory InItemCategory)
	{
		FObjectInitializer* CurrentInitializer = FUObjectThreadContext::Get().TopInitializer();
		auto Processor = CurrentInitializer->CreateDefaultSubobject<TProcessor>(this, TProcessor::StaticClass()->GetFName());
		// 1
		CategoryToProcessors[InItemCategory].Processors.Add(Processor);
		//Processors.Add(Processor);
		return Processor;
	}
	
	template<class TProcessor>
	TProcessor* AddProcessor(const FGameplayTagContainer& ProcessorTags = {}, ELyraItemCategory InItemCategory = ELyraItemCategory::Prop)
	{
		return Cast<TProcessor>(AddProcessor(TProcessor::StaticClass(), ProcessorTags, InItemCategory));
	}	
	ULyraInventoryProcessor* AddProcessor(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, const FGameplayTagContainer& ProcessorTags = {}, ELyraItemCategory InItemCategory = ELyraItemCategory::Prop);

	using ProcessorQueryPredicate = TFunctionRef<bool(ULyraInventoryProcessor* Processor, ELyraItemCategory InItemCategory)>;
	
	ULyraInventoryProcessor* FindFirstProcessor(ProcessorQueryPredicate Predicate) const;
	void FindAllProcessors(TArray<ULyraInventoryProcessor*>& OutProcessors, ProcessorQueryPredicate Predicate) const;

	template<typename T>
	T* FindFirstProcessor() const
	{
		return Cast<T>(FindFirstProcessor([](ULyraInventoryProcessor* Processor)
			{
				return Processor->IsA(T::StaticClass());
			}));
	}
	
	template<typename T>
	T* FindFirstProcessor(ProcessorQueryPredicate Predicate) const
	{
		return Cast<T>(FindFirstProcessor([&Predicate](ULyraInventoryProcessor* Processor, ELyraItemCategory InItemCategory) {
			return Processor->IsA(T::StaticClass()) && Predicate(Processor, InItemCategory);
		}));
	}

	template<typename T>
	T* FindFirstProcessorMatchingCategory(ELyraItemCategory InItemCategory) const
	{
		return Cast<T>(FindFirstProcessor([&InItemCategory](ULyraInventoryProcessor* Processor,ELyraItemCategory ItemCategory)
		{
			return Processor->IsA(T::StaticClass()) && InItemCategory == ItemCategory;
		})
		);
	}

	UFUNCTION(meta = (DeterminesOutputType = ProcessorClass))
	ULyraInventoryProcessor* FindFirstProcessorMatchingCategory(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, ELyraItemCategory ItemCategory);

public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool LootItem(ULyraInventoryItemInstance* Item, FGameplayTag EventTag);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool PlaceItemIntoSlot(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle);
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool RemoveAllItemsFromInventory(TArray<ULyraInventoryItemInstance*>& OutItemsRemoved);
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool RemoveItemFromInventory(const FLyraInventoryItemSlotHandle& ItemHandle);

	// stay
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual ULyraInventoryItemInstance* GetItemInstanceBySlot(const FLyraInventoryItemSlotHandle& ItemHandle);
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual FLyraInventoryItemSlot& GetItemSlot(const FLyraInventoryItemSlotHandle& Handle);
	
	virtual bool IsValidItemSlot(const FLyraInventoryItemSlotHandle& Handle);
	virtual bool AcceptsItem(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle);
	virtual bool AcceptsItem_AssumeEmptySlot(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle);

	virtual void RemoveInventorySlot(const FLyraInventoryItemSlotHandle& Handle);
	virtual void BulkCreateInventorySlots(ELyraItemCategory Category, const FGameplayTagContainer& SlotTags, const FLyraInventoryItemFilterHandle& Filter, int32 Count, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles);

protected:
	void OnItemSlotUpdate(ULyraInventoryComponent* InventoryComponent, const FLyraInventoryItemSlotHandle& SlotHandle, ULyraInventoryItemInstance* CurrentItem, ULyraInventoryItemInstance* PreviousItem);

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory | Item Query")
	bool Query_GetAllSlots(const FLyraInventoryQuery& Query, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles);

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdateDelegate, ULyraInventoryComponent*, InventoryComponent);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdateDelegateNative, ULyraInventoryComponent*);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnItemSlotUpdateDelegate,
		ULyraInventoryComponent*, InventoryComponent,
		const FLyraInventoryItemSlotHandle&, SlotHandle,
		ULyraInventoryItemInstance*, CurrentItem,
		ULyraInventoryItemInstance*, PreviousItem);
	DECLARE_MULTICAST_DELEGATE_FourParams(FOnItemSlotUpdateDelegateNative,
		ULyraInventoryComponent*, /* InventoryComponent */
		const FLyraInventoryItemSlotHandle&, /* SlotHandle */
		ULyraInventoryItemInstance*, /* CurrentItem */
		ULyraInventoryItemInstance* /* PreviousItem */);
	
public:
	FOnInventoryUpdateDelegateNative& OnInventoryUpdateDelegate(){ return OnInventoryUpdateDelegateNative; }
	FOnItemSlotUpdateDelegateNative& OnItemSlotUpdateDelegate(){ return OnItemSlotUpdateDelegateNative; }
	
private:

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnInventoryUpdate" ), Category = Inventory)
	FOnInventoryUpdateDelegate BP_OnInventoryUpdate;
	
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnItemSlotUpdate" ), Category = Inventory)
	FOnItemSlotUpdateDelegate BP_OnItemSlotUpdate;
	
private:
	mutable FOnInventoryUpdateDelegateNative OnInventoryUpdateDelegateNative;
	mutable FOnItemSlotUpdateDelegateNative OnItemSlotUpdateDelegateNative;

protected:
	ULyraInventoryProcessor_Bag* GetProcessorBag(ELyraItemCategory Category);
	
protected:
	friend class ULyraInventoryProcessor_Bag;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TMap<ELyraItemCategory, FProcessorArray> CategoryToProcessors;

private:
	friend class ULyraInventoryProcessor_Bag;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

protected:
	FLyraInventoryItemSlot GuardSlot;

};