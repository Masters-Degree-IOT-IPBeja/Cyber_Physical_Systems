#include "common.h"
#include "config.h"
#include "ma_api_rgb_led.h"
#include "ma_api_logger\ma_api_logger.h"
#include <WiFi.h>

volatile unsigned long lastDebounceTime = 0; // To use when button is pressed
static bool buttonState = false;

const char* ccEnumStateMachineToString[] = {
  [eButtonPressed] = "eButtonPressed",
  [eLoadLastSavedSensorData] = "eLoadLastSavedSensorData",
  [eReadSensors] = "eReadSensors",
  [eCompareOldNewSensorsData] = "eCompareOldNewSensorsData",
  [eSaveSensorsData] = "eSaveSensorsData",
  [eWifiActive] = "eWifiActive",
  [eWifiConnect] = "eWifiConnect",
  [eBrokerInitConfig] = "eBrokerInitConfig",
  [eBrokerConnect] = "eBrokerConnect",
  [eBrokerSentData] = "eBrokerSentData",
  [eBrokerGetSettings] = "eBrokerGetSettings",
  [eSaveConfigs] = "eSaveConfigs",
  [eSleepMode] = "eSleepMode",
};

void set_modem_sleep(void) 
{
    WiFi.setSleep(true);
    if (!setCpuFrequencyMhz(80))
    {
        Serial2.println("Not valid frequency!");
    }
    // Use this if 40Mhz is not supported
    // setCpuFrequencyMhz(80);
}

void button_init(int in_pin, int in_mode, void (*isr)(void), int in_trigger) 
{
    buttonState = false;
    pinMode(in_pin, in_mode);
    attachInterrupt(digitalPinToInterrupt(in_pin), isr, in_trigger);
}

void set_button_state(bool in_buttonState)
{
  buttonState = in_buttonState;
}

bool get_button_state(void)
{
  return buttonState;
}

void handle_button_interrupt_routine_service_press(void) 
{
  unsigned long currentMillis = millis();

  // Check if enough time has passed since the last press
  if (currentMillis - lastDebounceTime >= DEBOUNCE_DELAY) 
  {
    ma_api_rgb_led_on(MAGENTA, true);
    lastDebounceTime = currentMillis;
    
    // Indicates that the button has been pressed
    buttonState = true;
    PRINTF("BUTTON PRESSED\n");
  }
}

/*
Method to logger the reason by which ESP32
has been awaken from sleep
*/
void logger_wakeup_reason(void)
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : LOGGER(DEBUG, "Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : LOGGER(DEBUG, "Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : LOGGER(DEBUG, "Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : LOGGER(DEBUG, "Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : LOGGER(DEBUG, "Wakeup caused by ULP program"); break;
    default : LOGGER(DEBUG, "Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

const char *enum_state_machine_to_string(e_state_machine_t in_enum)
{
  return ccEnumStateMachineToString[in_enum];
}