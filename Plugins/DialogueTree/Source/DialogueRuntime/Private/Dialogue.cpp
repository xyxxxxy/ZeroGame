
#include "Dialogue.h"
#include "DialogueController.h"
#include "DialogueRuntimeLogChannels.h"
#include "DialogueSpeakerSocket.h"
#include "Nodes/DialogueNode.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(Dialogue)

FColor FDefaultDialogueColors::PopColor()
{
	const FColor TargetColor = DefaultColors[ColorIndex];
	ColorIndex++;
	ColorIndex %= DefaultColors.Num();
	return TargetColor;
}

UDialogue::UDialogue()
{
	AddDefaultSpeakers();
}

#if WITH_EDITOR
void UDialogue::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	
	if(PropertyChangedEvent.GetPropertyName().IsEqual("SpeakerRoles"))
	{
		OnChangeSpeakers(PropertyChangedEvent.ChangeType);
	}
}
#endif

void UDialogue::SetSpeaker(FName InName, UDialogueSpeakerComponent* InSpeaker)
{
	if(Speakers.Contains(InName))
	{
		Speakers[InName] = InSpeaker;
	}
}

UDialogueSpeakerComponent* UDialogue::GetSpeaker(FName InName) const
{
	if(Speakers.Contains(InName))
	{
		return Speakers[InName];
	}
	return nullptr;
}

void UDialogue::AddSpeakerEntry(FName InName)
{
	if(!Speakers.Contains(InName))
	{
		Speakers.Add(InName,nullptr);
	}
}

void UDialogue::OpenDialogue(ADialogueController* InController, TMap<FName, UDialogueSpeakerComponent*> InSpeakers)
{
	FString ErrorMessage;
	if(!CanPlay(InController, ErrorMessage))
	{
		UE_LOG(LogDialogueRuntime, Error, TEXT("Dialogue : Cannot play Dialogue. %s"), *ErrorMessage);
		return;
	}

	UE_LOG(LogDialogueRuntime, Warning, TEXT("Dialogue : OpenDialogue."));
	DialogueController = InController;
	FillSpeakers(InSpeakers);
	TraverseNode(RootNode);
}

void UDialogue::ClearController()
{
	DialogueController = nullptr;
}

void UDialogue::DisplaySpeech(const FSpeechDetails& InDetails)
{
	if (!Speakers[InDetails.SpeakerName])
	{
		EndDialogue();
		return;
	}

	DialogueController->DisplaySpeech(InDetails, Speakers[InDetails.SpeakerName]);
	//DialogueController->OnDialogueSpeechDisplayed.Broadcast(InDetails);
}

void UDialogue::DisplayOptions(TArray<FDialogueOption> InOptions) const
{
	if (!DialogueController)
	{
		UE_LOG(LogDialogueRuntime, Error, TEXT("Attempting to display options via missing dialogue controller. Aborting dialogue."));
		return;
	}

	TArray<FSpeechDetails> AllDetails;
	for (const FDialogueOption& Option : InOptions)
	{
		AllDetails.Add(Option.Details);
	}

	DialogueController->DisplayOptions(AllDetails);	
}

void UDialogue::SelectOption(int32 InOptionIndex) const
{
	if (ActiveNode)
	{
		ActiveNode->SelectOption(InOptionIndex);
	}	
}

void UDialogue::Skip() const
{
	if(ActiveNode)
	{
		ActiveNode->Skip();
	}
}

void UDialogue::EndDialogue()
{
	if(DialogueController)
	{
		DialogueController->EndDialogue();
	}
}

void UDialogue::TraverseNode(UDialogueNode* InNode)
{
	if(!DialogueController)
	{
		return;
	}
	
	if(!InNode)
	{
		EndDialogue();
		return;
	}

	MarkNodeVisited(InNode,true);
	ActiveNode = InNode;
	InNode->EnterNode();
}

EDialogueCompileStatus UDialogue::GetCompileStatus() const
{
	return CompileStatus;
}

TMap<FName, UDialogueSpeakerComponent*> UDialogue::GetAllSpeakers() const
{
	return Speakers;
}

bool UDialogue::SpeakerIsPresent(const FName SpeakerName) const
{
	return GetSpeaker(SpeakerName) != nullptr;
}

bool UDialogue::WasNodeVisited(UDialogueNode* TargetNode) const
{
	if(!DialogueController || !TargetNode || !DialogueNodes.Contains(TargetNode))
	{
		return false;
	}
	return DialogueController->WasNodeVisited(this,TargetNode->GetNodeIndex());
}

void UDialogue::MarkNodeVisited(UDialogueNode* TargetNode, bool bToBeVisited)
{
	if(!DialogueController || !TargetNode)
	{
		return;
	}

	if(bToBeVisited)
	{
		DialogueController->MarkNodeVisited(this,TargetNode->GetNodeIndex());
	}
	else
	{
		DialogueController->MarkNodeUnvisited(this,TargetNode->GetNodeIndex());
	}
}

void UDialogue::ClearAllNodeVisits()
{
	if(!DialogueController)
	{
		return;
	}

	DialogueController->ClearAllNodeVisitsForDialogue(this);
}

#if WITH_EDITOR
void UDialogue::AddNode(UDialogueNode* InNode)
{
	if(!DialogueNodes.Contains(InNode))
	{
		DialogueNodes.Add(InNode);
		InNode->SetDialogue(this);
	}
}

