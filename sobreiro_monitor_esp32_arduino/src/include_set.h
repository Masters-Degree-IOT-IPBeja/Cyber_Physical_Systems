#ifndef __INCLUDE_SET__
#define __INCLUDE_SET__

#include <Arduino.h>
#include <PubSubClient.h>

// For BME680 sensor Adafruit Lib
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// For BH1750 sensor
#include "BH1750.h"
#include <Wire.h>

#include "DallasTemperature.h"
#include "OneWire.h"

#include "ma_api_wifi_auto_ap_station.h"
#include "ma_api_rain_sensor.h"
#include "ma_api_soil_moisture_sensor.h"
#include "ma_api_rgb_led.h"
#include "ma_api_logger.h"

#include "wifi_and_mqtt.h"
#include "common.h"
#include "sensors.h"
#include "config.h"
#include "publish_data.h"
#include "wifi_and_mqtt.h"
#include "file_system.h"

#endif // __INCLUDE_SET__