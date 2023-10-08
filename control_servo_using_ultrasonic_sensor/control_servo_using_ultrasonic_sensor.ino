
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

#include <Servo.h>

Servo servo1;  // create servo object to control a servo


void setup() {
  // put your setup code here, to run once:
  servo1.attach(6,600,2300);  // (pin, min, max)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;

  if(distance<100)
  {
    Serial.println(distance);
    servo1.write(180);              
    delay(500); 
  }
  else
  {
    servo1.write(0);              
    delay(500); 
    Serial.println(distance);
  }

}
