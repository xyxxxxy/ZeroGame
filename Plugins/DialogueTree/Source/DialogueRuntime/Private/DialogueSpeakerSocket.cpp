
#include "DialogueSpeakerSocket.h"
#include "Dialogue.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueSpeakerSocket)

void UDialogueSpeakerSocket::SetSpeakerName(FName InSpeakerName)
{
	SpeakerName = InSpeakerName;
}

FName UDialogueSpeakerSocket::GetSpeakerName() const
{
	return SpeakerName;
}

UDialogueSpeakerComponent* UDialogueSpeakerSocket::GetSpeakerComponent(UDialogue* InDialogue) const
{
	if(!InDialogue || SpeakerName.IsNone())
	{
		return nullptr;
	}

	return InDialogue->GetSpeaker(SpeakerName);
}

bool UDialogueSpeakerSocket::IsValidSocket() const
{
	if(SpeakerName.IsNone())
	{
		return false;
	}
	return true;
}
