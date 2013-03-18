#include "size.h"

float getDuration(){
	float duration = 0;
	digitalWrite(SIZE_TRIG, LOW);
	delayMicroseconds(2);
	digitalWrite(SIZE_TRIG, HIGH);
	delayMicroseconds(10); //10µs HIGH -> sends 8 wavelenght
	digitalWrite(SIZE_TRIG, LOW);
	duration = pulseIn(SIZE_ECHO, HIGH);
	delay(100);
	return duration;
}

int getSize(){
	float duration;
	float calibrationFactor=0;
	float taille = 0;

	//Calibration mesure
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Calibration : ");
	lcd.setCursor(0,1);
	lcd.print("quitter balance");
	delay(1000); //Attend 2s

	digitalWrite(SIZE_POWER, HIGH);
	while (calibrationFactor < ((300.0/10000.0)/2.0) || calibrationFactor > ((400.0/10000.0)/2.0)){
		duration = getDuration();
		calibrationFactor = SIZE_FLOOR_DIST / duration;
	}

	//Size measures
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Remontez sur");
	lcd.setCursor(0,1);
lcd.print(calibrationFactor*10000*2);	
//lcd.print("la balance");
	delay(1000); //Attend 2s
	 while(taille < 10.0 || taille > 250.0){
		taille = getDuration() * calibrationFactor;
		taille = SIZE_FLOOR_DIST - taille;
		}

	digitalWrite(SIZE_POWER, LOW);

	Serial.print(taille);
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(duration/1000.0);
	lcd.setCursor(0,1);
	lcd.print(taille, 0);
	lcd.setCursor(5,1);
	lcd.print(calibrationFactor);
	delay(1000);


	return int(taille);
}
