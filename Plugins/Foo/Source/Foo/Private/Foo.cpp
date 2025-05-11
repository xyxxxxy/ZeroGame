// Copyright Epic Games, Inc. All Rights Reserved.

#include "Foo.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"
#include "ShaderCore.h"
#define LOCTEXT_NAMESPACE "FFooModule"

void FFooModule::StartupModule()
{
	FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("Foo"))->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugins/Foo"), PluginShaderDir);
}

void FFooModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFooModule, Foo)