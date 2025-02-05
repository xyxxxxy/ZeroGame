
#pragma once

#include "EdGraphUtilities.h"

class FDialogueNodeFactory : public FGraphPanelNodeFactory
{
public:
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override;
};