void UDialogue::RemoveNode(UDialogueNode* InNode)
{
	if(InNode)
	{
		DialogueNodes.Remove(InNode);
	}
}

void UDialogue::SetRootNode(UDialogueNode* InNode)
{
	RootNode = InNode;
}

UDialogueNode* UDialogue::GetRootNode() const
{
	return RootNode;
}

void UDialogue::SetGraph(UEdGraph* InGraph)
{
	Graph = InGraph;
}

UEdGraph* UDialogue::GetGraph()
{
	return Graph;
}

const TMap<FName, FSpeakerField>& UDialogue::GetSpeakerRoles() const
{
	return SpeakerRoles;
}

void UDialogue::ClearDialogue()
{
	RootNode = nullptr;
	DialogueNodes.Empty();
	Speakers.Empty();
	CompileStatus = EDialogueCompileStatus::UnCompiled;
}

void UDialogue::PreCompileDialogue()
{
	ClearDialogue();

	for(auto& Entry : SpeakerRoles)
	{
		AddSpeakerEntry(Entry.Key);
	}
}

void UDialogue::PostCompileDialogue()
{
	for(int32 NodeIndex = 0; NodeIndex < DialogueNodes.Num(); ++NodeIndex)
	{
		DialogueNodes[NodeIndex]->SetNodeIndex(NodeIndex);
	}
}

void UDialogue::SetCompileStatus(EDialogueCompileStatus InStatus)
{
	CompileStatus = InStatus;
	MarkPackageDirty(); //need to save
}
#endif

void UDialogue::AddDefaultSpeakers()
{
	//Default NPC
	UDialogueSpeakerSocket* NPCSpeaker = CreateDefaultSubobject<UDialogueSpeakerSocket>("DefaultSpeakerNPC");
	NPCSpeaker->SetSpeakerName("NPC");
	FSpeakerField NPCField;
	NPCField.GraphColor = DefaultSpeakerColors.PopColor();
	NPCField.SpeakerSocket = NPCSpeaker;

	SpeakerRoles.Add(NPCSpeaker->GetSpeakerName(), NPCField);

	//Default player
	UDialogueSpeakerSocket* PlayerSpeaker = CreateDefaultSubobject<UDialogueSpeakerSocket>("DefaultSpeakerPlayer");
	PlayerSpeaker->SetSpeakerName("Player");
	FSpeakerField PlayerField;
	PlayerField.GraphColor = DefaultSpeakerColors.PopColor();
	PlayerField.SpeakerSocket = PlayerSpeaker;

	SpeakerRoles.Add(PlayerSpeaker->GetSpeakerName(), PlayerField);
}

void UDialogue::OnChangeSpeakers(const EPropertyChangeType::Type& ChangeType)
{
	if(ChangeType == EPropertyChangeType::ArrayAdd)
	{
		OnAddSpeaker();
	}
	else if(ChangeType == EPropertyChangeType::ArrayRemove)
	{
		OnRemoveSpeaker();
	}
	else
	{
		OnChangeSingleSpeaker();
	}
	
	OnSpeakerRolesChanged.ExecuteIfBound();
}

void UDialogue::OnAddSpeaker()
{
	for(auto& Entry : SpeakerRoles)
	{
		FSpeakerField& Value = Entry.Value;
		if(!Value.SpeakerSocket)
		{
			Value.SpeakerSocket = NewObject<UDialogueSpeakerSocket>(this);
			Value.SpeakerSocket->SetSpeakerName(Entry.Key);

			Value.GraphColor = DefaultSpeakerColors.PopColor();
		}
	}
}

void UDialogue::OnRemoveSpeaker()
{
}

void UDialogue::OnChangeSingleSpeaker()
{
	for(auto& Entry : SpeakerRoles)
	{
		FSpeakerField& Value = Entry.Value;
		// check SpeakerSocket
		if(Value.SpeakerSocket->GetSpeakerName() != Entry.Key)
		{
			Value.SpeakerSocket->SetSpeakerName(Entry.Key);
		}
	}
}

bool UDialogue::CanPlay(ADialogueController* InController, FString& OutErrorMessage) const
{
	if (CompileStatus != EDialogueCompileStatus::Compiled)
	{
		OutErrorMessage = "Dialogue is not compiled.";
		return false;
	}
	if (!InController)
	{
		OutErrorMessage = "No valid controller provided.";
		return false;
	}
	if (!RootNode)
	{
		OutErrorMessage = "Entry node does not exist.";
		return false;
	}

	return true;
}

void UDialogue::FillSpeakers(TMap<FName, UDialogueSpeakerComponent*> InSpeakers)
{
	for(auto& Entry : Speakers)
	{
		Entry.Value = nullptr;
	}

	for(auto& Entry : InSpeakers)
	{
		if(Speakers.Contains(Entry.Key) && Entry.Value != nullptr)
		{
			Speakers[Entry.Key] = Entry.Value;
		}
	}

	for(auto& Entry : Speakers)
	{
		if(!Entry.Value)
		{
			DialogueController->HandleMissingSpeaker(Entry.Key);
		}
	}
}

UDialogueNode* UDialogue::GetActiveNode() const
{
	return ActiveNode;
}
