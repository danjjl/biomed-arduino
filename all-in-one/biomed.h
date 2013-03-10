#ifndef BIOMED_H
#define BIOMED_H

#include "global.h"
#include "sync.h"
#include "size.h"
#include "temperature.h"
#include "weight.h"

//#include "Wire.h"
#include "DS1307.h"

// define some values used by the panel and buttons
#define BTN_RIGHT 0
#define BTN_UP 1
#define BTN_DOWN 2
#define BTN_LEFT 3
#define BTN_SELECT 4
#define BTN_NONE 5

#define MAX_USERS 30 //j'aurais bien voulu une alloc dynamic, pas encore trouvé comment faire

int read_LCD_buttons(int currentKey);
void tout();
void getBmi();
void writeEeprom(byte utilisateur, byte size, byte weight, float temperature, byte frequence);
void printUsers(int user, int numUsers, char names[MAX_USERS][6]);

#endif
