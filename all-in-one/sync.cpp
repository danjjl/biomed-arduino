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
	for(int address = 0; address < 1000 && value != 0; address++){
		value = EEPROM.read(address);
		if(value != 0){
			Serial.print(address);
			Serial.print("\t");
			Serial.print(value, DEC);
			Serial.println();

			//Erase value from memory
			EEPROM.write(address, 0);
		}
	}
//TODO write Serial.read() and write users to end of eeprom
}
