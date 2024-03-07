#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// Which pin on the Arduino is connected to the NeoPixels?
#define PIXEL_PIN 13 // On Trinket or Gemma, suggest changing this to 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

extern unsigned long previousMillis;
const long interval = 1500;
const long shortInterval = 800;
static bool lightsOn = false;
static int blink_index = 0;

void movingPixels(int R, int G, int B)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= shortInterval)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.setPixelColor(blink_index, pixels.Color(R, G, B));
    pixels.show();
    blink_index = (blink_index == (NUMPIXELS - 1)) ? 0 : blink_index + 1;
  }
}

void clearPixels()
{
  pixels.clear();
  pixels.show();
}

void movingPixelsRandom()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= shortInterval)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
          srand(time(NULL)+i);
      int R = rand() % 256; 
      int G = rand() % 256; 
      int B = rand() % 256; 
      pixels.setPixelColor(i, pixels.Color(R, G, B));
    }

    pixels.setPixelColor(blink_index, pixels.Color(0, 0, 0));
    pixels.show();
    blink_index = (blink_index == (NUMPIXELS - 1)) ? 0 : blink_index + 1;
  }
}

void turnOnPixels(int R, int G, int B)
{
  for (int i = 0; i < NUMPIXELS; i++)
  { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(R, G, B));
   
  }
  lightsOn = true;

  pixels.show();
  previousMillis = millis(); // Reset the timer
}

void turnOffPixels(bool scanMode = false)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    // It's been 3 seconds, turn off the lights
    pixels.clear();
    pixels.show();
    lightsOn = false;
    if (scanMode)
    {
      turnOnPixels(255, 165, 0);
    }
  }
}