#include "set.h"
#include <Adafruit_NeoPixel.h>
#ifdef _AVR_
#include <avr/power.h>
#endif

// #include <CD74HC4067.h>

#include <Arduino.h>
#include "Mux.h"

using namespace admux;
/*
 * Creates a Mux instance.
 *
 * 1st argument is the SIG (signal) pin (Arduino digital input pin 6).
 * 2nd argument is the S0-S3 (channel control) pins (Arduino pins 2, 3, 4, 5).
 */
Mux mux1(Pin(6, INPUT, PinType::Digital), Pinset(2, 3, 4, 5));
Mux mux2(Pin(12, INPUT, PinType::Digital), Pinset(8, 9, 10, 11));



#define LEDPIN 13
#define NUMPIXELS 120
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 15

bool flow = 1;

int total_sensors_1 = 12;
int total_sensors_2 = 6;

const uint32_t aliveColor = pixels.Color(0, 0, 255);
const uint32_t deadColor = pixels.Color(0, 0, 0);
const uint32_t noColor = pixels.Color(0, 0, 0);

Set ledDead;

int sectionNo = -1;  // don't change
const int leds_in_section = NUMPIXELS / total_sensors_1 + total_sensors_2;

int resetTime = 20000;
long int t1;
long int t2;


void setup() {
#if defined(_AVR_ATtiny85_) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  t1 = millis();

  pixels.begin();
  pixels.clear();
  // Serial port initialization.
  Serial.begin(9600);
  while (!Serial) /* Waits for serial port to connect (needed for Leonardo only) */
    ;
  // sectionDead(5);
}

void (*resetFunc)(void) = 0;

void loop() {
  // to reset the machine after some time
  long int t2 = millis();
  if ((t2 - t1) >= resetTime) {
    Serial.println("reset");
    delay(30);
    resetFunc();
  }
  // flowing strip
  // livingStrip();
  constBlue();
  // // reading the channels from mux
  readMux1();
  readMux2();
}

void livingStrip() {
  if (flow == 1) {
    addFlow();
  } else {
    removeFlow();
  }
}
void addFlow() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (ledDead.has(i)) {
      continue;
    }
    pixels.setPixelColor(i, aliveColor);
    pixels.show();
    delay(DELAYVAL);
    if (i == (NUMPIXELS - 1)) {
      flow = 0;
    }
  }
}
void constBlue() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (ledDead.has(i)) {
      continue;
    }
    pixels.setPixelColor(i, aliveColor);
    pixels.show();
  }
}
void removeFlow() {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (ledDead.has(i)) {
      continue;
    }
    pixels.setPixelColor(i, noColor);
    pixels.show();
    delay(DELAYVAL);
    if (i == (NUMPIXELS - 1)) {
      flow = 1;
    }
  }
}
void sectionDead(int section) {
  Serial.println("deleting " + String(section + 1) + " Section");
  int start = section * leds_in_section;
  for (int i = start; i < (start + leds_in_section); i++) {
    if (i >= NUMPIXELS) {
      break;
    }
    ledDead.add(i);
    pixels.setPixelColor(i, deadColor);
    pixels.show();
  }
}

void readMux1() {
  byte data;
  Serial.println("Total channel count " + String(mux1.channelCount()));
  Serial.println("using channel count " + String(total_sensors_1));
  for (int8_t i = 0; i < (total_sensors_1); i++) {
    data = mux1.read(i); /* Reads from channel i (returns HIGH or LOW) */
    Serial.println(String((i + 1)) + String(data));
    // delay(200);
    if (data == HIGH) {
      continue;
    } else {
      Serial.println("channel " + String(i) + " in mux 1");
      sectionDead(i);
    }
  }
  Serial.println("----------");
  delay(400);
}
void readMux2() {
  byte data;
  Serial.println("Total channel count " + String(mux2.channelCount()));
  Serial.println("using channel count " + String(total_sensors_2));
  for (int8_t i = 0; i < (total_sensors_2); i++) {
    data = mux2.read(i) /* Reads from channel i (returns HIGH or LOW) */;
    Serial.println(String((i + 13)) + String(data));
    // delay(200);
    if (data == HIGH) {
      continue;
    } else {
      Serial.print("channel " + String(i + 12) +  " in mux 1");
      sectionDead((i+12));
    }
  }
  Serial.println("----------");
  delay(400);
}