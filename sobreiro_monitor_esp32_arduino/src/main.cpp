#include "include_set.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatureSensor(&oneWire);

// For BME680 sensor
Adafruit_BME680 bme;

// For BH1750 sensor
BH1750 lightMeter;

// RTC and time-related variables
ESP32Time rtc(OFFSET_FOR_LOCAL_TIME);
volatile unsigned long startTimeMillis = 0;
volatile unsigned long pressDuration = 0;
RTC_DATA_ATTR int bootCount = 0;

// Configuration and state variables
static st_all_sensors_t stAllSensorsData;
static st_all_sensors_t stAllSensorsDataOld;
static e_state_machine_t eStateMachine = eButtonPressed;
static uint8_t countRetryState = 0;

static st_wifi_station_credential_t stWifiCredential;
const gpio_num_t PIN_EXTERNAL_BUTTON_WAKEUP_MASK = GPIO_NUM_27;

// WiFi and MQTT-related variables
WiFiClient wifiClient;
PubSubClient pubSubClient(wifiClient);

st_configs_saved_memory_update_via_mqtt_t stConfigs = {false, 1013.25, 300, 3, false};

void setup() 
{
  #ifdef BROWNOT_OFF
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector. 
  #endif

  Serial.begin(SERIAL_BAUD_RATE);
  rtc.setTime(0, 0, 0, 1, 1, 2024); // 1th Jan 2024 00:00:00

  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    PRINTF("ERROR TO MOUNT SPIFFS\n");
    return;
  }

  ma_api_logger_init(DEBUG, LOG_MAX_FILE_SIZE_KB, LOG_MAX_FILE_COUNT, LOG_FILE_NAME, PRINT_ENABLED);
  LOGGER(INFO, "Initializing...");

  LOGGER(INFO, "Total size SPIFFS: %d bytes", total_size_on_spiffs());
  LOGGER(INFO, "Free size SPIFFS: %d bytes", free_size_on_spiffs());
  LOGGER(INFO, "Used SPIFFS: %d bytes", used_size_on_spiffs());
  LOGGER(INFO, "List files on SPIFFS: \n%s", list_files_and_size_on_spiffs().c_str());

  set_modem_sleep();

  ++bootCount;
  LOGGER(INFO, "Boot number: %s", String(bootCount).c_str());
  logger_wakeup_reason();

  st_configs_saved_memory_update_via_mqtt_t stLoadStruct;
  if(load_struct_on_spiffs(FILE_FOR_CONFIG_STRUCT, &stLoadStruct, sizeof(stLoadStruct)) != -1)
  {
    check_and_update_configs(stLoadStruct);
  }
  LOGGER(DEBUG, "Configs:\n   Average times: %d\n  Config changed: %d\n    Led Status: %d\n   Sea level pressure: %f\n    Sleep time: %d\n", stConfigs.averageTimesToCalculateSensorValues, stConfigs.configChanged, stConfigs.ledStatus, stConfigs.seaLevelPressureHpa, stConfigs.sleepTimeSecond);

  button_init(BUTTON_PIN, INPUT, handle_button_interrupt_routine_service_press, RISING);
  ma_api_rgb_led_init(LED_RED_PIN, LED_GREEN_PIN, LED_BLUE_PIN);
  ma_api_rgb_led_on(RED, stConfigs.ledStatus);

  // Enable wakeup from deep sleep using a timer
  esp_sleep_enable_timer_wakeup(stConfigs.sleepTimeSecond * MICROSECONDS_TO_SECONDS_FACTOR); // Convert seconds to microseconds
  LOGGER(DEBUG, "Setup ESP32 to sleep for every %d Seconds.", stConfigs.sleepTimeSecond * MICROSECONDS_TO_SECONDS_FACTOR);

  esp_sleep_enable_ext0_wakeup(PIN_EXTERNAL_BUTTON_WAKEUP_MASK, HIGH);

  ma_api_wifi_read_network_credentials(&stWifiCredential);
  if(stWifiCredential.stringSsid.length() < 5 || stWifiCredential.stringPassword.length() < 5)
  {
    ma_api_rgb_led_on(BLUE, true);
    ma_api_wifi_setup_access_point(stWifiCredential); // Enable AP mode with WebServer to get news credentials if is necessary

    startTimeMillis = millis();
    while(millis() - startTimeMillis < (TIME_TO_WAIT_WEB_SERVER_SECONDS * MILLIS_TO_SECONDS_FACTOR) && get_button_state() == false)
    {
      ma_api_wifi_process_client_request(&wifiClient, stWifiCredential.stringSsid, stWifiCredential.stringPassword, TIME_TO_WAIT_WEB_SERVER_SECONDS);
    }
  }
  
  if(stWifiCredential.hasCredentials == true)
  {
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    if (!lightMeter.begin()) 
    {
      stAllSensorsData.lightMeterError = true;
      LOGGER(ERROR, "Could not find a valid BH1750 sensor, check wiring!");
    }

    if (!bme.begin()) 
    {
      stAllSensorsData.bme680Error = true;
      LOGGER(ERROR, "Could not find a valid BME680 sensor, check wiring!");
    }

    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms

    temperatureSensor.begin();
    ma_api_rain_sensor_init(RAIN_SENSOR_PIN);
    ma_api_soil_moisture_sensor_init(SOIL_MOISTURE_SENSOR_PIN);
  }
}


