
using UnrealBuildTool;

public class DialogueEditor : ModuleRules
{
	public DialogueEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", 
				"CoreUObject",
				"UnrealEd",
				"Slate",
				"SlateCore", 
                "DialogueRuntime",
                "GameplayTags", 
                "EditorStyle"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				
				"DialogueRuntime",
				
				"AssetTools",
				"UnrealEd",
				"PropertyEditor",
				"Projects",
				"ToolMenus",
				"GraphEditor"
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
