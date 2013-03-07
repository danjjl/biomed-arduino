#include "temperature.h"

float getTemperature(){
	uint16_t temp_value1 = 0;
	uint16_t temp_value2 = 0;
	float celsius = 0.0;
	bool ok = true;

	lcd.clear();//Wait thermalisation
	lcd.setCursor(0,0);
	lcd.print("thermalisation :");
	for(int i=0; i<TSIC_WAIT_TIME; i++){
		lcd.setCursor(0,1);
		lcd.print(TSIC_WAIT_TIME - i);
		delay(1000);
	}
	
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
			ok = false;

	//check parity for byte 2
	parity = 0;
	for(uint8_t i = 0; i < 9; i++)
		if(temp_value2 & (1 << i))
			parity++;
		if(parity % 2)
			ok = false;
	temp_value1 >>= 1;// delete parity bit
	temp_value2 >>= 1;// delete parity bit
	uint16_t temp_Temperature = (temp_value1 << 8) | temp_value2;

	celsius = ( (float)temp_Temperature / 2047 * 7000) - 1000;// conversion equation from TSic's data sheet

	if(!ok){
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Erreur lecture");
		lcd.setCursor(0,1);
		lcd.print(":'(");
		delay(2000);
	}
	else{
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Temperature : ");
		lcd.setCursor(0,1);
		lcd.print(celsius, 1);
		lcd.setCursor(5,1);
		lcd.print("°C");
		delay(2000);
	}
	
	return celsius;
}