void loop() 
{
  if(get_button_state() == true)
  {
    eStateMachine = eButtonPressed;
    set_button_state(false);
  }
    
  switch (eStateMachine)
  {
    case eButtonPressed:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      startTimeMillis = millis();
      while(digitalRead(BUTTON_PIN) == HIGH)
      {
        ma_api_rgb_led_on(WHITE, true);
        delay(DEBOUNCE_DELAY);
      }
      ma_api_rgb_led_off();
      pressDuration = millis() - startTimeMillis;
      PRINTF("PRESS_DURATION : %lu\n", pressDuration);

      if (pressDuration >= startTimeMillis && pressDuration < VERY_LONG_PRESS_DURATION_MICRO_SECONDS) 
      {
        //Disable/Enable led status
        stConfigs.ledStatus = !stConfigs.ledStatus;
        if(stConfigs.ledStatus == true)
        {
          PRINTF("Enabled Led Status\n");
          ma_api_rgb_led_blink(GREEN, 200, 200, 3, true);
        }
        else
        {
          PRINTF("Disabled Led Status\n");
          ma_api_rgb_led_blink(GREEN, 200, 200, 1, true);
        }
      } 
      else if (pressDuration >= VERY_LONG_PRESS_DURATION_MICRO_SECONDS) 
      {
        // Format Spiffs
        PRINTF("Formating SPIFFS in progress and reboot.\n");
        ma_api_rgb_led_blink(RED, 200, 200, 5, true);
        if (!SPIFFS.format()) 
        {
          PRINTF("Erro ao formatar o SPIFFS.\n");
          ma_api_rgb_led_blink(RED, 200, 200, 3, true);
        }
        else
        {
          esp_restart();
        }
        
      } 
      else 
      {
        //eStateMachine = Idle;
        //Serial.println("Short pressed");
      }
      ma_api_rgb_led_off();

      if (stWifiCredential.hasCredentials == true)
      {
        eStateMachine = eLoadLastSavedSensorData;
      }
      else
      {
        eStateMachine = eSleepMode;
      }
      break;

    case eLoadLastSavedSensorData:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      if(load_struct_on_spiffs(SENSORS_DATA_FILE_NAME , &stAllSensorsDataOld, sizeof(stAllSensorsDataOld)) != -1 || countRetryState >= 4)
      {
        PRINTF("Sensors value read on memory\n");
        logger_sensors_data(stAllSensorsDataOld);
        eStateMachine = eReadSensors;
      }
      else
      {
        countRetryState++;
      }
      break;

    case eReadSensors:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      ma_api_rgb_led_on(GREEN, stConfigs.ledStatus);
      get_all_sensors_average_value(stConfigs.seaLevelPressureHpa, bme, lightMeter, temperatureSensor, &stAllSensorsData, stConfigs.averageTimesToCalculateSensorValues);
      eStateMachine = eCompareOldNewSensorsData;      
      break;
    
    case eCompareOldNewSensorsData:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));

      if (compare_all_sensors_struct(stAllSensorsData, stAllSensorsDataOld)) 
      {
        eStateMachine = eSaveSensorsData;
      }
      else
      {
        eStateMachine = eSleepMode;
      }
      break;

    case eSaveSensorsData:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      if(save_struct_on_spiffs(SENSORS_DATA_FILE_NAME , &stAllSensorsData, sizeof(stAllSensorsData)) != -1)
      {
        PRINTF("Sensors value saved on memory\n");
        logger_sensors_data(stAllSensorsData);
      }
      else
      {
        PRINTF("Error to save sensors value on memory\n");
      }
      eStateMachine = eWifiActive;
      break;

    case eWifiActive:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      WiFi.mode(WIFI_STA); // Active mode
      eStateMachine = eWifiConnect;
      break;

    case eWifiConnect:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      if(ma_api_wifi_setup_station(stWifiCredential, WIFI_ATTEMPTS) == 0)
      {
        ma_api_rgb_led_on(CYAN, stConfigs.ledStatus);
        eStateMachine = eBrokerInitConfig;
      }
      else
      {
        ma_api_rgb_led_blink(CYAN, 250, 250, 3, stConfigs.ledStatus);
        eStateMachine = eSleepMode;
      }
      break;

    case eBrokerInitConfig:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      pubSubClient.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
      pubSubClient.setCallback(mqtt_callback);
      eStateMachine = eBrokerConnect;
      break;

    case eBrokerConnect:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      if(mqtt_broker_reconnect(&pubSubClient, BROKER_ATTEMPTS) == 0)
      {
        ma_api_rgb_led_on(PURPLE, stConfigs.ledStatus);
        PRINTF("MQTT Connected\n");
        eStateMachine = eBrokerSentData;
      }
      else
      {
        ma_api_rgb_led_blink(PURPLE, 250, 250, 3, stConfigs.ledStatus);
        eStateMachine = eSleepMode;
      }
      break;

    case eBrokerSentData:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      publish_sensors_data(&pubSubClient, &stAllSensorsData);
      publish_configs_data(&pubSubClient, &stConfigs);
      eStateMachine = eBrokerGetSettings;
      break;

    case eBrokerGetSettings:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      pubSubClient.subscribe(GET_LED_STATUS_TOPIC);     
      pubSubClient.subscribe(GET_AVERAGE_TIMES_TOPIC);  
      pubSubClient.subscribe(GET_SEA_LEVEL_PRESSURE_TOPIC);  
      pubSubClient.subscribe(GET_SLEEP_TIME_TOPIC);  
      startTimeMillis = millis();
      while (millis() - startTimeMillis < TIME_TO_WAIT_RECEIVED_DATA_MQTT) 
      {
        pubSubClient.loop(); // Allow the MQTT client to process messages
      }
      eStateMachine = eSaveConfigs;
      break;

    case eSaveConfigs:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      LOGGER(DEBUG, "Configs to save:\n   Average times: %d\n  Config changed: %d\n    Led Status: %d\n   Sea level pressure: %f\n    Sleep time: %d\n", 
              stConfigs.averageTimesToCalculateSensorValues, stConfigs.configChanged, stConfigs.ledStatus, stConfigs.seaLevelPressureHpa, stConfigs.sleepTimeSecond);
      if(stConfigs.configChanged == true)
      {
        if(save_struct_on_spiffs(FILE_FOR_CONFIG_STRUCT, &stConfigs, sizeof(stConfigs)) != -1)
        {
          stConfigs.configChanged = false;
        }
      }
      
      eStateMachine = eSleepMode;
      break;
    case eSleepMode:
      PRINTF("On state machine in case: %s\n", enum_state_machine_to_string(eStateMachine));
      // Enter deep sleep mode
      LOGGER(DEBUG, "Going to sleep now.");
      Serial.flush(); 
      esp_deep_sleep_start();
      break;

    default:
      break;
  }
}
