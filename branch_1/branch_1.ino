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
#define NUMPIXELS 396
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 1

bool flow = 1;

int total_sensors_1 = 14;
int total_sensors_2 = 14;

const uint32_t aliveColor = pixels.Color(0, 0, 255);
const uint32_t deadColor = pixels.Color(0, 0, 0);
const uint32_t noColor = pixels.Color(0, 0, 0);

Set ledDead;
Set dead_section_set;

int leds_sectional_flow_count = 100;

int sectionNo = -1;  // don't change
const int total_sections = (total_sensors_1 + total_sensors_2); // 14+14 = 28
const int leds_in_section = 17; 
const int reset_section_number = 10;
int dead_section_count = 0;


// variables to reset stuff
// int resetTime = 20000;
// long int t1;
// long int t2;

void setup() {
#if defined(_AVR_ATtiny85_) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  // t1 = millis();

  pixels.begin();
  pixels.clear();
  // Serial port initialization.
  Serial.begin(9600);
  // while (!Serial);
}

void (*resetFunc)(void) = 0;

void loop() {
  // to reset the machine after some time
  // long int t2 = millis();
  // if ((t2 - t1) >= resetTime) {
  //   Serial.println("reset");
  //   delay(30);
  //   resetFunc();
  // }
  // if(total_sections)
  constColor();
  // livingStrip();
  // reading the channels from mux 1 and 2
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
  // for (int i = 0; i < NUMPIXELS; i++) {
  for (int i = (NUMPIXELS-1); i >=0; i--) {
    if (ledDead.has(i)) {
      continue;
    }
    pixels.setPixelColor(i, aliveColor);
    pixels.show();
    // delay(DELAYVAL);
    if (i == (NUMPIXELS - 1)) {
      flow = 0;
    }
  }
}
void constColor() {
  // for (int i = 0; i < NUMPIXELS; i++) {
    for (int i = (NUMPIXELS-1); i >=0; i--) {
    if (ledDead.has(i)) {
      continue;
    }
    pixels.setPixelColor(i, aliveColor);
  }
  pixels.show();
}

void removeFlow() {
  // for (int i = 0; i < NUMPIXELS; i++) {
    for (int i = (NUMPIXELS-1); i >=0; i--) {
    if (ledDead.has(i)) {
      continue;
    }
    pixels.setPixelColor(i, noColor);
    pixels.show();
    if (i == (NUMPIXELS - 1)) {
      flow = 1;
    }
  }
}
void sectionDead(int section) {
  Serial.println("deleting " + String(section + 1) + " Section");
  int start = (section) * leds_in_section;
  Serial.println(String(start));
  Serial.println(String(start+leds_in_section));
  // delay(2000);
  for (int i = start; i < (start + leds_in_section); i++) {
    if (i >= NUMPIXELS) {
      break;
    }
    ledDead.add(i);
    pixels.setPixelColor(i, deadColor);
  }
    pixels.show();
}

void readMux1() {
  byte data;
  for (int8_t i = 0; i < (total_sensors_1); i++) {
    data = mux1.read(i); /* Reads from channel i (returns HIGH or LOW) */
    Serial.println("a" + String(i) + " " + String(data));
    if (data == HIGH) {
      continue;
    } else {
      // Serial.println("channel " + String(i) + " in mux 1");
      sectionDead(i);
    }
  }
  // Serial.println("----------");
}
void readMux2() {
  byte data;
  for (int8_t i = 0; i < (total_sensors_2); i++) {
    data = mux2.read(i) /* Reads from channel i (returns HIGH or LOW) */;
    Serial.println("b" + String(i) + " " + String(data));
    if (data == HIGH) {
      continue;
    } else {
      sectionDead((i+14));
    }
  }
  Serial.println("----------");
}