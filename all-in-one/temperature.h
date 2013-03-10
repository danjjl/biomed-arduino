#ifndef TEMP_H
#define TEMP_H

#include "global.h"

#define TSIC_ON() digitalWrite(TSIC_POWER_PIN, HIGH)// Power up the TSIC-Sensor
#define TSIC_OFF() digitalWrite(TSIC_POWER_PIN, LOW)// Power down the TSIC-Sensor

#define TSIC_SIGNAL_HIGH digitalRead(TSIC_SIGNAL_PIN)//if TSIC_SIGNAL_PIN is high (=1)
#define TSIC_SIGNAL_LOW !digitalRead(TSIC_SIGNAL_PIN)//if TSIC_SIGNAL_PIN is low (=0)

#define TSIC_WAIT_TIME 3


float getTemperature();

#endif
