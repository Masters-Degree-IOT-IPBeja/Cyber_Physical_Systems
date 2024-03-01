#ifndef __COMMON_H__
#define __COMMON_H__

#include <Arduino.h>

typedef enum 
{
  eButtonPressed = 0,
  eLoadLastSavedSensorData,
  eReadSensors,
  eCompareOldNewSensorsData,
  eSaveSensorsData,
  eWifiActive,
  eWifiConnect,
  eBrokerInitConfig,
  eBrokerConnect,
  eBrokerSentData,
  eBrokerGetSettings,
  eSaveConfigs,
  eSleepMode,
}e_state_machine_t;


extern void set_modem_sleep(void);
extern void logger_wakeup_reason(void);
extern void handle_button_interrupt_routine_service_press(void); 
extern void button_init(int in_pin, int in_mode, void (*isr)(void), int in_trigger);
extern void set_button_state(bool in_buttonState);
extern bool get_button_state(void);
extern const char *enum_state_machine_to_string(e_state_machine_t in_enum);


#endif // __COMMON_H__