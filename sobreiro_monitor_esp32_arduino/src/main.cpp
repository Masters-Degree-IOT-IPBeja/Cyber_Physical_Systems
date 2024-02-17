#include <Arduino.h>
#include "ma_api_logger.h"
#include "ma_api_rain_sensor.h"
#include "ma_api_soil_moisture_sensor.h"

#define SERIAL_BAUD_RATE 115200
#define OFFSET_FOR_LOCAL_TIME 3600

#define LOG_FILE_NAME "/sobreiro_monitor.log"
#define PRINT_ENABLED true

ESP32Time rtc(OFFSET_FOR_LOCAL_TIME);  // offset in seconds GMT+1

void setup() 
{
  Serial.begin(SERIAL_BAUD_RATE);
  PRINTF("Initializing...\n");
  rtc.setTime(0, 0, 0, 1, 1, 2024);  // 17th Jan 2021 15:24:30

  ma_api_logger_init(DEBUG, 200, 5, LOG_FILE_NAME, PRINT_ENABLED);
}

void loop() 
{

}