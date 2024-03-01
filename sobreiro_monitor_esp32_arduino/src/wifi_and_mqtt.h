#ifndef __WIFI_AND_MQTT_H__
#define __WIFI_AND_MQTT_H__

#include <Arduino.h>
#include <PubSubClient.h>

// COMMUNICATIONS Configs
#define MQTT_BROKER_PORT    1883
#define MQTT_BROKER_USER    "admin"
#define MQTT_BROKER_PASS    "admin"
#define MQTT_BROKER_IP      "192.168.1.151"
#define MQTT_QOS            1 // 0, 1 ou 2

#define WIFI_ATTEMPTS       3
#define BROKER_ATTEMPTS     10

#define TIME_TO_WAIT_RECEIVED_DATA_MQTT 1000
#define TIME_TO_WAIT_WEB_SERVER_SECONDS 300 // 300s = 5 minutos 

// Define strings de tópicos de configuração
#define GET_LED_STATUS_TOPIC                    "project/configs/set/led_status"
#define GET_AVERAGE_TIMES_TOPIC                 "project/configs/set/average_times_to_calculate_sensor_values"
#define GET_SEA_LEVEL_PRESSURE_TOPIC            "project/configs/set/sea_level_pressure_hpa"
#define GET_SLEEP_TIME_TOPIC                    "project/configs/set/sleep_time_second"

// Define strings de tópicos de configuração do dispositivo atual
#define INFO_LED_STATUS_TOPIC                   "project/configs/device/led_status"
#define INFO_AVERAGE_TIMES_TOPIC                "project/configs/device/average_times_to_calculate_sensor_values"
#define INFO_SEA_LEVEL_PRESSURE_TOPIC           "project/configs/device/sea_level_pressure_hpa"
#define INFO_SLEEP_TIME_TOPIC                   "project/configs/device/sleep_time_second"

#define SENT_SENSOR_AIR_TEMPERATURE_DATA_TOPIC  "project/sensors/air_temperature_celsius"
#define SENT_SENSOR_AIR_HUMIDITY_DATA_TOPIC     "project/sensors/air_humidity_percentage"
#define SENT_SENSOR_AIR_PRESSURE_DATA_TOPIC     "project/sensors/air_pressure_pa"
#define SENT_SENSOR_GAS_RESISTANCE_DATA_TOPIC   "project/sensors/gas_resistante_kohms"
#define SENT_SENSOR_ALTITUDE_METERS_DATA_TOPIC  "project/sensors/altitude_meters"
#define SENT_SENSOR_SOIL_TEMPERATURE_DATA_TOPIC "project/sensors/soil_temperature_celsius"
#define SENT_SENSOR_SOIL_HUMIDITY_DATA_TOPIC    "project/sensors/soil_humidity_percentage"
#define SENT_SENSOR_LIGHT_LUX_DATA_TOPIC        "project/sensors/lightmeter_lux"
#define SENT_SENSOR_RAIN_DATA_TOPIC             "project/sensors/rain_prrcentage"

typedef struct
{
  bool ledStatus; // project/configs/led_status = 0 or 1
  double seaLevelPressureHpa; // project/configs/sea_level_pressure_hpa = 1013.25 default
  unsigned int sleepTimeSecond; // project/configs/sleep_time_second = 300 = 5 Minutes minimum and default
  uint8_t averageTimesToCalculateSensorValues; // project/configs/average_times_to_calculate_sensor_values = 3 times minimum and default
  bool configChanged;
}st_configs_saved_memory_update_via_mqtt_t;

extern st_configs_saved_memory_update_via_mqtt_t stConfigs; 

extern int8_t mqtt_broker_reconnect(PubSubClient *in_PubSubClient, int maxAttempts);
extern void mqtt_callback(char* topic, byte* message, unsigned int length); 

extern void check_and_update_configs(st_configs_saved_memory_update_via_mqtt_t in_newConfigs);
/*
extern void set_config_led_status(bool in_ledStatus);
extern void set_config_average_times_to_calculate_sensor_values(uint8_t in_averageTimesToCalculateSensorValues);
extern void set_config_sea_level_pressure_hpa(double in_seaLevelPressureHpa);
extern void set_config_sleep_time_second(unsigned int in_sleepTimeSecond);
extern void set_config_changed(bool in_configChanged);
extern bool get_config_led_status(void);
extern bool get_config_changed();
extern unsigned int get_config_sleep_time_second();
extern double get_config_sea_level_pressure_hpa();
extern uint8_t get_config_average_times_to_calculate_sensor_values();
extern bool get_config_led_status();
*/

#endif // __WIFI_AND_MQTT_H__