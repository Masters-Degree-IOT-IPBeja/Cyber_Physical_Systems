#include "publish_data.h"
#include "config.h"
#include "wifi_and_mqtt.h"

void publish_sensors_data(PubSubClient *in_PubSubClient, st_all_sensors_t *data) 
{
  char message[256];
  int payloadLength = 0;
  
  sprintf(message, "%.2f", data->airTemperatureCelsius);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_AIR_TEMPERATURE_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%.2f", data->airRelativeHumidityPercentage);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_AIR_HUMIDITY_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%lu", data->airPressurePa);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_AIR_PRESSURE_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%lu", data->airGasResistance);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_GAS_RESISTANCE_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%.2f", data->altitudeMeters);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_ALTITUDE_METERS_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%.2f", data->soilTemperatureCelsius);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_SOIL_TEMPERATURE_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%d", data->soilHumidityPercentage);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_SOIL_HUMIDITY_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%.2f", data->lighLux);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_LIGHT_LUX_DATA_TOPIC, message, payloadLength);

  sprintf(message, "%d", data->rainPercentage);
  payloadLength = strlen(message);
  in_PubSubClient->publish(SENT_SENSOR_RAIN_DATA_TOPIC, message, payloadLength);
}

void publish_configs_data(PubSubClient *in_PubSubClient, st_configs_saved_memory_update_via_mqtt_t *in_configs) 
{
  char message[256];
  int payloadLength = 0;
  
  sprintf(message, "%d", in_configs->averageTimesToCalculateSensorValues);
  payloadLength = strlen(message);
  in_PubSubClient->publish(INFO_AVERAGE_TIMES_TOPIC, message, payloadLength);

  sprintf(message, "%d", (int)in_configs->ledStatus);
  payloadLength = strlen(message);
  in_PubSubClient->publish(INFO_LED_STATUS_TOPIC, message, payloadLength);

  sprintf(message, "%f", in_configs->seaLevelPressureHpa);
  payloadLength = strlen(message);
  in_PubSubClient->publish(INFO_SEA_LEVEL_PRESSURE_TOPIC, message, payloadLength);

  sprintf(message, "%d", in_configs->sleepTimeSecond);
  payloadLength = strlen(message);
  in_PubSubClient->publish(INFO_SLEEP_TIME_TOPIC, message, payloadLength);
}