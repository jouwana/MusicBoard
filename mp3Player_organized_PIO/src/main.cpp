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

#include <BluetoothSerial.h>

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
FSControl FSC;

int getNextFolder()
{
  return FSC.getNextFolder();
}

int getPrevFolder()
{
  return FSC.getPrevFolder();
}

enum SCANTYPE
{
  SCAN,
  STOP_SCAN,
  ADD_DETAILS
};

bool authorized_card = false;
unsigned long previousMillis = 0;
int lastStates = 1;
int currentStates = 1;
String mp3_serial_command = "";
SCANTYPE scanning_mode = STOP_SCAN;

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
  FSC = FSControl();

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
  lastStates = currentStates;
  currentStates = digitalRead(START_PIN);

  if(Serial.available() > 0){
    String str = Serial.readString();
    if(scanning_mode == STOP_SCAN && str == "scan"){
      MP3_controller.stop();
      Serial.println("Please scan the card");
      clearNUID();
      scanning_mode = SCAN;
      turnOnPixels(2);
    }

    else if ((scanning_mode == SCAN || scanning_mode == ADD_DETAILS) && str == "stop scan")
    {
      Serial.println("Stopping scanning mode");
      clearNUID();
      scanning_mode = STOP_SCAN;
      turnOffPixels();
    }

    else if(str == "read"){
      FSC.readFile();
    }
    else if(str == "clear"){
      FSC.clearFile();
    }
    else if(scanning_mode == ADD_DETAILS && str != "stop scan"){
      String folders = str;
      Serial.println("Details are: " + folders);
      FSC.addCardToFile(getLastCard(), folders);
      scanning_mode = STOP_SCAN;
      turnOnPixels(1);
      clearNUID();
    }
    else if (authorized_card)
    {
      mp3_serial_command = str;
    }
  }

  if(authorized_card){
    MP3_controller.play_controls(mp3_serial_command);
    mp3_serial_command = "";
  }

  if (lightsOn)
  {
    turnOffPixels();
  }
  // Look for new cards
  CARDTYPE cardType = NO_CARD;

  if(scanning_mode != ADD_DETAILS){
    cardType = handle_reads();
  }
  
  if(scanning_mode == SCAN){
    if(cardType == AUTHORIZED || cardType == PREVIOUS){
      Serial.println("card alreadt exists in system");
      Serial.println("Please scan another card OR exit scan mode");
    }
    else if (cardType == NEW){
      bool infile = FSC.checkCardInFile(getLastCard());
      if(!infile){
        Serial.println("Card not in file");
        Serial.println("Please write in details for the card");
        scanning_mode = ADD_DETAILS;
      }
    }
  }

  else if (scanning_mode == STOP_SCAN){
    if ( cardType == AUTHORIZED)
    {
      turnOnPixels(1);
      FSC.setCurrentCard(getLastCard());
      MP3_controller.folder_number = FSC.getNextFolder();

      authorized_card = true;
      MP3_controller.stop();
      MP3_controller.file_counter = 1;
    }
    else if (cardType == PREVIOUS)
    {
      turnOnPixels(0);
    }
    else if (cardType == NEW)
    {
      Serial.println(F("this card is not in file, please register it."));
      clearNUID();
    }
  }
}
