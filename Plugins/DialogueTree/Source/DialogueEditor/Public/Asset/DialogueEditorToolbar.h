
#pragma once

#include "CoreMinimal.h"
#include "Asset/DialogueAssetEditor.h"

class UToolMenu;
struct FSlateIcon;
class FMenuBuilder;
class DIALOGUEEDITOR_API FDialogueEditorToolbar : public TSharedFromThis<FDialogueEditorToolbar>
{
public:
	explicit FDialogueEditorToolbar(const TSharedPtr<FDialogueAssetEditor> InAssetEditor, UToolMenu *ToolbarMenu);

public:
	void BuildAssetToolbar(UToolMenu *ToolbarMenu) const;

	FSlateIcon GetStatusImage() const;
	FText GetLabel() const;
	FText GetToolTip() const;

private:
	void FillSubMenu(FMenuBuilder &MenuBuilder) const;
	void OnAction1() const;
	void OnAction2() const;
	bool CanExecuteAction1() const { return true; }
	bool CanExecuteAction2() const { return true; }

	TSharedRef<SWidget> MakeDiffMenu() const;

private:
	TWeakPtr<FDialogueAssetEditor> Editor;
};