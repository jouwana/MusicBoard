#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

enum CARDTYPE
{
    PREVIOUS,
    AUTHORIZED,
    UNSUPPORTED,
    NO_CARD
};

// Definitions Pin and setups
#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4];

// struct to hold RFID card ID
struct RFIDCard
{
    byte hexID[4];
    int folderId;
};

// Init array to store scanned RFID card IDs
RFIDCard scannedCards[2] = {
    {{0x59, 0xEA, 0xD4, 0xA3}, 1}, {{0xC9, 0xA6, 0xC1, 0xA2}, 2}};

// add function definition to all the helper functions used in the code

void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);

CARDTYPE handle_reads()
{

    CARDTYPE cardType = NO_CARD;

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!rfid.PICC_IsNewCardPresent())
    {
        return NO_CARD;
    }

    // Verify if the NUID has been readed
    if (!rfid.PICC_ReadCardSerial())
        return NO_CARD;

    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return UNSUPPORTED;
    }

    if (rfid.uid.uidByte[0] != nuidPICC[0] ||
        rfid.uid.uidByte[1] != nuidPICC[1] ||
        rfid.uid.uidByte[2] != nuidPICC[2] ||
        rfid.uid.uidByte[3] != nuidPICC[3])
    {
        Serial.println(F("A new card has been detected."));
        cardType = AUTHORIZED;
        // Store NUID into nuidPICC array
        for (byte i = 0; i < 4; i++)
        {
            nuidPICC[i] = rfid.uid.uidByte[i];
        }

        Serial.println(F("The NUID tag is:"));
        Serial.print(F("In hex: "));
        printHex(rfid.uid.uidByte, rfid.uid.size);
        Serial.println();
        Serial.print(F("In dec: "));
        printDec(rfid.uid.uidByte, rfid.uid.size);
        Serial.println();
    }
    else
    {
        cardType = PREVIOUS;
        Serial.println(F("Card read previously."));
    }

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();

    return cardType;
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        Serial.print(' ');
        Serial.print(buffer[i], DEC);
    }
}
