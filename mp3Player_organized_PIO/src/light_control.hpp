#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 3 // Popular NeoPixel ring size

// Which pin on the Arduino is connected to the NeoPixels?
#define PIXEL_PIN 13 // On Trinket or Gemma, suggest changing this to 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

extern unsigned long previousMillis;
const long interval = 1500;
static bool lightsOn = false;

void turnOnPixels(int color)
{
  for (int i = 0; i < NUMPIXELS; i++)
  { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    if (color == 1)
    {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    }
    else if (color == 0)
    {
      pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    }
    else
    { //supposeddly orange
      pixels.setPixelColor(i, pixels.Color(255, 165, 0));
    }
  }
  lightsOn = true;
  switch(color){
    case 0:{
      Serial.println(F("Turn on red lights"));
      break;
    }
    case 1:{
      Serial.println(F("Turn on green lights"));
      break;
    }
    default:{
      Serial.println(F("Turn on orange lights"));
      lightsOn = false;
      break;
    }
  }

  pixels.show();
  previousMillis = millis(); // Reset the timer
}

void turnOffPixels()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    // It's been 3 seconds, turn off the lights
    pixels.clear();
    pixels.show();
    lightsOn = false;
    Serial.println(F("Turn off lights"));
  }
}