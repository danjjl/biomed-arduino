#include "weight.h"

int getWeight(){
	digitalWrite(WEIGHT_POWER, HIGH);

	float voltage;
	float weight;
	int sensorValue = 0;
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Montez sur");
	lcd.setCursor(0,1);
	lcd.print("la balance");
	//delay(2000); //Attend 2s
	
	for(unsigned int i=0; i<WEIGHT_MESURES; i++){
		sensorValue += analogRead(WEIGHT_SENSOR);
		delay(333); //Wait 0.3s
	}
	digitalWrite(WEIGHT_POWER, LOW);

	sensorValue /= WEIGHT_MESURES;
	voltage=(sensorValue/1023.0)*5;
	weight = (voltage+CONST_B)/CONST_A;

	if(false){
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
		lcd.print(voltage);
		lcd.setCursor(0,1);
		lcd.print(weight, 0);
		lcd.setCursor(4,1);
		lcd.print("kg");
		delay(2000);
	}
	return int(weight);
}
