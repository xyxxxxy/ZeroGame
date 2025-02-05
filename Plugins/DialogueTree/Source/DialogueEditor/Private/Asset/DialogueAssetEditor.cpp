
#include "Asset/DialogueAssetEditor.h"

#include "Dialogue.h"
#include "DialogueEditorCommands.h"
#include "GraphEditorActions.h"
#include "Asset/DialogueAssetAppMode.h"
#include "Asset/DialogueEditorToolbar.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/DialogueEdGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/Commands/GenericCommands.h"
#include "DialogueEditorLogChannels.h"

#define LOCTEXT_NAMESPACE "DialogueAssetEditor"

void FDialogueAssetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void FDialogueAssetEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	Dialogue = Cast<UDialogue>(InObject);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
	NodeDetailsView = PropertyEditorModule.CreateDetailView(FDetailsViewArgs());
	
	CreateGraph();

	ToolKitCommands = MakeShareable(new FUICommandList());
	
	UE_LOG(LogDialogueEditor, Error, TEXT("Pre Init Editor"));
	
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		TEXT("DialogueAssetEditor"),
		FTabManager::FLayout::NullLayout,
		true,//?
		true,
		ObjectsToEdit
	);



	AddApplicationMode(TEXT("DialogueAssetAppMode"),MakeShareable(new FDialogueAssetAppMode(SharedThis(this))));

	SetCurrentMode(TEXT("DialogueAssetAppMode"));

	BindToolbarCommands();
	CreateToolbar();

	RegenerateMenusAndToolbars();
}

UDialogue* FDialogueAssetEditor::GetDialogue() const
{
	return Dialogue;
}

void FDialogueAssetEditor::CreateGraph()
{
	if(!Dialogue->GetGraph())
	{
		Graph = FBlueprintEditorUtils::CreateNewGraph(
			Dialogue,
			NAME_None,
			UDialogueEdGraph::StaticClass(),
			UDialogueEdGraphSchema::StaticClass()
			);
		check(Graph);

		Dialogue->SetGraph(Graph);
		Dialogue->GetGraph()->bAllowDeletion = false;
		UE_LOG(LogDialogueEditor, Warning, TEXT("AssetEditor : SpawnInitialNodes!"));
		SpawnInitialNodes();
	}

}

void FDialogueAssetEditor::SetGraph(UEdGraph* InGraph)
{
	Graph = InGraph;
}

UEdGraph* FDialogueAssetEditor::GetGraph() const
{
	return Graph;
}

void FDialogueAssetEditor::SetGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor)
{
	GraphEditor = InGraphEditor;
}

TSharedPtr<SGraphEditor> FDialogueAssetEditor::GetGraphEditor() const
{
	return GraphEditor.Pin();
}

FName FDialogueAssetEditor::GetToolkitFName() const
{
	return FName(TEXT("FDialogueTreeEditor"));
}

FText FDialogueAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("DialogueEditorBaseToolkitName", "Dialogue Tree Editor");
}

FText FDialogueAssetEditor::GetToolkitName() const
{
	if (Dialogue)
	{
		return FText::FromString(Dialogue->GetName());
	}

	return GetBaseToolkitName();
}

FString FDialogueAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("DialogueAssetEditor");
}

FLinearColor FDialogueAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(.3f,.2f,.5f,.5f);
}

FText FDialogueAssetEditor::GetToolkitToolTipText() const
{
	    return GetToolTipTextForObject(Dialogue);
}

FString FDialogueAssetEditor::GetDocumentationLink() const
{
	return TEXT("http://github.com/xyxxxxy");
}

void FDialogueAssetEditor::OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit)
{
}

void FDialogueAssetEditor::OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit)
{
}

bool FDialogueAssetEditor::OnRequestClose(EAssetEditorCloseReason InCloseReason)
{
	return FWorkflowCentricApplication::OnRequestClose(InCloseReason);
}

void FDialogueAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	check(Dialogue);
	UEdGraph* TargetGraph = Dialogue->GetGraph();
	check(TargetGraph);

	Collector.AddReferencedObject(Dialogue);
	Collector.AddReferencedObject(TargetGraph);
}

FString FDialogueAssetEditor::GetReferencerName() const
{
	return TEXT("FDialogueTreeEditor");
}

void FDialogueAssetEditor::SetNodeDetailView(TSharedPtr<IDetailsView> InDetailView)
{
	InDetailView = InDetailView.ToSharedRef();
}

TSharedPtr<IDetailsView> FDialogueAssetEditor::GetNodeDetailView() const
{
	return NodeDetailsView;
}

void FDialogueAssetEditor::SpawnInitialNodes()
{
	const UEdGraphSchema* GraphSchema = Graph->GetSchema();
	check(GraphSchema);
	GraphSchema->CreateDefaultNodesForGraph(*Graph);
}

void FDialogueAssetEditor::CreateToolbar()
{
	FName ParentToolbarName;
	const FName ToolBarName = GetToolMenuToolbarName(ParentToolbarName);

	UToolMenus* ToolMenus = UToolMenus::Get();
	UToolMenu* FoundMenu = ToolMenus->FindMenu(ToolBarName);
	if (!FoundMenu || !FoundMenu->IsRegistered())
	{
		FoundMenu = ToolMenus->RegisterMenu(ToolBarName, ParentToolbarName, EMultiBoxType::ToolBar);
	}

	if (FoundMenu)
	{
		AssetToolbar = MakeShareable(new FDialogueEditorToolbar(SharedThis(this), FoundMenu));
		AssetToolbar->BuildAssetToolbar(FoundMenu);
	}
}

void FDialogueAssetEditor::BindToolbarCommands()
{
	FDialogueEditorCommands::Register();
	const FDialogueEditorCommands& Commands = FDialogueEditorCommands::Get();


	GetToolkitCommands()->MapAction(
		Commands.CompileAsset, 
		FExecuteAction::CreateSP(this,&FDialogueAssetEditor::OnCompile), 
		FCanExecuteAction()
	);
}

void FDialogueAssetEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
}

void FDialogueAssetEditor::OnCompile()
{
    check(Graph);
	UE_LOG(LogDialogueEditor, Warning, TEXT("On Compile"));
    UDialogueEdGraph* TargetDialogueGraph = CastChecked<UDialogueEdGraph>(Graph);
    TargetDialogueGraph->CompileAsset();
}

void FDialogueAssetEditor::OnChangeSelection(const TSet<UObject*>& SelectedObjects)
{
}

#undef LOCTEXT_NAMESPACE
