
#include "UI/DialogueWidget.h"
#include "Components/RichTextBlock.h"
#include "DialogueRuntimeLogChannels.h"
#include "Input/CommonUIInputTypes.h"
#include "Transitions/InputDialogueTransition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueWidget)

const static FString Suffix = FString(TEXT("</>"));

UDialogueWidget::UDialogueWidget(const FObjectInitializer& ObjectInitializer)
 : Super(ObjectInitializer)
{
}

void UDialogueWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ForwardHandle = RegisterUIActionBinding(FBindUIActionArgs(ForwardInputActionData,true,FSimpleDelegate::CreateUObject(this,&ThisClass::Forward)));
	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData,true,FSimpleDelegate::CreateUObject(this,&ThisClass::Back)));
}


void UDialogueWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	check(DialogueController);

	OriginStr = OriginText.ToString();
	DisplayTextTimerDelegate.BindUObject(this,&UDialogueWidget::DisplayDialogue);
	DialogueController->OnStatementStart().AddUObject(this, &UDialogueWidget::StartNewStatement);
	DialogueController->OnStatementEnd().AddUObject(this, &UDialogueWidget::PreSwitchToNextStatement);
	InitialValuables();
	DialogueTextBlock->SetText(FText::FromString(TEXT("")));
}

void UDialogueWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	
	DialogueController->OnStatementStart().Clear();
	DialogueController->OnStatementEnd().Clear();
	GetWorld()->GetTimerManager().ClearTimer(DisplayTextTimerHandle);
	DisplayTextTimerDelegate.Unbind();
}

void UDialogueWidget::SetController_Implementation(ADialogueController* InController)
{
	check(InController);
	DialogueController = InController;
}

void UDialogueWidget::DisplayDialogue()
{
	if(StrIndex >= OriginStr.Len())
	{
		UE_LOG(LogDialogueRuntime, Warning , TEXT("Widget : StrIndex >= OriginStr.Len()!"));
		// OnStatementEnd
		DialogueController->OnStatementEnd().Broadcast();
		return;
	}

	if(OriginStr[StrIndex] == '<')
	{
		// start to enter rich block
		if(!bHasEntered)
		{
			FindFirstIndexAfterRich();
			if(StrIndex < OriginStr.Len() && OriginStr[StrIndex] == '<')
			{
				FindFirstIndexAfterRich();
				StrIndex--;
				CurrentStr = OriginStr.Left(StrIndex+1);
				DialogueTextBlock->SetText(FText::FromString(CurrentStr));
			}
			else
			{
				bHasEntered = true;
				CurrentStr = OriginStr.Left(StrIndex+1).Append(Suffix);
				StrIndex++;
				DialogueTextBlock->SetText(FText::FromString(CurrentStr));
			}
		}
		// start to exit rich block
		else
		{
			StrIndex++;
			FindFirstIndexAfterRich();
			bHasEntered = false;
			
			CurrentStr = OriginStr.Left(StrIndex+1);
			StrIndex++;
			DialogueTextBlock->SetText(FText::FromString(CurrentStr));
			
			return;
		}
	}
	// in rich block
	if(bHasEntered)
	{
		CurrentStr = OriginStr.Left(StrIndex+1).Append(Suffix);
		StrIndex++;
		DialogueTextBlock->SetText(FText::FromString(CurrentStr));
	}
	// out of rich block
	else
	{
		CurrentStr = OriginStr.Left(StrIndex+1);
		StrIndex++;
		DialogueTextBlock->SetText(FText::FromString(CurrentStr));
	}
}


void UDialogueWidget::FindFirstIndexAfterRich()
{
	while(StrIndex < OriginStr.Len() && OriginStr[StrIndex] != '>')
	{
		StrIndex++;
	}
	StrIndex++;
}

void UDialogueWidget::StartNewStatement(const FSpeechDetails& InDetails)
{
	// input transition
	OriginText = InDetails.SpeechText;
	if(OriginText.IsEmpty())
	{
		OriginText = FText::FromString(TEXT("------------[It's empty! check your DialogueTree]------------"));
	}
	OriginStr = OriginText.ToString();
	//DisplayRate = InDetails.
	bEntireStatement = false;
	GetWorld()->GetTimerManager().SetTimer(DisplayTextTimerHandle, DisplayTextTimerDelegate, DisplayRate, true);
}

void UDialogueWidget::PreSwitchToNextStatement()
{
	InitialValuables();
	GetWorld()->GetTimerManager().ClearTimer(DisplayTextTimerHandle);
}

void UDialogueWidget::InitialValuables()
{
	CurrentStr.Empty();
	OriginStr.Empty();
	StrIndex = 0;
	bHasEntered = false;
	bEntireStatement = true;
}

void UDialogueWidget::DisplayEntireStatement()
{
	GetWorld()->GetTimerManager().PauseTimer(DisplayTextTimerHandle);
	DialogueTextBlock->SetText(OriginText);
	PreSwitchToNextStatement();
}

void UDialogueWidget::OnSwitchToNextStatement()
{
	DialogueController->TransitionOut();
	
}

void UDialogueWidget::Forward()
{
	if(!bEntireStatement)
	{
		DisplayEntireStatement();		
	}
	else if(!DialogueController->GetTransition()->IsA<UInputDialogueTransition>())
	{
		OnSwitchToNextStatement();
	}
}





void UDialogueWidget::Back()
{
	DeactivateWidget();
}