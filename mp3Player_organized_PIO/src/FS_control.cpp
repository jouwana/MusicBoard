#include <FS_control.hpp>

FSControl::FSControl()
{
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    Serial.println("SPIFFS Mount Successful");
    creatFileIfNotExist();
}

bool FSControl::checkCardInFile(String cardNumber)
{
    // check if card number already exists in file
    File file = SPIFFS.open(fileName, FILE_READ);
    if (!file)
    {
        Serial.println("There was an error opening the file for reading");
        return false;
    }
    String fileContent = file.readString();
    // check if card number exists in file
    if (fileContent.indexOf(cardNumber) >= 0)
    {
        Serial.println("Card number exists in file");
        return true;
    }
    else
    {
        Serial.println("Card number does not exist in file");
        return false;
    }
}

void FSControl::creatFileIfNotExist()
{
    if (!SPIFFS.exists(fileName))
    {
        Serial.println("File does not exist, creating it");
        writeToFile("");
    }
    else
    {
        Serial.println("File exists");
    }
}

void FSControl::writeToFile(String data)
{
    File file = SPIFFS.open(fileName, FILE_WRITE);
    if (!file)
    {
        Serial.println("There was an error opening the file for writing");
        return;
    }
    if (file.print(data))
    {
        Serial.println("File was written");
    }
    else
    {
        Serial.println("File write failed");
    }
    file.close();
}

void FSControl::appedToFile(String data)
{
    File file = SPIFFS.open(fileName, FILE_APPEND);
    if (!file)
    {
        Serial.println("There was an error opening the file for writing");
        return;
    }
    if (file.print(data))
    {
        Serial.println("File was written");
    }
    else
    {
        Serial.println("File write failed");
    }
    file.close();
}

void FSControl::clearFile()
{
    File file = SPIFFS.open(fileName, FILE_WRITE);
    if (!file)
    {
        Serial.println("There was an error opening the file for writing");
        return;
    }
    if (file.print(""))
    {
        Serial.println("File was cleared");
    }
    else
    {
        Serial.println("File clear failed");
    }
    file.close();
}

// This function will read the file and return the contents
void FSControl::readFile()
{
    File file = SPIFFS.open(fileName, FILE_READ);
    if (!file)
    {
        Serial.println("There was an error opening the file for reading");
        return;
    }
    String fileContent = file.readString();
    Serial.println("File Content:");
    Serial.println(fileContent);
    file.close();
}

bool FSControl::addCardToFile(String cardNumber, String folders)
{
    if (checkCardInFile(cardNumber))
    {
        return false;
    }
    else
    {
        appedToFile(cardNumber + "," + folders + "\n");
        return true;
    }
}

bool FSControl::setCurrentCard(String cardNumber)
{
    folderList.clear();
    folderSize.clear();
    resetFolderIndex();
    File file = SPIFFS.open(fileName, FILE_READ);
    if (!file)
    {
        Serial.println("There was an error opening the file for reading");
        return false;
    }
    String fileContent = file.readString();
    int index = fileContent.indexOf(cardNumber);
    if (index >= 0)
    {
        Serial.println("Card number exists in file");
        int folderIndex = fileContent.indexOf(",", index);
        int folderIndexEnd = fileContent.indexOf("\n", folderIndex);
        String folders = fileContent.substring(folderIndex + 1, folderIndexEnd);
        currentFolderIndex = 0;
        Serial.println("Folders: " + folders);
        size_t pos = 0;
        String token;
        while ((pos = folders.indexOf(',')) != -1)
        {
            token = folders.substring(0, pos);
            folderList.push_back(token);
            folders.remove(0, pos + 1);
        }
        folderList.push_back(folders); // Add the last folder
        Serial.println("Folder list: ");
        for (int i = 0; i < folderList.size(); i++)
        {
            Serial.println(folderList[i]);
        }
        //initialize a folder size list to -1
        for (int i = 0; i < folderList.size(); i++)
        {
            folderSize.push_back(-1);
        }
        return true;
    }
    else
    {
        Serial.println("Card number does not exist in file");
        return false;
    }
}

int FSControl::getCurrentFolder()
{
    return folderList[currentFolderIndex].toInt();
}

int FSControl::getNextFolder()
{
    currentFolderIndex++;
    if (currentFolderIndex >= folderList.size())
    {
        currentFolderIndex = 0;
    }
    return getCurrentFolder();
}

int FSControl::getPrevFolder()
{
    currentFolderIndex--;
    if (currentFolderIndex < 0)
    {
        currentFolderIndex = folderList.size() - 1;
    }
    return getCurrentFolder();
    
}

void FSControl::resetFolderIndex()
{
    currentFolderIndex = 0;
}

void FSControl::setCurrentFolderSize(int size)
{
    folderSize[currentFolderIndex] = size;
}

int FSControl::getCurrentFolderSize()
{
    return folderSize[currentFolderIndex];
}

int FSControl::getNumberOfMappedFolders()
{
    return folderList.size();
}
