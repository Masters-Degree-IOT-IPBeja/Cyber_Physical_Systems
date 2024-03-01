#include "sensors.h"
#include "config.h"
#include "ma_api_logger\ma_api_logger.h"
#include "ma_api_rain_sensor\ma_api_rain_sensor.h"
#include "ma_api_soil_moisture_sensor\ma_api_soil_moisture_sensor.h"

void get_all_sensors_average_value(double in_seaLevelPressureHpa, Adafruit_BME680 in_bme, BH1750 in_lightMeterSensor, DallasTemperature in_soilTemperature, st_all_sensors_t *out_allSensorsAverageValue, uint8_t in_timesToAverage)
{
  st_all_sensors_t stAllSensorsSum;
  memset(&stAllSensorsSum, 0, sizeof(st_all_sensors_t));

  for(int i = 0; i < in_timesToAverage; i++)
  {
    // Check for errors in the BME680 sensor
    if(out_allSensorsAverageValue->bme680Error == false)
    {
      in_bme.performReading();
      stAllSensorsSum.airTemperatureCelsius += in_bme.temperature;
      stAllSensorsSum.airRelativeHumidityPercentage += in_bme.humidity;
      stAllSensorsSum.airPressurePa += in_bme.pressure;
      stAllSensorsSum.altitudeMeters += in_bme.readAltitude(in_seaLevelPressureHpa);
      stAllSensorsSum.airGasResistance += (in_bme.gas_resistance / 1000.0);
    }

    in_soilTemperature.requestTemperatures();
    stAllSensorsSum.soilTemperatureCelsius += in_soilTemperature.getTempCByIndex(0);

    stAllSensorsSum.lighLux += in_lightMeterSensor.readLightLevel();
  }

  // Calculate the average by dividing the sums by the number of readings
  out_allSensorsAverageValue->airTemperatureCelsius         = stAllSensorsSum.airTemperatureCelsius / in_timesToAverage;
  out_allSensorsAverageValue->airRelativeHumidityPercentage = stAllSensorsSum.airRelativeHumidityPercentage / in_timesToAverage;
  out_allSensorsAverageValue->airPressurePa                = stAllSensorsSum.airPressurePa / in_timesToAverage;
  out_allSensorsAverageValue->altitudeMeters                = stAllSensorsSum.altitudeMeters / in_timesToAverage;
  out_allSensorsAverageValue->airGasResistance              = stAllSensorsSum.airGasResistance / in_timesToAverage;
  out_allSensorsAverageValue->soilTemperatureCelsius        = stAllSensorsSum.soilTemperatureCelsius / in_timesToAverage;
  out_allSensorsAverageValue->lighLux                       = stAllSensorsSum.lighLux / in_timesToAverage;

  out_allSensorsAverageValue->soilHumidityPercentage        = ma_api_soil_moisture_sensor_average_percentage_value(in_timesToAverage);
  out_allSensorsAverageValue->rainPercentage                = ma_api_rain_sensor_average_percentage_value(in_timesToAverage);


  LOGGER(INFO, "New sensors average values: ");
  logger_sensors_data(*out_allSensorsAverageValue);
}

bool compare_all_sensors_struct(st_all_sensors_t data1, st_all_sensors_t data2) 
{
  if (data1.airTemperatureCelsius == 0 &&
      data1.airRelativeHumidityPercentage == 0 &&
      data1.airPressurePa == 0 &&
      data1.altitudeMeters == 0 &&
      data1.airGasResistance == 0 &&
      data1.soilTemperatureCelsius == 0 &&
      data1.soilHumidityPercentage == 0 &&
      data1.rainPercentage == 0 &&
      data1.lighLux == 0) 
  {
    return false;
  }

  if (data1.airTemperatureCelsius != data2.airTemperatureCelsius ||
      data1.airRelativeHumidityPercentage != data2.airRelativeHumidityPercentage ||
      data1.airPressurePa != data2.airPressurePa ||
      data1.altitudeMeters != data2.altitudeMeters ||
      data1.airGasResistance != data2.airGasResistance ||
      data1.soilTemperatureCelsius != data2.soilTemperatureCelsius ||
      data1.soilHumidityPercentage != data2.soilHumidityPercentage ||
      data1.rainPercentage != data2.rainPercentage ||
      data1.lighLux != data2.lighLux) 
  {
    return true;
  }

  return false;
}

void logger_sensors_data(st_all_sensors_t in_sensorsData)
{
  LOGGER(INFO, "  Air Temperature (ºC): %f",      in_sensorsData.airTemperatureCelsius);
  LOGGER(INFO, "  Air Relative Humidity (%%): %f",in_sensorsData.airRelativeHumidityPercentage);
  LOGGER(INFO, "  Air Pressure (PA): %lu",       in_sensorsData.airPressurePa);
  LOGGER(INFO, "  Gas resistance (Kohms): %lu",   in_sensorsData.airGasResistance);
  LOGGER(INFO, "  Altitude (Meters): %f",         in_sensorsData.altitudeMeters);
  LOGGER(INFO, "  Soil Temperature (ºC): %f",     in_sensorsData.soilTemperatureCelsius);
  LOGGER(INFO, "  Lux: %f",                       in_sensorsData.lighLux);
  LOGGER(INFO, "  Soil Humidity (%%): %d",        in_sensorsData.soilHumidityPercentage);
  LOGGER(INFO, "  Rain (%%): %d\n",               in_sensorsData.rainPercentage);
}