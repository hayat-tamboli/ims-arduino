#include "set.h"
#include <Adafruit_NeoPixel.h>
#ifdef _AVR_
#include <avr/power.h>
#endif

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

// other LED pins
#define LED17 8
#define LED18 9
#define LED19 10
#define LED20 11
#define LED21 12


#define LEDPIN 13
#define NUMPIXELS 396
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 1

bool flow = 1;

int total_sensors_1 = 16;
int total_sensors_2 = 5;

const uint32_t aliveColor = pixels.Color(0, 0, 255);
const uint32_t deadColor = pixels.Color(0, 0, 0);
const uint32_t noColor = pixels.Color(0, 0, 0);

Set ledDead;
Set dead_section_set;

int leds_sectional_flow_count = 100;

int sectionNo = -1;                                              // don't change
const int total_sections = (total_sensors_1 + total_sensors_2);  // 14+14 = 28
const int leds_in_section = 18;
const int reset_section_number = 10;
int dead_section_count = 0;

void setup() {
#if defined(_AVR_ATtiny85_) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
  pixels.clear();
  // Serial port initialization.
  Serial.begin(9600);
}

void (*resetFunc)(void) = 0;

// the main loop !!!!!!!!
void loop() {
  constColor();
  readMux1();
  readIRSensor(LED17,16);
  readIRSensor(LED18,17);
  readIRSensor(LED19,18);
  readIRSensor(LED20,19);
  readIRSensor(LED21,20);
}

void constColor() {
  // for (int i = 0; i < NUMPIXELS; i++) {
  for (int i = (NUMPIXELS - 1); i >= 0; i--) {
    if (ledDead.has(i)) {
      continue;
    }
    pixels.setPixelColor(i, aliveColor);
  }
  pixels.show();
}


void sectionDead(int section) {
  Serial.println("deleting " + String(section + 1) + " Section");
  int start = (section)*leds_in_section;
  Serial.println(String(start));
  Serial.println(String(start + leds_in_section));
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
      sectionDead(i);
    }
  }
  Serial.println("----------");
}
void readIRSensor(int pinNum, int section_to_die)
{
  int inputState = digitalRead(pinNum); // Read the state of the input

  if (inputState == LOW) {
    Serial.println(String(pinNum)+ " got some detection");
    sectionDead(section_to_die);
  }
}