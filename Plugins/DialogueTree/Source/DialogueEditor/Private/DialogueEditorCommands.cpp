
#include "DialogueEditorCommands.h"
#include "DialogueEditorStyle.h"

#define LOCTEXT_NAMESPACE "DialogueEditorCommands"

FDialogueEditorCommands::FDialogueEditorCommands()
: TCommands<FDialogueEditorCommands>("DialogueEditor.Common", LOCTEXT("DialogueEditor", "Dialogue"), NAME_None, FDialogueEditorStyle::GetStyleSetName())
{

}

void FDialogueEditorCommands::RegisterCommands()
{
    UI_COMMAND(CompileAsset,"Compile","Compile the Graph and Nodes",EUserInterfaceActionType::Button,FInputChord());
    UI_COMMAND(TestAsset,"Test","Test the Editor Toolbar",EUserInterfaceActionType::Button,FInputChord());
}

#undef LOCTEXT_NAMESPACE