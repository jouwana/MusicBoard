#define FS_CONTROL_HPP
#include <Arduino.h>
#include <SPIFFS.h>
#include <FS.h>


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
        int getNextFolder();
        int getPrevFolder();
    private:
        void writeToFile(String data);
        void appedToFile(String data);

        String fileName = "/Cards.txt";
        String folders ="";
        int currentFolderIndex = 0;

};

