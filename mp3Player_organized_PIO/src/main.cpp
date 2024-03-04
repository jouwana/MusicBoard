#include <Arduino.h>

/*
Code that uses status check to play next songs passong over all songs in the SD card

we can use the serial monitor to interact with the mp3play:
Written Text ---- what it does
start             start playing from folder 01/001 and go through all songs
stop              pause currently playing song
play              resume playback, or start placyback from 03/001 if no playback wat started
next              change to the next song
prev              change to previous song
combine           play a combination list of songs: 01/001 02/005 01/003
*/
// Include required libraries:

#include "RFID_reader.hpp"
#include "mp3_commands.hpp"
#include "light_control.hpp"

#ifndef HardwareSerial_h
#include <HardwareSerial.h>
#endif

// Definitions Pin and setups

// Define the Serial MP3 Player Module.
HardwareSerial MP3(2); // Use UART2 for MP3 player communication
// GPIO17 ------------------------ MP3 TX2
// GPIO16 ------------------------ MP3 RX2
#define RX 16
#define TX 17

#define START_PIN 21
//we will use pins 12,14,27,26,25,32,33 for the buttons

MP3Commands MP3_controller = MP3Commands(RX, TX);

bool authorized_card = false;
unsigned long previousMillis = 0;
int[] lastStates = [1, 1, 1, 1, 1, 1, 1];
int[] currentStates = [1, 1, 1, 1, 1, 1, 1];

void setup()
{
  // Initiate the serial monitor.
  Serial.begin(9600);
  Serial.setTimeout(10);

  pinMode(START_PIN, INPUT_PULLUP);

  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin();

  if (MP3_controller.reset_mp3() == true)
  {
    Serial.println("reset MP3 success");
  }
  else
  {
    Serial.println("reset MP3 fail");
    delay(10000);
  }

  MP3_controller.select_SD_card(); // ALWAYS select SD card at beginning
  delay(1200);                     // indexing the files on your SD card will take at least 1 second. let the operation finish in the background before trying to play a file.
  MP3_controller.set_volume(8);
}

void loop()
{
  lastStates[1] = currentStates[1];
  currentStates[1] = digitalRead(START_PIN);

  if (lightsOn)
  {
    turnOffPixels();
  }
  if (authorized_card)
  {
    MP3_controller.play_controls();
  }

  // Look for new cards
  CARDTYPE cardType = handle_reads();

  if (cardType == AUTHORIZED)
  {
    turnOnPixels(1);
    

    for (int i = 0; i < 2; i++)
    {
      if (!(nuidPICC[0] != scannedCards[i].hexID[0] ||
            nuidPICC[1] != scannedCards[i].hexID[1] ||
            nuidPICC[2] != scannedCards[i].hexID[2] ||
            nuidPICC[3] != scannedCards[i].hexID[3]))
      {
        MP3_controller.folder_number = scannedCards[i].folderId;
      }
    }

    authorized_card = true;
    MP3_controller.stop();
    MP3_controller.file_counter = 1;
  }
  else if (cardType == PREVIOUS)
  {
    turnOnPixels(0);
  }
}
