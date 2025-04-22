
#include "Asset/DialogueEditorToolbar.h"
#include "DialogueEditorCommands.h"
#include "Asset/DialogueAssetEditor.h"
#include "Textures/SlateIcon.h"
#include "Dialogue.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#define LOCTEXT_NAMESPACE "DialogueEditorToolbar"

FDialogueEditorToolbar::FDialogueEditorToolbar(const TSharedPtr<FDialogueAssetEditor> InAssetEditor, UToolMenu *ToolbarMenu)
{
	Editor = InAssetEditor;
	// BuildAssetToolbar(ToolbarMenu);
}

void FDialogueEditorToolbar::BuildAssetToolbar(UToolMenu *ToolbarMenu) const
{
	{
		ToolbarMenu->AddDynamicSection("NewSection",
									   FNewToolMenuDelegate::CreateLambda([](UToolMenu *InMenu)
																		  {
		// if(...) 可以在此增加灵活的判断条件
		{
			FToolMenuSection& SubSection = InMenu->AddSection("DynamicSection", LOCTEXT("DynamicSection", "Dynamic Section"));
			SubSection.AddMenuEntry(
				"DynamicSectionTestEntry",
				LOCTEXT("DynamicSectionTestEntry", "Dynamic Section Test Entry"),
				LOCTEXT("", ""),
				FSlateIcon(),
				FUIAction());
			
		} }));
	}

	{

		// 添加Section
		FToolMenuSection &Section = ToolbarMenu->FindOrAddSection("DialogueTreeAsset");
		// 如果引擎默认或其他插件已经定义了一个名为 "Asset" 的区块（如资源管理操作），此代码会将自定义区块紧接其后
		Section.InsertPosition = FToolMenuInsert("Asset", EToolMenuInsertType::After);

		Section.AddEntry(FToolMenuEntry::InitToolBarButton(
			FDialogueEditorCommands::Get().CompileAsset,
			TAttribute<FText>(this, &FDialogueEditorToolbar::GetLabel),
			TAttribute<FText>(this, &FDialogueEditorToolbar::GetToolTip),
			TAttribute<FSlateIcon>(this, &FDialogueEditorToolbar::GetStatusImage)));
	}

	{
		FToolMenuSection &Section = ToolbarMenu->FindOrAddSection("Test");
		Section.InsertPosition = FToolMenuInsert("DialogueTreeAsset", EToolMenuInsertType::After);
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(
			FDialogueEditorCommands::Get().TestAsset
			// TAttribute<FText>(this,&FDialogueEditorToolbar::GetLabel)
			));

		// 添加子菜单
		Section.AddDynamicEntry("SourceControlCommands", FNewToolMenuSectionDelegate::CreateLambda([this](FToolMenuSection &InSection)
																								   {
			InSection.InsertPosition = FToolMenuInsert();
			FToolMenuEntry DiffEntry = FToolMenuEntry::InitComboButton(
				"Diff",
				FUIAction(),
				FOnGetContent::CreateRaw(this, &FDialogueEditorToolbar::MakeDiffMenu),
				LOCTEXT("Diff", "Diff"),
				LOCTEXT("FlowAssetEditorDiffToolTip", "Diff against previous revisions"),
				FSlateIcon(FAppStyle::Get().GetStyleSetName(), "BlueprintDiff.ToolbarIcon")
			);
			DiffEntry.StyleNameOverride = "CalloutToolbar";
			InSection.AddEntry(DiffEntry); }));
	}
}

FSlateIcon FDialogueEditorToolbar::GetStatusImage() const
{
	UDialogue *Dialogue = Editor.Pin()->GetDialogue();
	check(Dialogue);
	switch (Dialogue->GetCompileStatus())
	{
	case EDialogueCompileStatus::Compiled:
		return FSlateIcon();

	default:
		return FSlateIcon();
	}
}

FText FDialogueEditorToolbar::GetLabel() const
{
	return FText::FromString(TEXT("Compile"));
}

FText FDialogueEditorToolbar::GetToolTip() const
{
	return FText::FromString(TEXT("Compile the Graph and Nodes!"));
}

void FDialogueEditorToolbar::FillSubMenu(FMenuBuilder &MenuBuilder) const
{
	// 向子菜单中添加条目
	UE_LOG(LogTemp, Warning, TEXT("FillSubMenu called!"));

	MenuBuilder.AddMenuEntry(
		FText::FromString("Action 1"),
		FText::FromString("Perform Action 1"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateSP(this->AsShared(), &FDialogueEditorToolbar::OnAction1)));

	MenuBuilder.AddMenuEntry(
		FText::FromString("Action 2"),
		FText::FromString("Perform Action 2"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateSP(this->AsShared(), &FDialogueEditorToolbar::OnAction2)));
}

void FDialogueEditorToolbar::OnAction1() const
{
}

void FDialogueEditorToolbar::OnAction2() const
{
}

TSharedRef<SWidget> FDialogueEditorToolbar::MakeDiffMenu() const
{
	FMenuBuilder MenuBuilder(true, nullptr);
	// TODO: FToolMenuEntry::InitComboButton
	MenuBuilder.AddMenuEntry(LOCTEXT("SourceControlDisabled1", "Source control is disabled1"), FText(), FSlateIcon(), FUIAction());
	MenuBuilder.AddMenuEntry(LOCTEXT("SourceControlDisabled2", "Source control is disabled2"), FText(), FSlateIcon(), FUIAction());
	return MenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE
