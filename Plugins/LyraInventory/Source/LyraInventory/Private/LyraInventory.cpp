
#include "LyraInventory.h"

UE_DEFINE_GAMEPLAY_TAG(FLyraInvBagSlotTag,TEXT("Inventory.Slot.Bag"));
UE_DEFINE_GAMEPLAY_TAG(FLyraInvEquipSlotTag,TEXT("Inventory.Slot.Equip"));
UE_DEFINE_GAMEPLAY_TAG(FLyraInvActiveSlotTag,TEXT("Inventory.Slot.Active"));

UE_DEFINE_GAMEPLAY_TAG(FLyraItemPropTag,TEXT("Item.Prop"));
UE_DEFINE_GAMEPLAY_TAG(FLyraItemPropAppleTag,TEXT("Item.Prop.Apple"));
UE_DEFINE_GAMEPLAY_TAG(FLyraItemPropMilkTag,TEXT("Item.Prop.Milk"));

UE_DEFINE_GAMEPLAY_TAG(FLyraItemWeaponTag,TEXT("Item.Weapon"));
UE_DEFINE_GAMEPLAY_TAG(FLyraItemArmorTag,TEXT("Item.Armor"));

UE_DEFINE_GAMEPLAY_TAG(FLyraInvPickupItemAbilityTag,TEXT("Ability.Inventory.PickupItem"))
UE_DEFINE_GAMEPLAY_TAG(FLyraInvDropItemAbilityTag,TEXT("Ability.Inventory.DropItem"))

#define LOCTEXT_NAMESPACE "FLyraInventoryModule"

void FLyraInventoryModule::StartupModule()
{
}

void FLyraInventoryModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLyraInventoryModule, LyraInventory)