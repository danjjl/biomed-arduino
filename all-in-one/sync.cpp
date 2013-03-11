#include "sync.h"

void sync(){
	int address = 0;
	byte value = 1;

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Synchronisation");
	lcd.setCursor(12,1);
	lcd.print("...");

	Serial.print('r');

	//Read Values from memory
	while(address < 1000 && value != 0) {
		if(Serial.available()) {
			Serial.read();
			value = EEPROM.read(address);
			EEPROM.write(address, 0);
			Serial.println(value);
			delay(100);
			address ++;
		}
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

	Serial.print('r');//Print ready
	value = 1;
	address = 999;
	while(address >= 0 && value != '0'){
		if(Serial.available()) {
			value = Serial.read();
			if(value != '0'){
				EEPROM.write(address, value);
				Serial.print('o');
				delay(100);
				address --;
			}
		}
	}
	Serial.print('f');
	
//TODO write Serial.read() and and erase users from db write users to end of eeprom
}
