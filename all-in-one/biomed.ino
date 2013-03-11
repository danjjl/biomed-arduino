#include "biomed.h"

extern LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup(){
	Serial.begin(9600);
	lcd.begin(16, 2);// start the lcd
	//DS1307.begin();

	pinMode(WEIGHT_SENSOR, INPUT);
	
	pinMode(TSIC_POWER_PIN, OUTPUT);
	pinMode(TSIC_SIGNAL_PIN, INPUT);

	pinMode(SIZE_TRIG, OUTPUT);
	pinMode(SIZE_ECHO, INPUT);
}

void loop(){
	int cursor = BTN_LEFT;
	int lcdKey = BTN_NONE;

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Choisissez");
	lcd.setCursor(0,1);
	lcd.print("BMI");
	lcd.setCursor(9,1); // move cursor to second line "1" and 9 spaces over
	lcd.print("Tout");
	lcd.setCursor(3,1);
	lcd.blink();
delay(2000);

	do{
		if(Serial.available() && Serial.read() == 's'){
			sync();
			break;
		}

		lcdKey = read_LCD_buttons(lcdKey); // read the input

		switch (lcdKey){ // depending on which button was pushed, we perform an action
			case BTN_RIGHT:
				lcd.setCursor(13,1);
				lcd.blink();
				cursor = BTN_RIGHT;
				break;
			case BTN_LEFT:
				lcd.setCursor(3,1);
				lcd.blink();
				cursor = BTN_LEFT;
				break;
			case BTN_SELECT:
				lcd.noBlink();
				if(cursor == BTN_LEFT)
					getBmi();
				else
					tout();
				cursor = BTN_SELECT;
				break;
		}
	}while(cursor != BTN_SELECT);
}

// select the pins used on the LCD panel
// Utilise les pins 4, 5, 6, 7, 8 et 9.
// Pin 10 communique avec la LCD. Analog pin 0 lit les boutons.

// read the buttons
int read_LCD_buttons(int currentKey){
	int inputKey = analogRead(0);// read the value from the sensor 

// button possible values: 0, 144, 329, 504, 741 (approx : +-50)
	if (inputKey > 1000)
		return currentKey; // We make this the 1st option for speed reasons since it will be the most likely result
	else if (inputKey < 50)
		return BTN_RIGHT;
	else if (inputKey < 195)
		return BTN_UP;
	else if (inputKey < 380)
		return BTN_DOWN;
	else if (inputKey < 555)
		return BTN_LEFT;
	else if (inputKey < 790)
		return BTN_SELECT;
	else
		return currentKey;
}

void getBmi(){
	int taille = getSize();
	int weight = getWeight();
	float bmi = float(taille)/float(weight*weight);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("BMI");
	lcd.setCursor(0,1);
	lcd.print(bmi);
	delay(2000);
}

void tout(){
	int taille = getSize();
	int weight = getWeight();
	float temperature = getTemperature();
	int frequence = 80;
	int user = selectUser();
	
	writeEeprom(user, taille, weight, temperature, frequence);
}

void writeEeprom(byte utilisateur, byte taille, byte weight, float temperature, byte frequence)
{
	int address = 0;
	int RTCValues[7];

	while(address < 1000 && EEPROM.read(address) != 0)
		address++;
	if(address == 1000 || EEPROM.read(address + 10) != 0){
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Memoire pleine");
		delay(2000);
	}
	else{
//TODO REMOVE IN RELEASE(module not at home
RTCValues[0] = 13;
RTCValues[1] = 3;
RTCValues[2] = 10;
RTCValues[4] = 19;
RTCValues[5] = 20;
		Serial.println(utilisateur);
		Serial.println(taille);//Il faut q'un check de bonne valeur ai été fait -> 0 pas une bonne valeur
		Serial.println(weight);
		Serial.println(byte((temperature - 35.0)*10));//sur pc ca à l'effet désiré mais faut tester
		Serial.println(frequence);
		Serial.println(RTCValues[0]);//Year: two-digit, from 00 to 99
		Serial.println(RTCValues[1]);//Month: two-digit, from 01 to 12
		Serial.println(RTCValues[2]);//Day of month, from 01 to 31
		Serial.println(RTCValues[4]);//Hour: 24-hour format, from 0 to 23
		Serial.println(RTCValues[5]);//Minute: from 0 to 59
		EEPROM.write(address, utilisateur);
		EEPROM.write(address + 1, taille);//Il faut q'un check de bonne valeur ai été fait -> 0 pas une bonne valeur
		EEPROM.write(address + 2, weight);
		EEPROM.write(address + 3, byte((temperature - 35.0)*10));//sur pc ca à l'effet désiré mais faut tester
		EEPROM.write(address + 4, frequence);
		EEPROM.write(address + 5, RTCValues[0]);//Year: two-digit, from 00 to 99
		EEPROM.write(address + 6, RTCValues[1]);//Month: two-digit, from 01 to 12
		EEPROM.write(address + 7, RTCValues[2]);//Day of month, from 01 to 31
		EEPROM.write(address + 8, RTCValues[4]);//Hour: 24-hour format, from 0 to 23
		EEPROM.write(address + 9, RTCValues[5]);//Minute: from 0 to 59
	}
}

int selectUser(){
	byte id[MAX_USERS];
	char names[MAX_USERS][6];
	int numUsers = 0;
	char value = 1;
	int lcdKey = BTN_NONE;
	int cursor = 0;

	//Sauve les utilsateurs dans un tableau
	for(int i=999; i>=6 && value!=0; i-=7){
		value = EEPROM.read(i);
		if(value != 0){
			id[numUsers] = value;
Serial.println(id[numUsers]);
			for(int j=0; j<6; j++)
				names[numUsers][j] = EEPROM.read(i-j-1);
Serial.println(names[numUsers]);
			numUsers++;
		}
	}
	id[numUsers] = 255;
	names[numUsers][0] = 'N';
	names[numUsers][1] = 'U';
	names[numUsers][2] = 'S';
	names[numUsers][3] = 'E';
	names[numUsers][4] = 'R';
	names[numUsers][5] = ' ';
	numUsers++;

	printUsers(0, numUsers, names);

	do{
		lcdKey = BTN_NONE;
		lcdKey = read_LCD_buttons(lcdKey); // read the input

		switch (lcdKey){ // depending on which button was pushed, we perform an action
			case BTN_UP:
				cursor--;
				if(cursor < 0)
					cursor = numUsers - 1;
				printUsers(cursor, numUsers, names);
				break;
			case BTN_DOWN:
				cursor++;
				if(cursor >= numUsers)
					cursor = 0;
				printUsers(cursor, numUsers, names);
				break;
		}
	}while(lcdKey != BTN_SELECT);

	return id[cursor];
}
void printUsers(int user, int numUsers, char names[MAX_USERS][6]){
	lcd.clear();
	for(int j=0; j<2; j++){
		lcd.setCursor(0,j);
		lcd.print(names[(user+j) % numUsers][0]);
		lcd.print(" ");
		for(int i=1; i<6; i++)
			lcd.print(names[(user+j) % numUsers][i]);
		if(j==0)
			lcd.blink();
	}
}
