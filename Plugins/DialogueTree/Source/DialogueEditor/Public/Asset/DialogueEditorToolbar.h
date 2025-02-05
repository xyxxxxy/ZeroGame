
#pragma once

#include "CoreMinimal.h"
#include "Asset/DialogueAssetEditor.h"

class UToolMenu;
struct FSlateIcon;

class DIALOGUEEDITOR_API FDialogueEditorToolbar : public TSharedFromThis<FDialogueEditorToolbar>
{
public:
	explicit FDialogueEditorToolbar(const TSharedPtr<FDialogueAssetEditor> InAssetEditor, UToolMenu* ToolbarMenu);

public:
	void BuildAssetToolbar(UToolMenu* ToolbarMenu) const;
	
	FSlateIcon GetStatusImage() const;
	FText GetLabel() const;
	FText GetToolTip() const;

private:
	TWeakPtr<FDialogueAssetEditor> Editor;
};