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
#include <HardwareSerial.h>

// Define the Serial MP3 Player Module.
HardwareSerial MP3(2);  // Use UART2 for MP3 player communication
// GPIO17 ------------------------ MP3 TX2
// GPIO16 ------------------------ MP3 RX2


static int8_t Send_buf[6] = { 0 };
char receivedData[20];  //buffer for storing incoming data
int dataIndex = 0;      //index for storing incoming data
enum MP3_status { STOPPED, IDLE, PLAYING, NODATA };
int file_counter = 1;
static bool playing = false;
static String last_cmd = "";

// Define the required MP3 Player Commands: based on command codes listed in manual - https://static1.squarespace.com/static/584d41b3f5e2310b396cd953/t/5c7c2f29104c7b336a2f8380/1551642412037/Serial+MP3+Player+A+v1.0+Manual.pdf
static byte start_byte = 0x7E;  // Start byte
static byte end_byte = 0xEF;    // End byte

static byte resume_CMD = 0x01;
static byte pause_CMD = 0x02;
static byte stop_CMD = 0x0E;
static byte next_song_CMD = 0x03;
static byte prev_song_CMD = 0x04;
static byte volume_up_CMD = 0x05;
static byte volume_down_CMD = 0x06;

static byte set_volume_CMD = 0x31;
static byte play_index_CMD = 0x41;
static byte play_filename_CMD = 0x42;
static int8_t select_SD_CMD[] = { 0x7e, 0x03, 0X35, 0x01, 0xef };
static int8_t reset_CMD[] = { 0x7e, 0x03, 0X35, 0x05, 0xef };

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include <SPI.h>
#include <MFRC522.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        26 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 3 // Popular NeoPixel ring size

#define SS_PIN 5
#define RST_PIN 22
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
bool lightsOn = false;
bool authorized_card = false;
unsigned long previousMillis = 0;
const long interval = 1500;  // 1.5 seconds
int folder_number = 0;

// Init array that will store new NUID 
byte nuidPICC[4];

// struct to hold RFID card ID 
struct RFIDCard {
    byte hexID[4]; 
    int folderId; 
};

// Init array to store scanned RFID card IDs 

RFIDCard scannedCards[2]={
    {{0x59, 0xEA, 0xD4, 0xA3}, 1},{{0xC9, 0xA6, 0xC1, 0xA2}, 2}};

void setup() {
  // Initiate the serial monitor.
  Serial.begin(9600);
  Serial.setTimeout(10);

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin();

  // Initiate the Serial MP3 Player Module.
  MP3.begin(9600, SERIAL_8N1, 17, 16);
  delay(100);  //delay for stability

  if (reset_mp3() == true) {
    Serial.println("reset MP3 success");
  } else {
    Serial.println("reset MP3 fail");
    delay(10000);
  }

  select_SD_card();  //ALWAYS select SD card at beginning
  delay(1200);       //indexing the files on your SD card will take at least 1 second. let the operation finish in the background before trying to play a file.
  set_volume(8);
}

void loop() {
  if(lightsOn){
    turnOffPixels();
  }
  if(authorized_card){
    play_controls();
  }

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
  {

    return;
  }

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    turnOnPixels(1);

    for(int i=0; i<2 ;i++){
      if (!(rfid.uid.uidByte[0] != scannedCards[i].hexID[0] || 
        rfid.uid.uidByte[1] != scannedCards[i].hexID[1] || 
        rfid.uid.uidByte[2] != scannedCards[i].hexID[2] || 
        rfid.uid.uidByte[3] != scannedCards[i].hexID[3] ))
         {
          folder_number = scannedCards[i].folderId;
         }
    }

    authorized_card = true;
    mp3Basic(stop_CMD);
    file_counter = 1;
  }
  else {
    Serial.println(F("Card read previously."));
    turnOnPixels(0);
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

}

void play_controls(){
  //if there is text in the serial monitor buffer, then read it.
  String str = "";
  if (Serial.available() > 0) {
      str = Serial.readString();
      str.trim();
      Serial.println(str);
      last_cmd = str;
    }
  

  if(str == "play")
  {
    playing = true;
    mp3Basic(resume_CMD);
  }

  else if(str == "stop"){
    playing = false;
    mp3Basic(pause_CMD);
  }

  else if (str == "next"){
    play_filename(folder_number,file_counter);
    nextFile();
  }

  else if (str == "prev"){
    prevFile();
    play_filename(folder_number,file_counter); 
    nextFile();
 }

  else if (str == "combine"){
    preset_Combine();
  }

  else if(str == "vol up"){
    mp3Basic(volume_up_CMD);
    mp3Basic(volume_up_CMD);
  }
    else if(str == "vol down"){
    mp3Basic(volume_down_CMD);
    mp3Basic(volume_down_CMD);
  }

  //we play here if we use 'start' to begin playing, or if previous song ended and we are in playing mode
  if ((check_MP3_status() == STOPPED && playing)  || str == "start") {
    play_filename(folder_number,file_counter); 
    // uncomment this code if you want to cycle play a preset number of consecutive files
    nextFile();

    //mp3Basic(next_song_CMD); //cycles through ALL the songs on the file;
    playing = true;
  }
  delay(25);
}

//this sends basic commands that use only 4 bytes to the mp3 player
void mp3Basic(int8_t command) {
  Send_buf[0] = 0x7e;  //starting byte
  Send_buf[1] = 0x02;  //the number of bytes of the command without starting byte and ending byte
  Send_buf[2] = command;
  Send_buf[3] = 0xef;  //
  sendBytes(4);
}


