//VCC 5V

#define trigPin 12 //Trigger pin
#define echoPin 11 //Echo pin

void setup(){
	Serial.begin (9600);//Initialize USB connection
	pinMode(trigPin, OUTPUT);//Initialize pins
	pinMode(echoPin, INPUT);
}

void loop(){
	float distance;
	if(getDistance(&distance)){
		Serial.print(distance);
		Serial.println(" cm");
	}
	delay(500);

}

bool getDistance(float *distance){
	float duration;

	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);//10µs HIGH -> sends 8 wavelenght
	digitalWrite(trigPin, LOW);

	duration = pulseIn(echoPin, HIGH);
	*distance = ((duration/2) / (29.1))*(29/33.57);//TODO Clean const (and/or calibrate)
	*distance = 0.15918*(*distance)+1.9013; // correction erreur

	if(*distance > 200 || *distance < 0)//Check distance is possible
		return false;
	else
		return true;
}
