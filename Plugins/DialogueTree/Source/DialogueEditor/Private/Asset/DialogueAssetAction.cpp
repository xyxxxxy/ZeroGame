
#include "Asset/DialogueAssetAction.h"
#include "Dialogue.h"
#include "Asset/DialogueAssetEditor.h"

#define LOCTEXT_NAMESPACE "DialogueAssetAction"

FDialogueAssetAction::FDialogueAssetAction(EAssetTypeCategories::Type Category) : AssetCategory(Category)
{
}

FText FDialogueAssetAction::GetName() const
{
	return LOCTEXT("AssetTypeName", "Dialogue Tree");
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
	// 关卡编辑器WorldCentric:
	// 主视口：显示当前关卡的三维场景，开发者可在此进行物体拖放、移动、旋转等操作。
	// 内容浏览器：通常停靠在左侧或右侧，用于快速查找和拖拽资源（如模型、材质）到场景中。
	// 细节面板：停靠在右侧，显示选中对象的属性（如位置、旋转、缩放参数），支持实时修改。
	// 其他工具：如光照面板、地形编辑器等，可以浮动或集成到主界面中，与主视口协同工作。
	// Standalone模式：工具窗口独立于主编辑器存在，适用于不需要直接操作场景的工具(如独立的材质编辑器)
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for(UObject* Object : InObjects)
	{
		UDialogue* Dialogue = Cast<UDialogue>(Object);
		if(Dialogue)
		{
			TSharedRef<FDialogueAssetEditor> Editor(new FDialogueAssetEditor());
			Editor->InitEditor(Mode, EditWithinLevelEditor, Dialogue);
		}
	}
}

uint32 FDialogueAssetAction::GetCategories()
{
	return AssetCategory;
}

#undef LOCTEXT_NAMESPACE