#include "size.h"

float getDuration(){
	float duration = 0;

	for(unsigned int i=0; i<SIZE_MESURES; i++){
		digitalWrite(SIZE_TRIG, HIGH);
		delayMicroseconds(10); //10µs HIGH -> sends 8 wavelenght
		digitalWrite(SIZE_TRIG, LOW);
		duration += pulseIn(SIZE_ECHO, HIGH);
		delay(100); //Wait 0.1s
	}
	return duration /= SIZE_MESURES;
}

int getSize(){
	float duration;
	float calibrationFactor;
	float taille = 0;
	int maxTrials = 0;

	//Calibration mesure
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Calibration : ");
	lcd.setCursor(0,1);
	lcd.print("quitter balance");
	delay(2000); //Attend 2s

	duration = getDuration();
	calibrationFactor = SIZE_FLOOR_DIST / duration;

	//Size measures
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Remontez sur");
	lcd.setCursor(0,1);
	lcd.print("la balance");
	delay(2000); //Attend 2s
	taille = getDuration() * calibrationFactor;
	taille = SIZE_SENSOR_HEIGHT - taille;

	//distance = ((duration/2) / (29.1))*(29/33.57); //TODO Clean const (and/or calibrate) PLUS UTILE
	//distance += 0.15918*distance+1.9013; // correction erreur PAS SUR ENCORE UTILE
Serial.print(taille);
	if(taille <= 10.0 || taille > 200.0){ //Check distance is possible
		taille = 170;
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
		lcd.print("Taille : ");
		lcd.setCursor(0,1);
		lcd.print(taille, 0);
		lcd.setCursor(4,1);
		lcd.print("cm");
		delay(2000);
	}

	return int(taille) + 1;
}
