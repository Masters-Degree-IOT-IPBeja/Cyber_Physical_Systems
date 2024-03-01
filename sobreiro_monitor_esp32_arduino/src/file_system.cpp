#include "file_system.h"
#include "ma_api_logger.h"
#include "config.h"

#include <FS.h>

// Function to save the configuration data to SPIFFS
int8_t save_struct_on_spiffs(const char * in_filePathToSave, void *in_struct, size_t in_sizeOfStruct) 
{
  File configFile = SPIFFS.open(in_filePathToSave, "w");
  if (!configFile) 
  {
    LOGGER(INFO, "Failed to open config file %s for writing", in_filePathToSave);
    return -1;
  }

  size_t bytesWritten = configFile.write((uint8_t*)in_struct, in_sizeOfStruct);
  configFile.close();
  LOGGER(INFO, "Struct saved on file: %s", in_filePathToSave);
  return bytesWritten == in_sizeOfStruct;
}

// Function to read the configuration data from SPIFFS
int8_t load_struct_on_spiffs(const char *in_filePathToRead, void *out_struct, size_t in_sizeOfStruct)
{
  File configFile = SPIFFS.open(in_filePathToRead, "r");
  if (!configFile) 
  {
    LOGGER(INFO, "Failed to open config file %s for reading", in_filePathToRead);
    return -1;
  }

  size_t bytesRead = configFile.read((uint8_t*)out_struct, in_sizeOfStruct);
  configFile.close();

  LOGGER(INFO, "Struct read fron file: %s with Bytes: %d", in_filePathToRead, bytesRead);
  return bytesRead;
}

// Returns the total size of SPIFFS
size_t total_size_on_spiffs(void) 
{
    return SPIFFS.totalBytes();
}

// Returns the used size of SPIFFS
size_t used_size_on_spiffs(void) 
{
    return SPIFFS.usedBytes();
}

// Returns the free size of SPIFFS
size_t free_size_on_spiffs(void) 
{
    return total_size_on_spiffs() - used_size_on_spiffs();
}

// Returns a list of all files in SPIFFS with their sizes
String list_files_and_size_on_spiffs(void) 
{
    String fileList = "";
    File root = SPIFFS.open("/");
    
    if (!root) 
    {
        Serial.println("Failed to open root directory");
        return "";
    }

    File file = root.openNextFile();
    while (file) 
    {
        // Add file name to the list
        fileList += file.name();
        fileList += " - Size: ";
        // Get the file size
        fileList += file.size();
        fileList += " bytes\n";
        file = root.openNextFile();
    }

    root.close(); // Close the directory after reading
    return fileList;
}


// Function to read binary files in SPIFFS
bool read_binary_file_on_spiffs(const char* fileName) 
{
  File file = SPIFFS.open(fileName, "rb"); // Open the file in binary read mode
  
  if (!file) 
  {
    Serial.println("Error opening binary file");
    return false;
  }

  // Read file data and perform necessary processing
  // Here you can read the data directly into a data structure
  
  file.close(); // Close the file after reading
  return true;
}


bool read_text_file_on_spiffs(const char* fileName) 
{
  File file = SPIFFS.open(fileName, "r"); // Open the file in text read mode
  
  if (!file) 
  {
    Serial.println("Error opening text file");
    return false;
  }

  // Get the file size
  size_t fileSize = file.size();
  Serial.print("File size: ");
  Serial.println(fileSize);

  // Read file data line by line or as needed
  while (file.available()) 
  {
    String line = file.readStringUntil('\n');
    // Process the line as needed
    Serial.println(line);
  }
  
  file.close(); // Close the file after reading
  return true;
}
