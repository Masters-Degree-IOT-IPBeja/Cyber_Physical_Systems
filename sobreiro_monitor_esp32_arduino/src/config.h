#ifndef __CONFIG_H__
#define __CONFIG_H__

#define ONE_WIRE_BUS                            4   // Pino para o sensor de temperatura DS18B20 OneWire
#define RAIN_SENSOR_PIN                         35  // Pino para o sensor de chuva analógico
#define SOIL_MOISTURE_SENSOR_PIN                36  // Pino para o sensor de umidade do solo analógico

#define BUTTON_PIN                              27  // Pino para o botão físico

#define LED_RED_PIN                             16
#define LED_GREEN_PIN                           17  
#define LED_BLUE_PIN                            18 
#define LED_ENABLE_PIN                          19

#define I2C_SDA_PIN                             21
#define I2C_SCL_PIN                             22

#define SERIAL_BAUD_RATE                        115200
#define OFFSET_FOR_LOCAL_TIME                   3600


#define SENSORS_DATA_FILE_NAME                  "/struct.sensors"
#define FILE_FOR_CONFIG_STRUCT                  "/struct.configs"

// LOGGER Configs
#define LOG_FILE_NAME                           "/sm.log"
#define LOG_MAX_FILE_SIZE_KB                    1
#define LOG_MAX_FILE_COUNT                      3
#define PRINT_ENABLED                           true //true or false
#define FORMAT_SPIFFS_IF_FAILED                 true //true or false
#define LOGGER(level, ...)                      ma_api_log_message(level, __VA_ARGS__)

#define DEBOUNCE_DELAY                          100 // milissegundos para o Debounce do botão
#define MICROSECONDS_TO_SECONDS_FACTOR          1000000 /* Fator de conversão de microssegundos para segundos */ 
#define MILLIS_TO_SECONDS_FACTOR                1000 /* Fator de conversão de milissegundos para segundos */ 

#define MINIMUM_SLEEP_TIME_SECONDS               10 // 300s default

#define LONG_PRESS_DURATION_MICRO_SECONDS       3000  // timeButtonPressedToChangeStatusLed = 3000us default
#define VERY_LONG_PRESS_DURATION_MICRO_SECONDS  10000  // timeButtonPressedToFormatMemory = 10000us default


#endif // __CONFIG_H__
