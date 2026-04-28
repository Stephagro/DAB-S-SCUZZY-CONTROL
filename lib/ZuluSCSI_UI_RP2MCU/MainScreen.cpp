/**
 * Copyright (c) 2025 Guy Taylor
 * Modified for ZuluSCSI RP2040 - ASR-10 Optimized
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

  // LOGIQUE PERSONNALISÉE : Priorité à l'ID 4 pour le sampleur
  if (index == SCREEN_ID_NO_PREVIOUS || index == SCREEN_ID_UNINITIALIZED || _selectedDevice == NO_DEVICE_SELECTED)
  {
    _selectedDevice = NO_DEVICE_SELECTED;
    
    // On vérifie d'abord si l'ID 4 est actif (Ton disque dur fixe)
    if (g_devices[4].Active) {
        _selectedDevice = 4;
    } else {
        // Sinon, on cherche le premier disponible comme avant
        for (int i = 0; i < S2S_MAX_TARGETS; i++)
        {
          if (g_devices[i].Active)
          {
            _selectedDevice = i;
            break;
          }
        }
    }
  }

  if (_selectedDevice != NO_DEVICE_SELECTED)
  {
    _deviceMap = &g_devices[_selectedDevice];
  }
}

void MainScreen::update()
{
  // Gestion de l'affichage standard
  forceDraw();
}

void MainScreen::onNext()
{
  int i;
  bool found = false;

  // Recherche du prochain ID actif (vers le haut)
  for (i = _selectedDevice + 1; i < S2S_MAX_TARGETS; i++)
  {
    if (g_devices[i].Active)
    {
      _selectedDevice = i;
      found = true;
      break;
    }
  }

  if (!found)
  {
    for (i = 0; i <= _selectedDevice; i++)
    {
      if (g_devices[i].Active)
      {
        _selectedDevice = i;
        found = true;
        break;
      }
    }
  }

  if (found)
  {
    _deviceMap = &g_devices[_selectedDevice];
    forceDraw();
  }
}

void MainScreen::onPrevious()
{
  int i;
  bool found = false;

  // Recherche du prochain ID actif (vers le bas)
  for (i = _selectedDevice - 1; i >= 0; i--)
  {
    if (g_devices[i].Active)
    {
      _selectedDevice = i;
      found = true;
      break;
    }
  }

  if (!found)
  {
    for (i = S2S_MAX_TARGETS - 1; i >= _selectedDevice; i--)
    {
      if (g_devices[i].Active)
      {
        _selectedDevice = i;
        found = true;
        break;
      }
    }
  }

  if (found)
  {
    _deviceMap = &g_devices[_selectedDevice];
    forceDraw();
  }
}

void MainScreen::drawSCSIItem(int x, int y, int index)
{
  DeviceMap *map = &g_devices[index];

  _display->setCursor(x+10, y+2);             
  _display->print((int)index); 

  if (_selectedDevice == index)
  {
    _display->drawBitmap(x, y+1, icon_select, 8,8, WHITE);
  }

  if (map->Active)
  {
    // Affiche l'icône de disque si actif
    _display->drawBitmap(x+22, y+1, icon_disk, 8,8, WHITE);
  }
}

// Nettoyage final pour le compilateur
void MainScreen::forceDraw()
{
    _display->clearDisplay();
    // Dessine la liste des 8 IDs SCSI
    for (int i = 0; i < 8; i++) {
        int column = i / DEVICES_PER_COLUMN;
        int row = i % DEVICES_PER_COLUMN;
        drawSCSIItem(column * 64, row * 16, i);
    }
    _display->display();
}

#endif
