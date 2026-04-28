#ifndef UICONTAINER_H
#define UICONTAINER_H
#include <Adafruit_SSD1306.h>
#include "Screen.h"
#include "ScreenType.h"

class UIContainer {
public:
    UIContainer(Adafruit_SSD1306* display);
    void init();
    void tick();
    void draw();
    void changeScreen(SCREEN_TYPE type, int parameter = 0);
private:
    Adafruit_SSD1306* _display;
    Screen* _currentScreen;
};
#endif
