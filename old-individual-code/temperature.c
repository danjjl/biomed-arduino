#include "Arduino.h"

#define TSCI_POWER_PIN 5// Vin
#define TSIC_SIGNAL_PIN 7// Signal Pin

#define TSIC_ON() digitalWrite(TSCI_POWER_PIN, HIGH)// Power up the TSIC-Sensor
#define TSIC_OFF() digitalWrite(TSCI_POWER_PIN, LOW)// Power down the TSIC-Sensor

#define TSIC_SIGNAL_HIGH digitalRead(TSIC_SIGNAL_PIN)//if TSIC_SIGNAL_PIN is high (=1)
#define TSIC_SIGNAL_LOW !digitalRead(TSIC_SIGNAL_PIN)//if TSIC_SIGNAL_PIN is low (=0)

void setup(){
	Serial.begin(9600);
	pinMode(TSCI_POWER_PIN, OUTPUT);// sets the digital pin as output
	pinMode(TSIC_SIGNAL_PIN, INPUT);// sets the digital pin as input
}

//Main loop
void loop(){
	float temperature;
	if(getTSicTemp(&temperature)){
		Serial.println(temperature);
	}
	delay(500);// delay in between reads for stability
}

bool getTSicTemp(float *temperature){
	uint16_t temp_value1 = 0;
	uint16_t temp_value2 = 0;
	float celsius = 0.0;

	TSIC_ON();

	delayMicroseconds(60);// wait for stabilization

	while(TSIC_SIGNAL_HIGH);// wait until start bit starts
	while(TSIC_SIGNAL_LOW);// wait, TStrobe

	// first data byte
	// read 8 data bits and 1 parity bit
	for(uint8_t i = 0; i < 9; i++) {
		while(TSIC_SIGNAL_HIGH);// wait for falling edge
		delayMicroseconds(60);
		if(TSIC_SIGNAL_HIGH)
			temp_value1 |= 1 << (8-i);// get the bit
		else
			while(TSIC_SIGNAL_LOW);// wait until line comes high again
	}

	// second byte
	while(TSIC_SIGNAL_HIGH);// wait until start bit starts
	while(TSIC_SIGNAL_LOW);// wait, TStrobe
	// read 8 data bits and 1 parity bit
	for(uint8_t i = 0; i < 9; i++){
		while(TSIC_SIGNAL_HIGH);// wait for falling edge
		delayMicroseconds(60);
		if(TSIC_SIGNAL_HIGH)
			temp_value2 |= 1 << (8-i);// get the bit
		else
			while (TSIC_SIGNAL_LOW);// wait until line comes high again
	}

	TSIC_OFF();// switch TSic off

	// check parity for byte 1
	uint8_t parity = 0;
	for(uint8_t i = 0; i < 9; i++)
		if(temp_value1 & (1 << i))
			parity++;
	if(parity % 2)
		return false;

	//check parity for byte 2
	parity = 0;
	for(uint8_t i = 0; i < 9; i++)
		if(temp_value2 & (1 << i))
			parity++;
	if(parity % 2)
		return false;
	temp_value1 >>= 1;// delete parity bit
	temp_value2 >>= 1;// delete parity bit
	uint16_t temp_Temperature = (temp_value1 << 8) | temp_value2;

	celsius = (float)temp_Temperature / 2047 * 70 - 10;// conversion equation from TSic's data sheet
	*temperature = celsius; //return temperture in °C

	return true;
}
