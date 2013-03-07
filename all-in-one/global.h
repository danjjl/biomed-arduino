#ifndef GLOBAL_H
#define GLOBAL_H

#include "Arduino.h"
#include "LiquidCrystal.h"
// select the pins used on the LCD panel
// Utilise les pins 4, 5, 6, 7, 8 et 9.
// Pin 10 communique avec la LCD. Analog pin 0 lit les boutons.
extern LiquidCrystal lcd;

#define TSIC_SIGNAL_PIN 1
#define TSCI_POWER_PIN 2

#define WEIGHT_SENSOR 3

#define SIZE_ECHO 4
#define SIZE_TRIG 5

#endif
