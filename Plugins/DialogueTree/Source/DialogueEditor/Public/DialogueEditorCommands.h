
#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FDialogueEditorCommands : public TCommands<FDialogueEditorCommands>
{
public:
    FDialogueEditorCommands();
    virtual void RegisterCommands() override;

    TSharedPtr<FUICommandInfo> CompileAsset;
    TSharedPtr<FUICommandInfo> TestAsset;
};