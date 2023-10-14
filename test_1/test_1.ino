#include "set.h"
#include <Adafruit_NeoPixel.h>
#ifdef _AVR_
  #include <avr/power.h>
#endif

#define LEDPIN 13
#define NUMPIXELS 60
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 10

bool flow = 1;

const int s0Pin = 2;
const int s1Pin = 3;
const int s2Pin = 4;
const int s3Pin = 5;
const int signalPin = A0;  // The analog input pin connected to the SIG pin of 74HC4067

int last_channel = -1;
int total_sensors = 2;

uint32_t aliveColor = pixels.Color(0,0,255);
uint32_t deadColor = pixels.Color(255,0,0);
uint32_t noColor = pixels.Color(0,0,0);

Set ledDead;

int sectionNo = -1;


void setup()
{
  #if defined(_AVR_ATtiny85_) && (F_CPU == 16000000)
  	clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
  pixels.clear();
  pinMode(s0Pin, OUTPUT);
  pinMode(s1Pin, OUTPUT);
  pinMode(s2Pin, OUTPUT);
  pinMode(s3Pin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  livingStrip();
  readMux();
  switch(sectionNo) {
    case 0:
      sectionDead(0,12);
      break;
    case 1:
      sectionDead(12,24);
      break;
    case 2:
      sectionDead(24,36);
      break;
    case 3:
      sectionDead(36,48);
      break;
    case 4:
      sectionDead(48,60);
      break;
  }
}

void livingStrip()
{
  if(flow == 1)
  {
    addFlow();
  }
  else
  {
    removeFlow();
  }
}
void addFlow()
{
  for(int i=0; i<NUMPIXELS; i++) {
    if(ledDead.has(i))
    {
      continue;
    }
    pixels.setPixelColor(i, aliveColor);
    pixels.show();
    delay(DELAYVAL);
    if(i==(NUMPIXELS-1))
    {
      flow = 0;
    }
  }
}
void removeFlow()
{
  for(int i=0; i<NUMPIXELS; i++) 
  {
    if(ledDead.has(i))
    {
      continue;
    }
    pixels.setPixelColor(i, noColor);
    pixels.show();
    delay(DELAYVAL);
    if(i==(NUMPIXELS-1))
    {
      flow = 1;
    }
  }
}
void sectionDead(int start, int end) {
  for(int i = start; i<end ; i++)
    {
      ledDead.add(i);
      pixels.setPixelColor(i, deadColor);
      pixels.show();
    }
}

void readMux()
{
  for (int channel = 0; channel < total_sensors; channel++) {
    // Set the multiplexer channel
      Serial.print("IR Sensor ");
    digitalWrite(s0Pin, bitRead(channel, 0));
    digitalWrite(s1Pin, bitRead(channel, 1));
    digitalWrite(s2Pin, bitRead(channel, 2));
    digitalWrite(s3Pin, bitRead(channel, 3));

    // Read the sensor value
    int sensorValue = analogRead(signalPin);
      Serial.println(sensorValue);

    if(sensorValue<200)
    {
      sectionNo = channel;
      if(last_channel == channel)
      {
        break;
      }
      last_channel = channel;
      Serial.print("IR Sensor ");
      Serial.print(channel);
      Serial.println(" is bieng touched ");
    }
    delay(100);
  }
  delay(100);
}