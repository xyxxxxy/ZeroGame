#pragma once

#include "Framework/Application/NavigationConfig.h"

class FLyraNavigationConfig : public FNavigationConfig
{
public:
        FLyraNavigationConfig();
	
protected:
	virtual bool IsAnalogHorizontalKey(const FKey& InKey) const {return false;};
	virtual bool IsAnalogVerticalKey(const FKey& InKey) const {return false;};
};