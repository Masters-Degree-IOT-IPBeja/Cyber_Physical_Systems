/**
  ******************************************************************************
  * @Company    : Mauro Almeida.
  * @file       : ma_api_interrupt_button.c
  * @author     : Mauro Almeida
  * @version	  : V0.0 
  * @date       : 20/02/2024
  * @brief      : API of Interrupt Button
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/ 
// C language standard library

// Mauro Almeida driver library

// API library
#include "ma_api_interrupt_button.h"

/*******************************************************************************
							HOW TO USE THIS API
********************************************************************************

1. 	First, you should include in your .c file the 
    "ma_api_rain_sensor.h" file.

2.  Call ma_api_rain_sensor_init(DESIRED-PIN) to initialize and setup 
    the sensor parameters.

3.  Call ma_api_rain_sensor_value() to get the sensor raw value.

4.  Call ma_api_rain_sensor_average_percentage_value() to get the sensor 
    average in percentage value.

*******************************************************************************/

/* Private define ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
st_button_configs_t stButtonConfigs = {0};

/* Private function prototypes -----------------------------------------------*/  

/* Public objects ------------------------------------------------------------*/

/* Body of private functions -------------------------------------------------*/
void button_init(int pin, int mode, void (*isr)(void), int trigger) 
{
    stButtonConfigs.pin = pin;
    stButtonConfigs.pressed = false;
    pinMode(stButtonConfigs.pin, mode);
    attachInterrupt(digitalPinToInterrupt(stButtonConfigs.pin), isr, trigger);
}

/*****************************END OF FILE**************************************/