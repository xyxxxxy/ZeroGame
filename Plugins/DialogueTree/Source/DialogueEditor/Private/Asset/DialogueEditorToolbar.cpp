
#include "Asset/DialogueEditorToolbar.h"
#include "DialogueEditorCommands.h"
#include "Asset/DialogueAssetEditor.h"
#include "Textures/SlateIcon.h"
#include "Dialogue.h"

#define LOCTEXT_NAMESPACE "DialogueEditorToolbar"

FDialogueEditorToolbar::FDialogueEditorToolbar(const TSharedPtr<FDialogueAssetEditor> InAssetEditor, UToolMenu* ToolbarMenu)
{
	Editor = InAssetEditor;
	//BuildAssetToolbar(ToolbarMenu);
}

void FDialogueEditorToolbar::BuildAssetToolbar(UToolMenu* ToolbarMenu) const
{
	FToolMenuSection& Section = ToolbarMenu->AddSection("DialogueTreeAsset");

	Section.InsertPosition = FToolMenuInsert("Asset",EToolMenuInsertType::After);

	Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		FDialogueEditorCommands::Get().CompileAsset,
		TAttribute<FText>(this,&FDialogueEditorToolbar::GetLabel), 
		TAttribute<FText>(this,&FDialogueEditorToolbar::GetToolTip), 
		TAttribute<FSlateIcon>(this,&FDialogueEditorToolbar::GetStatusImage)
		));

	Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		FDialogueEditorCommands::Get().TestAsset
		//TAttribute<FText>(this,&FDialogueEditorToolbar::GetLabel)
		));	
}

FSlateIcon FDialogueEditorToolbar::GetStatusImage() const
{
	UDialogue* Dialogue = Editor.Pin()->GetDialogue();
	check(Dialogue);
	switch (Dialogue->GetCompileStatus())
	{
	case EDialogueCompileStatus::Compiled :
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

#undef LOCTEXT_NAMESPACE
