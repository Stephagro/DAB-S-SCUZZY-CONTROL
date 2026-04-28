#ifndef UICONTAINER_H
#define UICONTAINER_H

#include <Arduino.h>
#include "Screen.h"
#include "ScreenType.h"

class UIContainer {
public:
    UIContainer(); // Correspond à ta ligne 36
    void update(); // Correspond à ta ligne 43
    void activity(); // Correspond à ta ligne 60
    
private:
    unsigned long _lastActivityTime;
    bool _screenSaverActive;
};

// Fonctions globales appelées dans ton .cpp
const char* GetScreenName(SCREEN_TYPE type);
void changeScreen(SCREEN_TYPE type, int index = 0);

#endif
