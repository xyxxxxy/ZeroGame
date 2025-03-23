#include "LyraNavigationConfig.h"

FLyraNavigationConfig::FLyraNavigationConfig()
{
        bTabNavigation = false;
        bKeyNavigation = false;
        KeyEventRules =
            {
                {EKeys::Gamepad_DPad_Left, EUINavigation::Left},
                {EKeys::Gamepad_DPad_Right, EUINavigation::Right},
                {EKeys::Gamepad_DPad_Up, EUINavigation::Up},
                {EKeys::Gamepad_DPad_Down, EUINavigation::Down}};
}