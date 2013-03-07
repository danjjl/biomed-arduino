#include "weight.h"

int getWeight(){
	float voltage;
	float weight;
	int sensorValue = 0;

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Montez sur");
	lcd.setCursor(0,1);
	lcd.print("la balance");
	delay(3000); //Attend 3s
	
	for(unsigned int i=0; i<WEIGHT_MESURES; i++){
		sensorValue = analogRead(WEIGHT_SENSOR);
		delayMicroseconds(333); //Wait 0.3s
	}
	sensorValue /= WEIGHT_MESURES;
	voltage=(sensorValue/1023.0)*5;
	weight = (voltage+0.1461)/0.0155;

	if(weight < 20 || weight > 100){
		weight = 170;
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
		lcd.print("Poids : ");
		lcd.setCursor(0,1);
		lcd.print(weight, 0);
		lcd.setCursor(4,1);
		lcd.print("kg");
		delayMicroseconds(2000);
	}
	return int(weight);
}
