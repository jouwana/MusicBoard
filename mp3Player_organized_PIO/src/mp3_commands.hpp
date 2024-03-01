#ifndef MP3_COMMANDS_H
#define MP3_COMMANDS_H
#endif

#include <Arduino.h>
// Include necessary libraries here
#ifndef HardwareSerial_h
#include <HardwareSerial.h>
#endif

extern HardwareSerial MP3;

class MP3Commands
{
private:
    // Define the required MP3 Player Commands: based on command codes listed in manual - https://static1.squarespace.com/static/584d41b3f5e2310b396cd953/t/5c7c2f29104c7b336a2f8380/1551642412037/Serial+MP3+Player+A+v1.0+Manual.pdf
    byte start_byte = 0x7E; // Start byte
    byte end_byte = 0xEF;   // End byte

    byte resume_CMD = 0x01;
    byte pause_CMD = 0x02;
    byte stop_CMD = 0x0E;
    byte next_song_CMD = 0x03;
    byte prev_song_CMD = 0x04;
    byte volume_up_CMD = 0x05;
    byte volume_down_CMD = 0x06;

    byte set_volume_CMD = 0x31;
    byte play_index_CMD = 0x41;
    byte play_filename_CMD = 0x42;
    uint8_t select_SD_CMD[5] = {0x7e, 0x03, 0X35, 0x01, 0xef};
    uint8_t reset_CMD[5] = {0x7e, 0x03, 0X35, 0x05, 0xef};

    uint8_t Send_buf[6] = {0};
    char receivedData[20]; // buffer for storing incoming data
    int dataIndex = 0;     // index for storing incoming data

public: // Define the Serial MP3 Player Module.
    MP3Commands(uint8_t RX, uint8_t TX)
    {
        // Initiate the Serial MP3 Player Module.
        MP3.begin(9600, SERIAL_8N1, TX, RX);
        delay(100); // delay for stability
    };

    // Use UART2 for MP3 player communication
    // GPIO17 ------------------------ MP3 TX2
    // GPIO16 ------------------------ MP3 RX2

    enum MP3_status
    {
        STOPPED,
        IDLE,
        PLAYING,
        NODATA
    };

    bool playing = false;
    String last_cmd = "";
    uint8_t folder_number = 1;
    uint8_t file_counter = 1;

    void stop();
    void play_controls();
    void mp3Basic(uint8_t command);
    MP3_status check_MP3_status();
    bool reset_mp3();
    void select_SD_card();
    void set_volume(byte volume);
    void play_filename(uint8_t directory, uint8_t file);
    void play_Index(uint8_t file);
    void preset_Combine();
    void sendBytes(uint8_t nbytes);
    void nextFile();
    void nextFolder();
    void prevFile();
    void prevFolder();
};