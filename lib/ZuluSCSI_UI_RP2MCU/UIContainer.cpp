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

// TES PARAMÈTRES PERSONNELS
#undef SCREENSAVER_DELAY
#define SCREENSAVER_DELAY 10000 

UIContainer* g_uiContainer = NULL;
Screen* g_activeScreen = NULL;
SCREEN_TYPE g_previousScreen = SCREEN_NONE;
int g_previousIndex = 0;

// Cette fonction manquait et faisait tout planter
Screen* GetScreen(SCREEN_TYPE type) {
    switch (type) {
        case SCREEN_SPLASH:      return new SplashScreen();
        case SCREEN_MAIN:        return new MainScreen();
        case SCREEN_SETTINGS:    return new SettingsScreen();
        default:                 return new MainScreen(); 
    }
}

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

    if (!_screenSaverActive && (millis() - _lastActivityTime > SCREENSAVER_DELAY))
    {
        _screenSaverActive = true;
    }
}

void UIContainer::activity()
{
    _lastActivityTime = millis();
    if (_screenSaverActive)
    {
        _screenSaverActive = false;
    }
}

const char* GetScreenName(SCREEN_TYPE type)
{
    switch(type)
    {
        case SCREEN_MAIN:        return "Main";
        case SCREEN_SETTINGS:    return "Settings";
        case SCREEN_INFO:        return "Info";
        case SCREEN_BROWSE:      return "Browse";
        default:                 return "Menu";
    }
}

void changeScreen(SCREEN_TYPE type, int index)
{
    if (g_activeScreen != NULL) {
        delete g_activeScreen;
    }
    g_activeScreen = GetScreen(type);
}

#endif
