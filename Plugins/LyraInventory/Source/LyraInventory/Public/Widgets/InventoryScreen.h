
#include "CommonActivatableWidget.h"
#include "Inventory/LyraInventoryItemTypes.h"
#include "InventoryScreen.generated.h"

class UInventoryPanel;
UCLASS(Blueprintable, meta = (Category = Inventory))
class UInventoryScreen : public UCommonActivatableWidget
{
        GENERATED_BODY()
public:
        UInventoryScreen(){}

protected:
        UFUNCTION(BlueprintCallable)
        void FilterSlots(FName Id);

        UFUNCTION(BlueprintCallable)
        void AddItemForIdToQuery(FName Id, const FLyraInventoryQuery& Query);
private:
        UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
        TObjectPtr<UInventoryPanel> InventoryPanel;

        UPROPERTY(Transient)
        TMap<FName, FLyraInventoryQuery> IdToQuery;
};