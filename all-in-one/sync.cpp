#include "sync.h"

void sync(){
	int address = 0;
	byte value = 1;

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Synchronisation");
	lcd.setCursor(12,1);
	lcd.print("...");

	//Read Values from memory
	while(address < 1000 && value != 0) {
		value = EEPROM.read(address);
		EEPROM.write(address, 0);
		Serial.write(value);
		delay(100);
		address ++;
	}

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Sent mesures");
	lcd.setCursor(12,1);
	lcd.print("...");
	
	value = 1;
	address = 999;
	while(address >= 0 && value != 0){
		value = EEPROM.read(address);
		EEPROM.write(address, 0);
		address --;
	}

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Receive users");
	lcd.setCursor(12,1);
	lcd.print("...");


	value = 1;
	address = 999;
	while(address >= 0 && value != '0'){
		if(Serial.available() > 0){
			value = Serial.read();
			if(value != '0'){
				EEPROM.write(address, value);
				address --;
			}
		}
	}
}
