/**
 * Copyright (c) 2025 Guy Taylor
 * Modified for ZuluSCSI RP2040 Custom Firmware
**/

#if defined(CONTROL_BOARD)

#include "ZuluSCSI_log.h"
#include "SystemMode.h"
#include "MainScreen.h"
#include "SettingsScreen.h"
#include "SplashScreen.h"
#include "BrowseTypeScreen.h"
#include "BrowseScreen.h"
#include "BrowseScreenFlat.h"
#include "InfoScreen.h"
#include "InfoPage2Screen.h"
#include "InfoPage3Screen.h"
#include "InfoPage4Screen.h"
#include "MessageBox.h"
#include "CopyScreen.h"
#include "InitiatorMainScreen.h"
#include "NoControlsErrorScreen.h"
#include "UIContainer.h"

// FORCE LE DELAI ICI (10 secondes)
#undef SCREENSAVER_DELAY
#define SCREENSAVER_DELAY 10000

UIContainer* g_uiContainer = NULL;
Screen* g_activeScreen = NULL;
SCREEN_TYPE g_previousScreen = SCREEN_NONE;
int g_previousIndex = 0;

UIContainer::UIContainer()
{
    g_uiContainer = this;
    _lastActivityTime = millis();
    _screenSaverActive = false;
}

void UIContainer::update()
{
    if (g_activeScreen != NULL)
    {
        g_activeScreen->update();
    }

    // Gestion du Screen Saver
    if (!_screenSaverActive && (millis() - _lastActivityTime > SCREENSAVER_DELAY))
    {
        _screenSaverActive = true;
        // Ici le logo s'active
    }
}

void UIContainer::activity()
{
    _lastActivityTime = millis();
    if (_screenSaverActive)
    {
        _screenSaverActive = false;
        // Ici on revient au menu
    }
}

// Nettoyage des Switch pour la compilation
const char* GetScreenName(SCREEN_TYPE type)
{
    switch(type)
    {
        case SCREEN_MAIN:        return "Main";
        case SCREEN_SETTINGS:    return "Settings";
        case SCREEN_INFO:        return "Info (1/3)";
        case SCREEN_BROWSE:      return "Browse";
        case MESSAGE_BOX:        return "Message Box";
        case SCREEN_INSERT_SD_CARD: return "Insert SD"; // Correction compil
        default:                 return "Unknown";
    }
}

void changeScreen(SCREEN_TYPE type, int index)
{
    if (type == SCREEN_NONE)
    {
        g_activeScreen = NULL;
        return;
    }
    g_activeScreen = GetScreen(type);
    if (index == -1) index = g_previousIndex;
    g_activeScreen->init(index);
}

#endif
