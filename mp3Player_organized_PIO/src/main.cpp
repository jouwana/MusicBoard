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

#include "BluetoothSerial.h"

#ifndef HardwareSerial_h
  #define HardwareSerial_h
  #include <HardwareSerial.h>
#endif


// Definitions Pin and setups

// Define the Serial MP3 Player Module.
HardwareSerial MP3(2); // Use UART2 for MP3 player communication
// GPIO17 ------------------------ MP3 TX2
// GPIO16 ------------------------ MP3 RX2
#define RX 16
#define TX 17
#define NUM_BUTTONS 6
#define START_PIN_INDEX 0
#define NEXT_PIN_INDEX 1
#define PREV_PIN_INDEX 2
#define VOL_UP_PIN_INDEX 3
#define VOL_DOWN_PIN_INDEX 4
#define STOP_PIN_INDEX 5

bool firstClick = true;

int pinNumbers[NUM_BUTTONS] = {12, 14, 27, 26, 25, 33};
int arrlastStates[NUM_BUTTONS] = {1, 1, 1, 1, 1, 1};
int arrcurrentStates[NUM_BUTTONS] = {1, 1, 1, 1, 1, 1};
String arrButtonCommands[NUM_BUTTONS] = {"start", "next", "prev", "vol_up", "vol_down", "stop"};
// we will use pins 12,14,27,26,25,32,33 for the buttons

MP3Commands MP3_controller = MP3Commands(RX, TX);
FSControl FSC;

int getCurrentFolder()
{
  return FSC.getCurrentFolder();
}

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

bool SD_card_selected = false;

bool authorized_card = false;
unsigned long previousMillis = 0;
String mp3_serial_command = "";
SCANTYPE scanning_mode = STOP_SCAN;

String device_name = "ESP32";
BluetoothSerial SerialBT;

void setup()
{
  // Initiate the serial monitor.
  Serial.begin(9600);
  Serial.setTimeout(10);
  delay (100); // for stability
  for (int i = 0; i < NUM_BUTTONS; i++)
  {
    pinMode(pinNumbers[i], INPUT_PULLUP);
  }

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
    SD_card_selected = false;
    delay(10000);
  }

  MP3_controller.select_SD_card(); // ALWAYS select SD card at beginning
  delay(1200);                     // indexing the files on your SD card will take at least 1 second. let the operation finish in the background before trying to play a file.
  MP3_controller.set_volume(8);

  SerialBT.begin(device_name); // Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  delay(100); //for stability

}

void loop()
{

  // if(SD_card_selected)
  // {
  //   movingPixels(0, 0, 255);
  //   SD_card_selected = MP3_controller.reset_mp3();
  //   delay(100); // stability
  // }

    if (SerialBT.available())
    {

      String str = SerialBT.readString();
      if (scanning_mode == STOP_SCAN && str == "scan")
      {
        MP3_controller.stop();
        Serial.println("Please scan the card");
        clearNUID();
        scanning_mode = SCAN;
        turnOnPixels(255,165,0);
      }

      else if ((scanning_mode == SCAN || scanning_mode == ADD_DETAILS) && str == "exit_scan")
      {
        Serial.println("Stopping scanning mode");
        clearNUID();
        scanning_mode = STOP_SCAN;
        turnOffPixels();
      }
      else if (scanning_mode == ADD_DETAILS && str != "exit_scan" && str != "scan")
      {
        String folders = str;
        Serial.println("Details are: " + folders);
        FSC.addCardToFile(getLastCard(), folders);
        scanning_mode = STOP_SCAN;
        turnOnPixels(0,255,0);
        clearNUID();
      }
    }

    if (Serial.available() > 0)
    {
      String str = Serial.readString();
      
      if (str == "read")
      {
        FSC.readFile();
      }
      else if (str == "clear")
      {
        FSC.clearFile();
      }
      else if (scanning_mode == STOP_SCAN && str == "scan")
      {
        MP3_controller.stop();
        Serial.println("Please scan the card");
        clearNUID();
        scanning_mode = SCAN;
        turnOnPixels(255,165,0);
      }

      else if ((scanning_mode == SCAN || scanning_mode == ADD_DETAILS) && str == "exit_scan")
      {
        Serial.println("Stopping scanning mode");
        clearNUID();
        scanning_mode = STOP_SCAN;
        turnOffPixels();
      }
      else if (scanning_mode == ADD_DETAILS && str != "exit_scan" && str != "scan")
      {
        String folders = str;
        Serial.println("Details are: " + folders);
        FSC.addCardToFile(getLastCard(), folders);
        scanning_mode = STOP_SCAN;
        turnOnPixels(0,255,0);
        clearNUID();
      }
    }


    if (authorized_card)
    {
      for (int i = 0; i < NUM_BUTTONS; i++)
      {
        arrlastStates[i] = arrcurrentStates[i];
        arrcurrentStates[i] = digitalRead(pinNumbers[i]);
        if (arrlastStates[i] == 0 && arrcurrentStates[i] == 1)
        {
          Serial.println("Button " + arrButtonCommands[i] + " pressed");
          if (i == START_PIN_INDEX && !firstClick)
          {
            MP3_controller.play_controls(arrButtonCommands[i]);
            if (arrButtonCommands[i] == "play")
              arrButtonCommands[i] = "pause";
            else if (arrButtonCommands[i] == "pause")
              arrButtonCommands[i] = "play";
          }
          else if (i == START_PIN_INDEX && firstClick)
          {
            MP3_controller.play_controls("start");
            arrButtonCommands[i] = "pause";
            firstClick = false;
          }
          else
          {
            MP3_controller.play_controls(arrButtonCommands[i]);
          }
          if (i == STOP_PIN_INDEX)
          {
            firstClick = true;
            FSC.resetFolderIndex();
          }
        }
      }
      MP3_controller.play_controls("");
    }

    if (lightsOn)
    {
      turnOffPixels(scanning_mode == STOP_SCAN ? false : true);
    }
    // Look for new cards
    CARDTYPE cardType = NO_CARD;

    if (scanning_mode != ADD_DETAILS)
    {
      cardType = handle_reads();
    }


    if (scanning_mode == SCAN)
    {
      if (cardType == AUTHORIZED || cardType == PREVIOUS)
      {
        SerialBT.println("Card Already Exists in System");
        Serial.println("card already exists in system");
        Serial.println("Please scan another card OR exit scan mode");
        turnOnPixels(255,150,203); //bluey green?
      }
      else if (cardType == NEW)
      {
        turnOnPixels(0,255,0);
        SerialBT.println("new");
        Serial.println("Please write in details for the card");
        scanning_mode = ADD_DETAILS;

      }
    }

    else if (scanning_mode == STOP_SCAN)
    {
      if (cardType == AUTHORIZED)
      {
        turnOnPixels(0,255,0);
        FSC.setCurrentCard(getLastCard());
        MP3_controller.folder_number = getCurrentFolder();

        authorized_card = true;
        MP3_controller.stop();
        MP3_controller.file_counter = 1;
      }
      else if (cardType == PREVIOUS)
      {
        turnOnPixels(255,95,95);
      }
      else if (cardType == NEW)
      {
        turnOnPixels(255,0,0);
        Serial.println(F("this card is not in file, please register it."));
        clearNUID();
      }
    }
}
