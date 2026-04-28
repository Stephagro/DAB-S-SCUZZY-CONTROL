/**
 * Copyright (c) 2025 Guy Taylor
 * Modified for ZuluSCSI RP2040 Custom Firmware
**/

#if defined(CONTROL_BOARD)

#include <Arduino.h>
#include "ScreenType.h" // C'est ici que SCREEN_TYPE est défini !
#include "UIContainer.h"
#include "MainScreen.h"
#include "SplashScreen.h"

// TON PARAMÈTRE PERSONNEL
#undef SCREENSAVER_DELAY
#define SCREENSAVER_DELAY 10000 

UIContainer* g_uiContainer = NULL;
Screen* g_activeScreen = NULL;

// On déplace la définition pour être sûr que le compilateur la voie
Screen* GetScreen(SCREEN_TYPE type) {
    switch (type) {
        case SCREEN_SPLASH:      return new SplashScreen();
        case SCREEN_MAIN:        return new MainScreen();
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
    _screenSaverActive = false;
}

const char* GetScreenName(SCREEN_TYPE type)
{
    return "Menu";
}

void changeScreen(SCREEN_TYPE type, int index)
{
    if (g_activeScreen != NULL) {
        delete g_activeScreen;
    }
    g_activeScreen = GetScreen(type);
}

#endif
