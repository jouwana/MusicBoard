#pragma once
#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>
#include <vector>


/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true


class FSControl
{
    public:
        FSControl();

        void clearFile();
        void readFile();
        void creatFileIfNotExist();
        bool checkCardInFile(String cardNumber);
        bool addCardToFile(String cardNumber, String folders);
        bool setCurrentCard(String cardNumber);
        void resetFolderIndex();
        int getCurrentFolder();
        int getNextFolder();
        int getPrevFolder();
        void setCurrentFolderSize(int size);
        int getCurrentFolderSize();
        int getNumberOfMappedFolders();
    private:
        void writeToFile(String data);
        void appedToFile(String data);

        String fileName = "/Cards.txt";
        std::vector<String> folderList;
        std::vector<int> folderSize;
        int currentFolderIndex = 0;

};

