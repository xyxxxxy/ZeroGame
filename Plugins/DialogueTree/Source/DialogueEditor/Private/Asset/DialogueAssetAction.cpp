
#include "Asset/DialogueAssetAction.h"

#include "Dialogue.h"
#include "Asset/DialogueAssetEditor.h"

#define LOCTEXT_NAMESPACE "DialogueAssetAction"

FDialogueAssetAction::FDialogueAssetAction(EAssetTypeCategories::Type Category) : AssetCategory(Category)
{
}

FText FDialogueAssetAction::GetName() const
{
	return LOCTEXT("AssetTypeName","Dialogue Tree");
}

FColor FDialogueAssetAction::GetTypeColor() const
{
	return FColor::Green;
}

UClass* FDialogueAssetAction::GetSupportedClass() const
{
	return UDialogue::StaticClass();
}

void FDialogueAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for(UObject* Object : InObjects)
	{
		UDialogue* Dialogue = Cast<UDialogue>(Object);
		if(Dialogue)
		{
			TSharedRef<FDialogueAssetEditor> Editor(new FDialogueAssetEditor());
			Editor->InitEditor(Mode,EditWithinLevelEditor,Dialogue);
		}
	}
}

uint32 FDialogueAssetAction::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE