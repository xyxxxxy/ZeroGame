
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InstancedStruct/LyraInventoryTableRow.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "UObject/UObjectThreadContext.h"
#include "Processors/LyraInventoryProcessor.h"
#include "LyraInventoryComponent.generated.h"


USTRUCT(BlueprintType)
struct LYRAINVENTORY_API FLyraInventoryPayload
{
	GENERATED_BODY()
public:
	FLyraInventoryPayload(){}
	FLyraInventoryPayload(const FLyraInventoryPayload& Other) : SlotHandle(Other.SlotHandle), PreviousItem(Other.PreviousItem), CurrentItem(Other.CurrentItem){}
	FLyraInventoryPayload(const FLyraInventoryItemSlotHandle& InSlotHandle, ULyraInventoryItemInstance* InPreviousItem, ULyraInventoryItemInstance* InCurrentItem)
	 : SlotHandle(InSlotHandle), PreviousItem(InPreviousItem), CurrentItem(InCurrentItem){}
	UPROPERTY(BlueprintReadOnly, Category = Payload)
	FLyraInventoryItemSlotHandle SlotHandle;

	UPROPERTY(BlueprintReadOnly, Category = Payload)
	TObjectPtr<ULyraInventoryItemInstance> PreviousItem;

	UPROPERTY(BlueprintReadOnly, Category = Payload)
	TObjectPtr<ULyraInventoryItemInstance> CurrentItem;
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
	
	// processor
	using ForEachProcessorFunc = TFunctionRef<void(ULyraInventoryProcessor*)>;
	void ForEachProcessor(ForEachProcessorFunc Func);

	template<class TProcessor>
	TProcessor* AddDefaultProcessor(UObject* Owner)
	{
		FObjectInitializer* CurrentInitializer = FUObjectThreadContext::Get().TopInitializer();
		auto Processor = CurrentInitializer->CreateDefaultSubobject<TProcessor>(this, TProcessor::StaticClass()->GetFName());
		Processors.Add(Processor);
		return Processor;
	}
	
	template<class TProcessor>
	TProcessor* AddProcessor(const FGameplayTagContainer& ProcessorTags = {})
	{
		return Cast<TProcessor>(AddProcessor(TProcessor::StaticClass(), ProcessorTags));
	}	
	ULyraInventoryProcessor* AddProcessor(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, const FGameplayTagContainer& ProcessorTags = {});

	using ProcessorQueryPredicate = TFunctionRef<bool(ULyraInventoryProcessor* Processor)>;
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
		return Cast<T>(FindFirstProcessor([&Predicate](ULyraInventoryProcessor* Processor) {
			return Processor->IsA(T::StaticClass()) && Predicate(Processor);
		}));
	}


	UFUNCTION(BlueprintCallable, Category = "Inventory | Processor", DisplayName = "Find First Processor", meta = (DeterminesOutputType = ProcessorClass))
	ULyraInventoryProcessor* K2_FindFirstProcessor(TSubclassOf<ULyraInventoryProcessor> ProcessorClass);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Processor", DisplayName = "Find First Processor By Tag Query", meta = (DeterminesOutputType = ProcessorClass))
	ULyraInventoryProcessor* K2_FindFirstProcessorMatchingTags(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, FGameplayTagQuery Query);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Processor", DisplayName = "Find All Processors", meta = (DeterminesOutputType = ProcessorClass))
	TArray<ULyraInventoryProcessor*> K2_FindAllProcessors(TSubclassOf<ULyraInventoryProcessor> ProcessorClass);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Processor", DisplayName = "Find All Processors By Tag Query", meta = (DeterminesOutputType = ProcessorClass))
	TArray<ULyraInventoryProcessor*> K2_FindAllProcessorsMatchingTags(TSubclassOf<ULyraInventoryProcessor> ProcessorClass, FGameplayTagQuery Query);
	// ~~~~~processor

public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool LootItem(ULyraInventoryItemInstance* Item, FGameplayTag EventTag);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool PlaceItemIntoSlot(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle);
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool RemoveAllItemsFromInventory(TArray<ULyraInventoryItemInstance*>& OutItemsRemoved);
	
	// TODO: 删除后需要排序吗?
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool RemoveItemFromInventory(const FLyraInventoryItemSlotHandle& ItemHandle, bool bNeedSort = false);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual ULyraInventoryItemInstance* GetItemInstanceBySlot(const FLyraInventoryItemSlotHandle& ItemHandle);
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual FLyraInventoryItemSlot& GetItemSlot(const FLyraInventoryItemSlotHandle& Handle);
	
	virtual bool IsValidItemSlot(const FLyraInventoryItemSlotHandle& Handle);
	virtual bool AcceptsItem(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle);
	virtual bool AcceptsItem_AssumeEmptySlot(ULyraInventoryItemInstance* Item, const FLyraInventoryItemSlotHandle& ItemHandle);

	// TODO: 几乎用不到 <==> 弃用
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void RemoveInventorySlot(const FLyraInventoryItemSlotHandle& Handle);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void BulkCreateInventorySlots(const FGameplayTagContainer& SlotTags, const FLyraInventoryItemFilterHandle& Filter, int32 Count, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles);


	virtual void PostInventoryUpdate(FLyraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles);
	virtual void PopulateSlotReferenceArray(FLyraInventoryItemArray& InventoryRef, TArray<FLyraInventoryItemSlotHandle>& SlotHandles);

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory | Query")
	bool Query_GetAllSlotHandles(const FLyraInventoryQuery& Query, TArray<FLyraInventoryItemSlotHandle>& OutSlotHandles);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Query")
	bool Query_GetAllSlots(const FLyraInventoryQuery& Query, TArray<FLyraInventoryItemSlot>& OutSlots);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Query")
	bool Query_GetAllItems(const FLyraInventoryQuery& Query, TArray<ULyraInventoryItemInstance*>& OutItems);

	// TODO: Manually sort by id?

protected:
	UFUNCTION()
	void OnItemSlotUpdate(ULyraInventoryComponent* InventoryComponent, const FLyraInventoryPayload& Payload);


public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdateDelegate, ULyraInventoryComponent*, InventoryComponent);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdateDelegateNative, ULyraInventoryComponent*);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSlotUpdateDelegate,
		ULyraInventoryComponent*, InventoryComponent,
		const FLyraInventoryPayload&, Payload);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemSlotUpdateDelegateNative,
		ULyraInventoryComponent*, /* InventoryComponent */
		const FLyraInventoryPayload& /* Payload */);
	
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

private:
	friend class ULyraInventoryProcessor_Bag;

	UPROPERTY(EditAnyWhere, meta = (AllowPrivateAccess = "true"), Category = Inventory)
	TArray<TObjectPtr<ULyraInventoryProcessor>> Processors;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	int32 IdCounter;

private:
	UPROPERTY()
	FLyraInventoryItemArray Inventory;

	TArray<FLyraInventoryItemSlotHandle> AllSlotHandles;

};