
#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class FDialogueAssetAction : public FAssetTypeActions_Base
{
public:
	FDialogueAssetAction(EAssetTypeCategories::Type Category);

public:
	//定义资产在右键菜单中的显示名称
	virtual FText GetName() const override;
	//设置资产在资源管理器中的标识颜色
	virtual FColor GetTypeColor() const override;
	//指定资产关联的 UClass(需继承自 UObject),确保资产能被引擎正确识别和实例化
	virtual UClass* GetSupportedClass() const override;
	//定义用户双击资产时打开的编辑器界面
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
	//指定资产在新建菜单中的分类
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type AssetCategory;
};