// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZeroGame : ModuleRules
{
	public ZeroGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {
            "ZeroGame"
        });

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"Slate", 
			"SlateCore", 
			"LyraInventory",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"OnlineSubsystem",
			"Slate",
			"SlateCore",
			"CommonGame",
			"CommonInput",
			"CommonUI",
			"ModularGameplay",
			"ModularGameplayActors",
			"GameplayTags",
			"UMG",
			"EnhancedInput",
		});
	}
}
