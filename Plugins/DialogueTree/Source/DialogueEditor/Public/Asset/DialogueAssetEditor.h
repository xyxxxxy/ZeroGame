
#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class FDialogueEditorToolbar;
class UDialogue;

class FDialogueAssetEditor : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook, public FGCObject
{
public:
	//注册不同类型的标签页生成器
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);
	
	UDialogue* GetDialogue() const;

	void CreateGraph();

	void SetGraph(UEdGraph* InGraph);
	UEdGraph* GetGraph() const;

	void SetGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor);
	TSharedPtr<SGraphEditor> GetGraphEditor() const;

public:
	// FAssetEditorToolkit Interface
	// 唯一名称（如材质编辑器返回 "MaterialEditor"）
	virtual FName GetToolkitFName() const override;
	// 显示为窗口标题，help中可查看
	virtual FText GetBaseToolkitName() const override;
	// 自定义 GetToolkitName
	virtual FText GetToolkitName() const override;
	// 当编辑器以 嵌入式模式(WorldCentric)运行时，返回标签页的前缀名称，用于区分不同编辑器实例?
	virtual FString GetWorldCentricTabPrefix() const override;
	// 设置嵌入式编辑器标签页的 颜色标识?
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	// 返回编辑器的 工具提示文本(鼠标悬停在标签页或窗口时的提示信息)
	virtual FText GetToolkitToolTipText() const override;
	// 返回编辑器帮助文档的 URL 链接，用户按下 F1 时自动跳转
	virtual FString GetDocumentationLink() const override;
	// 当编辑器被 嵌入到主编辑器窗口(WorldCentric 模式)时触发，用于初始化与宿主相关的逻辑
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
	// 当编辑器从主编辑器窗口 分离(如关闭或切换为独立窗口)时触发，用于清理宿主相关资源
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;
	// 当用户请求关闭编辑器时触发，用于执行关闭前的检查(如保存未提交的修改)
	virtual bool OnRequestClose(EAssetEditorCloseReason InCloseReason) override;
	// ~ Interface

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;

	void SetNodeDetailView(TSharedPtr<IDetailsView> InDetailView);
	TSharedPtr<IDetailsView> GetNodeDetailView() const;

	void SpawnInitialNodes();

	void CreateToolbar();
	void BindToolbarCommands();
	

	
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);
	
	void OnCompile();
	
	

	
	void OnChangeSelection(const TSet<UObject*>& SelectedObjects);

private:
	TObjectPtr<UDialogue> Dialogue;
	TObjectPtr<UEdGraph> Graph;

	TSharedPtr<IDetailsView> NodeDetailsView;

	TWeakPtr<SGraphEditor> GraphEditor;

	//TSharedPtr<FUICommandList> ToolKitCommands;
	TSharedPtr<FDialogueEditorToolbar> AssetToolbar;
};
