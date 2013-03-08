#define SENSOR_PIN 5// select the input pin for the potentiometer

void setup() {
	Serial.begin (9600);//Initialize USB connection
	pinMode(SENSOR_PIN, INPUT);
}

void loop() {
	float weight = 0;
	if(getWeight(&weight)){
		Serial.print(weight);
		Serial.println(" kg");
	}

	delay(1000);
}

bool getWeight(float *weight){
	int sensorValue = analogRead(SENSOR_PIN);
	float voltage = (sensorValue/1023)*5; //convert analogRead to voltage

	if(voltage > 1)
		*weight = 32.5*voltage - 32.5;//Pseudo conversion en kg

	Serial.print(voltage); //Print voltage (remove in final just debug info)
	Serial.print(" V, ");

	return true; //Can be used not to record faulty values
}
