﻿
#pragma once

#include "IDetailCustomization.h"

class FDialogueGraphCustomization : public IDetailCustomization
{
public:
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	static TSharedRef<IDetailCustomization> MakeInstance();
};