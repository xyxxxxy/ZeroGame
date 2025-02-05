
#include "DialogueEditorModule.h"

#include "Asset/DialogueAssetAction.h"
#include "DialogueEditorStyle.h"
#include "IAssetTools.h"
#include "DetailCustomizations/DialogueGraphCustomization.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/Nodes/DialogueNodeFactory.h"
#include "DialogueSpeakerSocket.h"
#include "DetailCustomizations/DialogueSpeakerSocketCustomization.h"

#define LOCTEXT_NAMESPACE "FDialogueEditorModule"

void FDialogueEditorModule::StartupModule()
{
	FDialogueEditorStyle::Initialize();

	RegisterNodeFactory();
	RegisterAssets();
	RegisterDetailCustomizations();

}

void FDialogueEditorModule::ShutdownModule()
{
	FDialogueEditorStyle::ShutDown();

	UnRegisterNodeFactory();
	UnRegisterAssets();
	UnRegisterDetailCustomizations();
}

void FDialogueEditorModule::RegisterAssets()
{
	IAssetTools& AssetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(
		FName("DialogueTree"),
		LOCTEXT("AdvancedAssetCategory","Dialogue Tree"));

	TSharedPtr<FDialogueAssetAction> DialogueAssetAction = MakeShareable(new FDialogueAssetAction(AssetType));
	AssetToolsModule.RegisterAssetTypeActions(DialogueAssetAction.ToSharedRef());
}

void FDialogueEditorModule::UnRegisterAssets()
{
}

void FDialogueEditorModule::RegisterNodeFactory()
{
	NodeFactory = MakeShareable(new FDialogueNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);
}

void FDialogueEditorModule::UnRegisterNodeFactory()
{
	if (NodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
		NodeFactory.Reset();
	}
}

void FDialogueEditorModule::RegisterDetailCustomizations()
{
	if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		RegisterCustomClassLayout(UDialogueEdGraph::StaticClass(), FOnGetDetailCustomizationInstance::CreateStatic(FDialogueGraphCustomization::MakeInstance));
		RegisterCustomStructLayoutForClass(UDialogueSpeakerSocket::StaticClass(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(FDialogueSpeakerSocketCustomization::MakeInstance));
		
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

void FDialogueEditorModule::UnRegisterDetailCustomizations()
{
	if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		for (auto It = CustomClassLayouts.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomClassLayout(*It);
			}
		}

		for (auto It = CustomStructLayouts.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomPropertyTypeLayout(*It);
			}
		}		
		
		PropertyModule.NotifyCustomizationModuleChanged();
	}	
}

void FDialogueEditorModule::RegisterCustomClassLayout(const TSubclassOf<UObject> Class, const FOnGetDetailCustomizationInstance DetailLayout)
{
	if(Class)
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(Class->GetFName(), DetailLayout);

		CustomClassLayouts.Add(Class->GetFName());
	}
}

void FDialogueEditorModule::RegisterCustomStructLayoutForStruct(const UScriptStruct& Struct, const FOnGetPropertyTypeCustomizationInstance DetailLayout)
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(Struct.GetFName(), DetailLayout);

		CustomStructLayouts.Add(Struct.GetFName());
	}	
}

void FDialogueEditorModule::RegisterCustomStructLayoutForClass(const TSubclassOf<UObject> Class, const FOnGetPropertyTypeCustomizationInstance DetailLayout)
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout(Class->GetFName(), DetailLayout);

		CustomStructLayouts.Add(Class->GetFName());
	}		
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogueEditorModule, DialogueEditor)