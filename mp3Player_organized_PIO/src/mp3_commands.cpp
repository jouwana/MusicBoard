#include <Arduino.h>
#include "mp3_commands.hpp"

void MP3Commands::stop()
{
    playing = false;
    mp3Basic(stop_CMD);
}

void MP3Commands::play_controls(String str)
{
    if (str == "play")
    {
        playing = true;
        mp3Basic(resume_CMD);
    }

    else if (str == "stop"){
        playing = false;
        stop();
    }

    else if (str == "pause")
    {
        playing = false;
        mp3Basic(pause_CMD);
    }

    else if (str == "next" && playing)
    {
        nextFile();
        play_filename(folder_number, file_counter);
    }

    else if (str == "prev" && playing)
    {
        prevFile();
        play_filename(folder_number, file_counter);
    }

    else if (str == "vol_up")
    {
        mp3Basic(volume_up_CMD);
        mp3Basic(volume_up_CMD);
    }
    else if (str == "vol_down")
    {
        mp3Basic(volume_down_CMD);
        mp3Basic(volume_down_CMD);
    }

    // we play here if we use 'start' to begin playing, or if previous song ended and we are in playing mode
    if ((check_MP3_status() == STOPPED && playing) || str == "start")
    {
        if(str == "start"){
            folder_number = getCurrentFolder();
            file_counter = 1;
        }
        else{
            nextFile();
        }
        play_filename(folder_number, file_counter);

        // mp3Basic(next_song_CMD); //cycles through ALL the songs on the file;
        playing = true;
    }
    delay(25);
}

// this sends basic commands that use only 4 bytes to the mp3 player
void MP3Commands::mp3Basic(uint8_t command)
{
    Send_buf[0] = 0x7e; // starting byte
    Send_buf[1] = 0x02; // the number of bytes of the command without starting byte and ending byte
    Send_buf[2] = command;
    Send_buf[3] = 0xef; //
    sendBytes(4);
}

MP3Commands::MP3_status MP3Commands::check_MP3_status()
{
    if (MP3.available())
    { // should not be a while loop, but a if loop. but MP3 sends very little data, so it should be fine
        // get the new byte:
        char player_status = MP3.read();
        receivedData[dataIndex] = player_status;
        Serial.print(player_status, HEX);
        delay(1); // At 9600 baud , each char takes about 0.9ms
        if (receivedData[dataIndex] == 0xEF)
        { // reached end of reply
            Serial.print(" LEN=");
            Serial.println(dataIndex);
            if (receivedData[dataIndex - 3] == 0x3D && receivedData[dataIndex - 4] == 0x04)
            { // 7E.4.3D.(folder_byte).(file_byte).EF  - stopped msg - sent twice by MP3
                Serial.print(" STOP MSG #");
                if (dataIndex > 5)
                { // if the message length is 5, then this is the first time "stop" is sent. ignore it and wait for the second time.
                    dataIndex = 0;
                    Serial.println(" 2");
                    return (PLAYING); // we ignore it so send playing
                }
                else
                {
                    Serial.println(" 1");
                    return (STOPPED);
                }
            }
            else if (receivedData[dataIndex - 1] == 0x00 && receivedData[dataIndex - 2] == 0x31)
            { // 7E20EF7E3101EF - status reply - currently playing
                Serial.println(" STATUS REPLY - IDLE");
                dataIndex = 0;
                return (IDLE);
            }
            else if (receivedData[dataIndex - 1] == 0x01 && receivedData[dataIndex - 2] == 0x31)
            { // 7E20EF7E3101EF - status reply - currently playing
                Serial.println(" STATUS REPLY - PLAYING");
                dataIndex = 0;
                return (PLAYING);
            }
            dataIndex = 0;
        }
        else
        {
            dataIndex++;
            Serial.print(".");
        }
    }

    return (NODATA);
}

bool MP3Commands::reset_mp3()
{
    Serial.println("MP3 RESET");
    MP3.flush(); // delete all data in serial2 buffer for MP3
    int len = 5;
    for (int i = 0; i < len; i++)
    {
        MP3.write(reset_CMD[i]);
    }
    delay(50); // give the mp3 player time to reset and return message
    if (MP3.available())
    { // check if there is a response from the board (not reading the response itself)
        return true;
    }
    else
    {
        return false;
    }
}

void MP3Commands::select_SD_card()
{
    Serial.println("MP3_select_SD_card");
    int len = 4;
    for (int i = 0; i <= len; i++)
    {
        MP3.write(select_SD_CMD[i]);
    }
}

void MP3Commands::set_volume(byte volume)
{
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
    delay(20); // required for stability
}

void MP3Commands::play_filename(uint8_t directory, uint8_t file)
{
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
    delay(20); // required for stability
}

void MP3Commands::play_Index(uint8_t file)
{
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
    delay(20); // required for stability
}

void MP3Commands::sendBytes(uint8_t nbytes)
{
    for (uint8_t i = 0; i < nbytes; i++) //
    {
        MP3.write(Send_buf[i]);
    }
}

void MP3Commands::nextFile()
{
    file_counter++;
    if (file_counter > 4)
    {
        nextFolder();
        file_counter = 1;
    }
}

void MP3Commands::nextFolder()
{
    folder_number = getNextFolder();
}

void MP3Commands::prevFile()
{
    file_counter -= 1;
    if (file_counter < 1)
    {
        prevFolder();
        file_counter = 4;
    }
}

void MP3Commands::prevFolder()
{
    folder_number = getPrevFolder();
}
