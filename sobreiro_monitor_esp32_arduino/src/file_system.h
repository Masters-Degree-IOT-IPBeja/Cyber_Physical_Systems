#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <Arduino.h>

// Function to save the configuration data to SPIFFS
int8_t save_struct_on_spiffs(const char * in_filePathToSave, void *in_struct, size_t in_sizeOfStruct);

// Function to read the configuration data from SPIFFS
int8_t load_struct_on_spiffs(const char *in_filePathToRead, void *out_struct, size_t in_sizeOfStruct);

// Returns the total size of SPIFFS
size_t total_size_on_spiffs(void);

// Returns the used size of SPIFFS
size_t used_size_on_spiffs(void);

// Returns the free size of SPIFFS
size_t free_size_on_spiffs(void);

// Returns a list of all files in SPIFFS with their sizes
String list_files_and_size_on_spiffs(void);

// Function to read binary files in SPIFFS
bool read_binary_file_on_spiffs(const char* fileName);

// Function to read text files in SPIFFS
bool read_text_file_on_spiffs(const char* fileName);

#endif  // __FILE_SYSTEM_H__
