
#include "LyraGameplayTags.h"



namespace LyraTag
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Time_Message,               "Event.Time.Message", "Generic Time Event channel");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Time_Message_Paused,        "Event.Time.Message.Paused", "Global Game Time is now Paused");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Time_Message_Unpaused,      "Event.Time.Message.Unpaused", "Global Game Time is now Unpaused");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look,          "InputTag.Look", "Player Look");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move,          "InputTag.Move", "Player Move");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Time_Start,    "InputTag.Time.Start", "Start Game Time");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Time_Stop,     "InputTag.Time.Stop", "Stop Game Time");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Zoom,          "InputTag.Zoom", "Zoom Camera");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_OpenMenu,          "InputTag.OpenMenu", "Open Menu");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Inventory,          "InputTag.Inventory", "Open Inventory");

}