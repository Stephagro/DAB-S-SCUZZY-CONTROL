/**
 * ZuluSCSI™ firmware - MainScreen Corrected Version
**/

#if defined(CONTROL_BOARD)

#include "ui.h"
#include "MainScreen.h"
#include "MessageBox.h"
#include "SplashScreen.h"
#include "ZuluSCSI_log.h"
#include "cache.h"

#define DEVICES_PER_PAGE 8
#define DEVICES_PER_COLUMN 4
#define NO_DEVICE_SELECTED -1

extern bool g_sdAvailable;

void MainScreen::init(int index)
{
  Screen::init(index);

  if (index == SCREEN_ID_NO_PREVIOUS || index == SCREEN_ID_UNINITIALIZED || _selectedDevice == NO_DEVICE_SELECTED)
  {
    _selectedDevice = NO_DEVICE_SELECTED;
    for (int i = 0; i < S2S_MAX_TARGETS; i++)
    {
      if (g_devices[i].Active)
      {
        _selectedDevice = i;
        break;
      }
    }
  }
  else
  {
    _selectedDevice = index;
  }

  _deviceMap = (_selectedDevice != NO_DEVICE_SELECTED) ? &g_devices[_selectedDevice] : NULL;
}

void MainScreen::draw()
{
  _display->setCursor(0,0);             
  _display->print(F("SCSI Map"));

  int page = 0;
  int deviceOffset = 0;
  if (_selectedDevice > NO_DEVICE_SELECTED)
  {
    page = (_selectedDevice/DEVICES_PER_PAGE);
    deviceOffset = page * DEVICES_PER_PAGE;
  }
  
  int totalPages = S2S_MAX_TARGETS / DEVICES_PER_PAGE;
  if (totalPages > 1)
  {
    _display->print(" (");
    _display->print((page + 1));
    _display->print("/");
    _display->print(totalPages);
    _display->print(")");
  }

  _display->drawLine(0,10,112,10, 1);
  _display->drawBitmap(115, 0, g_sdAvailable ? icon_sd : icon_nosd, 12,12, WHITE);

  int xOffset = 0;
  int yOffset = 13;
  for (int i = 0; i < DEVICES_PER_PAGE; i++)
  {
    if (i == DEVICES_PER_COLUMN)
    {
      xOffset = 64;
      yOffset = 13;
    }
    drawSCSIItem(xOffset, yOffset, i + deviceOffset);
    yOffset+=13;
  }
}

void MainScreen::tick() { Screen::tick(); }

void MainScreen::sdCardStateChange(bool cardIsPresent)
{
  _selectedDevice = NO_DEVICE_SELECTED;
  _deviceMap = NULL;
}

void MainScreen::shortRotaryPress()
{
  if (_selectedDevice != NO_DEVICE_SELECTED) changeScreen(SCREEN_INFO, _selectedDevice);
}

void MainScreen::shortUserPress() {}

void MainScreen::shortEjectPress()
{
  if (_selectedDevice == NO_DEVICE_SELECTED || _deviceMap == NULL) return;

  if (_deviceMap->BrowseMethod == BROWSE_METHOD_NOT_BROWSABLE) {
      _messageBox->setReturnScreen(SCREEN_MAIN);
      _messageBox->setText("-- Warning --", "Browsing not", "Supported...");
      changeScreen(MESSAGE_BOX, _selectedDevice);
  } else {
      changeScreen((_deviceMap->HasDirs && (!g_cacheActive || _deviceMap->BrowseScreenType == 0)) ? SCREEN_BROWSE : SCREEN_BROWSE_FLAT, _selectedDevice);
  }
}

void MainScreen::longUserPress() { changeScreen(SCREEN_SETTINGS, 0); }

void MainScreen::longEjectPress()
{
  if (_selectedDevice == NO_DEVICE_SELECTED || _deviceMap == NULL) return;
  changeScreen(SCREEN_BROWSE_TYPE, _selectedDevice); 
}

void MainScreen::rotaryChange(int direction)
{
  if (_selectedDevice == NO_DEVICE_SELECTED) return;
  
  int i;
  bool found = false;
  if (direction == 1) {
    for (i = _selectedDevice + 1; i < S2S_MAX_TARGETS; i++) {
      if (g_devices[i].Active) { _selectedDevice = i; found = true; break; }
    }
    if (!found) {
      for (i = 0; i < _selectedDevice; i++) {
        if (g_devices[i].Active) { _selectedDevice = i; found = true; break; }
      }
    }
  } else {
    for (i=_selectedDevice - 1; i >=0; i--) {
      if (g_devices[i].Active) { _selectedDevice = i; found = true; break; }
    }
    if (!found) {
      for (i = S2S_MAX_TARGETS - 1; i >= _selectedDevice; i--) {
        if (g_devices[i].Active) { _selectedDevice = i; found = true; break; }
      }
    }
  }

  if (found) {
    _deviceMap = &g_devices[_selectedDevice];
    draw(); // Correction : on utilise draw() au lieu de forceDraw() qui n'existe pas
  }
}

void MainScreen::drawSCSIItem(int x, int y, int index)
{
  DeviceMap *map = &g_devices[index];
  _display->setCursor(x+10, y+2);             
  _display->print((int)index); 

  if (_selectedDevice == index) _display->drawBitmap(x, y+1, icon_select, 8,8, WHITE);

  if (map->Active) {
    const uint8_t *deviceIcon = getIconForType(map->DeviceType, true);
    _display->drawBitmap(x+36, y, deviceIcon, 12,12, WHITE);
    _display->drawBitmap(x+22, y, icon_ledon, 12,12, WHITE);
  } else {
    _display->drawBitmap(x+22, y, icon_ledoff, 12,12, WHITE);
  }
}

#endif
