const int s0Pin = 3;
const int s1Pin = 4;
const int s2Pin = 5;
const int s3Pin = 6;
const int signalPin = A0;  // The analog input pin connected to the SIG pin of 74HC4067

int last_channel = -1;
int total_sensors = 2;

void setup() {
  pinMode(s0Pin, OUTPUT);
  pinMode(s1Pin, OUTPUT);
  pinMode(s2Pin, OUTPUT);
  pinMode(s3Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for (int channel = 0; channel < total_sensors; channel++) {
    // Set the multiplexer channel
    digitalWrite(s0Pin, bitRead(channel, 0));
    digitalWrite(s1Pin, bitRead(channel, 1));
    digitalWrite(s2Pin, bitRead(channel, 2));
    digitalWrite(s3Pin, bitRead(channel, 3));

    // Read the sensor value
    int sensorValue = analogRead(signalPin);

    if(sensorValue<200)
    {
      if(last_channel == channel)
      {
        break;
      }
      last_channel = channel;
      Serial.print("IR Sensor ");
      Serial.print(++channel);
      Serial.println(" is bieng touched ");
    }
    
    delay(100); // Delay for better readability
  }
}