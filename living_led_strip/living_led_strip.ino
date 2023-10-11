#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define NUMPIXELS 6
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 100

bool flow = 1;

void setup()
{
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  	clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
  pixels.clear();
}

void loop()
{
  if(flow == 1)
  {
    for(int i=0; i<NUMPIXELS; i++) {

      pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      pixels.show();
      delay(DELAYVAL);
      if(i==(NUMPIXELS-1))
      {
        flow = 0;
      }
    }
  }
  else
  {
    for(int i=0; i<NUMPIXELS; i++) {

      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      delay(DELAYVAL);
      if(i==(NUMPIXELS-1))
      {
        flow = 1;
      }
    }
  }
}