MP3_status check_MP3_status() {
  if (MP3.available()) {  //should not be a while loop, but a if loop. but MP3 sends very little data, so it should be fine
    // get the new byte:
    char player_status = MP3.read();
    receivedData[dataIndex] = player_status;
    Serial.print(player_status, HEX);
    delay(1);                               //At 9600 baud , each char takes about 0.9ms
    if (receivedData[dataIndex] == 0xEF) {  //reached end of reply
      Serial.print(" LEN=");
      Serial.println(dataIndex);
      if (receivedData[dataIndex - 3] == 0x3D && receivedData[dataIndex - 4] == 0x04) {  //7E.4.3D.(folder_byte).(file_byte).EF  - stopped msg - sent twice by MP3
        Serial.print(" STOP MSG #");
        if (dataIndex > 5) {  //if the message length is 5, then this is the first time "stop" is sent. ignore it and wait for the second time.
          dataIndex = 0;
          Serial.println(" 2");
          return (PLAYING); //we ignore it so send playing
        } else {
          Serial.println(" 1");
          return (STOPPED);
        }

      } else if (receivedData[dataIndex - 1] == 0x00 && receivedData[dataIndex - 2] == 0x31) {  //7E20EF7E3101EF - status reply - currently playing
        Serial.println(" STATUS REPLY - IDLE");
        dataIndex = 0;
        return (IDLE);
      } else if (receivedData[dataIndex - 1] == 0x01 && receivedData[dataIndex - 2] == 0x31) {  //7E20EF7E3101EF - status reply - currently playing
        Serial.println(" STATUS REPLY - PLAYING");
        dataIndex = 0;
        return (PLAYING);
      }
      dataIndex = 0;
    } else {
      dataIndex++;
      Serial.print(".");
    }
  }

  return (NODATA);
}


bool reset_mp3() {
  Serial.println("MP3 RESET");
  MP3.flush();  //delete all data in serial2 buffer for MP3
  int len = 5;
  for (int i = 0; i < len; i++) {
    MP3.write(reset_CMD[i]);
  }
  delay(50);              //give the mp3 player time to reset and return message
  if (MP3.available()) {  //check if there is a response from the board (not reading the response itself)
    return true;
  } else {
    return false;
  }
}

void select_SD_card() {
  Serial.println("MP3_select_SD_card");
  int len = 4;
  for (int i = 0; i <= len; i++) {
    MP3.write(select_SD_CMD[i]);
  }
}

void set_volume(byte volume) {
  delay(20);
  Serial.print("set volume= ");
  Serial.print(volume);
  Serial.println(" of 30");
  MP3.write(start_byte);
  byte msg_len = 0x03;
  MP3.write(msg_len);
  MP3.write(set_volume_CMD);
  MP3.write(volume);
  MP3.write(end_byte);
  delay(20);  //required for stability
}

void play_filename(int8_t directory, int8_t file) {
  Serial.print("playing directory ");
  Serial.print(directory);
  Serial.print(" file number ");
  Serial.println(file);
  MP3.write(start_byte);
  byte msg_len = 0x04;
  MP3.write(msg_len);
  MP3.write(play_filename_CMD);
  byte dir_ind = directory;
  MP3.write(dir_ind);
  byte file_ind = file;
  MP3.write(file_ind);
  MP3.write(end_byte);
  delay(20);  //required for stability
}

void play_Index(int8_t file) {
  Serial.print(" playing file index: ");
  Serial.println(file);
  MP3.write(start_byte);
  byte msg_len = 0x04;
  MP3.write(msg_len);
  MP3.write(play_index_CMD);
  int8_t directory = 0;
  byte dir_ind = directory;
  MP3.write(dir_ind);
  byte file_ind = file;
  MP3.write(file_ind);
  MP3.write(end_byte);
  delay(20);  //required for stability
}

void preset_Combine(){
  Serial.print(" playing combination songs: 01/005, 02/001, 01/003 ");

  MP3.write(start_byte);
  byte msg_len = 0x08;
  MP3.write(msg_len);

  byte combine_cmd = 0x45;
  MP3.write(combine_cmd);

  byte dir_ind = 1;
  MP3.write(dir_ind);
  byte file_ind = 5;
  MP3.write(file_ind);
  dir_ind = 2;
  MP3.write(dir_ind);
  file_ind = 1;
  MP3.write(file_ind);
  dir_ind = 1;
  MP3.write(dir_ind);
  file_ind = 3;
  MP3.write(file_ind);

  MP3.write(end_byte);

  delay(20);  //required for stability
}


void sendBytes(uint8_t nbytes) {
  for (uint8_t i = 0; i < nbytes; i++)  //
  {
    MP3.write(Send_buf[i]);
  }
}

void turnOnPixels(int isNew){
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
      if(isNew){
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      }
      else {
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      }
    }
    if(isNew)
        Serial.println(F("Turn on green lights"));
    else 
        Serial.println(F("Turn on red lights"));
    pixels.show();  
    lightsOn = true;
    previousMillis = millis();  // Reset the timer
} 

void turnOffPixels(){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      // It's been 3 seconds, turn off the lights
      pixels.clear();
      pixels.show();
      lightsOn = false;
       Serial.println(F("Turn off lights"));
    }
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}

void nextFile(){
  file_counter ++;
  if (file_counter>5){
    nextFolder();
    file_counter = 1;
  }
}

void nextFolder(){
  folder_number ++;
  if (folder_number>4){
    folder_number = 1;
  }
}



void prevFile(){
  file_counter -=2;
  if (file_counter<1){
    prevFolder();
    file_counter = 5;
  }
}

void prevFolder(){
  folder_number --;
  if (folder_number<1){
    folder_number = 4;
  }
}

