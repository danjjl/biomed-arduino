#ifndef GLOBAL_H
#define GLOBAL_H

#include "Arduino.h"
#include "LiquidCrystal.h"
// select the pins used on the LCD panel
// Utilise les pins 4, 5, 6, 7, 8 et 9.
// Pin 10 communique avec la LCD. Analog pin 0 lit les boutons.
extern LiquidCrystal lcd;

#define TSIC_SIGNAL_PIN 0 //digital
#define TSIC_POWER_PIN 2 //digital

#define WEIGHT_POWER 1 //digital
#define WEIGHT_SENSOR 1 //analog

#define SIZE_POWER 11 //digital
#define SIZE_ECHO 12 //digital
#define SIZE_TRIG 3 //digital

#endif
