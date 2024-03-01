#include "wifi_and_mqtt.h"
#include "ma_api_logger.h"
#include "config.h"
#include "common.h"

int8_t mqtt_broker_reconnect(PubSubClient *in_PubSubClient, int maxAttempts) 
{
    int attempts = 0;

    // Loop until we're reconnected or reach max attempts
    while (!in_PubSubClient->connected() && attempts < maxAttempts) 
    {
        LOGGER(INFO, "Attempting MQTT connection...");
        // Attempt to connect
        if (in_PubSubClient->connect("SobreiroMonitorDevice", MQTT_BROKER_USER, MQTT_BROKER_PASS)) 
        {
            LOGGER(INFO, "Connected on Broker\n");
            return 0; // Success
        } 
        else 
        {
            LOGGER(INFO, "failed, rc= %s. Retrying in 1 second\n", in_PubSubClient->state());
            // Wait 1 second before retrying
            delay(1000);
            attempts++;
        }
    }

    return -1; // Error: failed to connect after max attempts
}


void mqtt_callback(char* topic, byte* message, unsigned int length) 
{
  // Print received message
  PRINTF("Message arrived on topic: %s. Message: ", topic);
  for (unsigned int i = 0; i < length; i++) {
      PRINTF("%c", (char)message[i]);
  }
  PRINTF("\n");

  // Convert message to null-terminated character array
  char msg[length + 1];
    memcpy(msg, message, length);
    msg[length] = '\0';

  // Check topic and update corresponding configuration
  if (strcmp(topic, GET_LED_STATUS_TOPIC) == 0) 
  {
    stConfigs.ledStatus = ((msg[0] == '1')); // Assuming '1' represents LED on and '0' represents LED off
    stConfigs.configChanged = true;
  } 
  else if (strcmp(topic, GET_AVERAGE_TIMES_TOPIC) == 0) 
  {
    stConfigs.averageTimesToCalculateSensorValues = (atoi(msg));
    stConfigs.configChanged = true;
  } 
  else if (strcmp(topic, GET_SEA_LEVEL_PRESSURE_TOPIC) == 0) 
  {
    stConfigs.seaLevelPressureHpa = (atof(msg));
    stConfigs.configChanged = true;
  } 
  else if (strcmp(topic, GET_SLEEP_TIME_TOPIC) == 0) 
  {
    stConfigs.sleepTimeSecond = (atoi(msg));
    stConfigs.configChanged = true;
  } 
  else 
  {
      // Unknown topic
  }
}


void check_and_update_configs(st_configs_saved_memory_update_via_mqtt_t in_newConfigs)
{
  if(in_newConfigs.seaLevelPressureHpa > 500)
  {
    stConfigs.seaLevelPressureHpa = in_newConfigs.seaLevelPressureHpa;
    stConfigs.configChanged = true;
  }
  
  if(in_newConfigs.sleepTimeSecond >= MINIMUM_SLEEP_TIME_SECONDS)
  {
    stConfigs.sleepTimeSecond = in_newConfigs.sleepTimeSecond;
    stConfigs.configChanged = true;
  }

  if(in_newConfigs.averageTimesToCalculateSensorValues > 3)
  {
    stConfigs.averageTimesToCalculateSensorValues = in_newConfigs.averageTimesToCalculateSensorValues;
    stConfigs.configChanged = true;
  }

  if(in_newConfigs.ledStatus != stConfigs.ledStatus)
  {
    stConfigs.ledStatus = in_newConfigs.ledStatus;
    stConfigs.configChanged = true;
  }
}

/*
void set_config_changed(bool in_configChanged)
{
  stConfigs.configChanged = in_configChanged;
}

void set_config_sleep_time_second(unsigned int in_sleepTimeSecond)
{
  stConfigs.sleepTimeSecond = in_sleepTimeSecond;
}

void set_config_sea_level_pressure_hpa(double in_seaLevelPressureHpa)
{
  stConfigs.seaLevelPressureHpa = in_seaLevelPressureHpa;
}

void set_config_average_times_to_calculate_sensor_values(uint8_t in_averageTimesToCalculateSensorValues)
{
  stConfigs.averageTimesToCalculateSensorValues = in_averageTimesToCalculateSensorValues;
}

void set_config_led_status(bool in_ledStatus)
{
  stConfigs.ledStatus = in_ledStatus;
}

bool get_config_led_status(void)
{
  return stConfigs.ledStatus;
}

bool get_config_changed(void)
{
  return stConfigs.configChanged;
}

unsigned int get_config_sleep_time_second(void)
{
  return stConfigs.sleepTimeSecond;
}

double get_config_sea_level_pressure_hpa(void)
{
  return stConfigs.seaLevelPressureHpa;
}

uint8_t get_config_average_times_to_calculate_sensor_values(void)
{
  return stConfigs.averageTimesToCalculateSensorValues;
}

bool get_config_led_status(void)
{
  return stConfigs.ledStatus;
}

*/