#ifndef __SENSORS_H__
#define __SENSORS_H__

#include "Arduino.h"

// For BME680 sensor Adafruit Lib
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// For BH1750 sensor
#include "BH1750.h"
#include <Wire.h>

#include "DallasTemperature.h"
#include "OneWire.h"

typedef struct 
{
  float airTemperatureCelsius; //BME680
  float airRelativeHumidityPercentage; //BME680
  uint32_t airPressurePa; //BME680
  float altitudeMeters; //BME680
  uint32_t airGasResistance; //Kohms BME680
  bool bme680Error = false;

  float soilTemperatureCelsius; //DS18b20
  bool soilTemperatureError = false;

  uint16_t soilHumidityPercentage; //Soil moisture sensor
  bool soilMoistureSensorError = false;
  
  uint16_t rainPercentage; // Rain sensor
  bool rainSensorError = false;

  float lighLux; // BH1750 sensor
  bool lightMeterError = false;
  // Define the structure for holding sensor data
} st_all_sensors_t;

extern void get_all_sensors_average_value(double in_seaLevelPressureHpa, Adafruit_BME680 in_bme, BH1750 in_lightMeterSensor, DallasTemperature in_soilTemperature, st_all_sensors_t *out_allSensorsAverageValue, uint8_t in_timesToAverage);
extern bool compare_all_sensors_struct(st_all_sensors_t data1, st_all_sensors_t data2); 
extern void logger_sensors_data(st_all_sensors_t in_sensorsData);

#endif // __SENSORS_H__