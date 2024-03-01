/**
    ******************************************************************************
    * @Company    : Mauro Almeida.
    * @file       : ma_api_rgb_led.h
    * @author     : Mauro Almeida
    * @version	  : V0.0 
    * @date       : 20/02/2024
    * @brief      : Header file of RGB Led
    ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MA_API_RGB_LED_H
#define __MA_API_RGB_LED_H

/* Includes ------------------------------------------------------------------*/  
#include <Arduino.h>
/* Define --------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/
typedef enum {
  RED = 0xFF0000,
  GREEN = 0x00FF00,
  BLUE = 0x0000FF,
  YELLOW = 0xFFFF00,
  CYAN = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  WHITE = 0xFFFFFF,
  BLACK = 0x000000,
  ORANGE = 0xFFA500,
  PURPLE = 0x800080,
  PINK = 0xFFC0CB,
  LIME = 0x00FF00,
  TEAL = 0x008080,
  INDIGO = 0x4B0082,
} e_rgb_colors;



/* Public objects ------------------------------------------------------------*/
extern int8_t ma_api_rgb_led_init(uint8_t in_redPin, uint8_t in_greenPin, uint8_t in_bluePin); 
extern void ma_api_rgb_led_on(e_rgb_colors in_color, bool in_flag_enable_leds);
extern void ma_api_rgb_led_off(void);
extern void ma_api_rgb_led_blink(e_rgb_colors in_color, unsigned long in_timeHigh, unsigned long in_timeLow, int in_numBlinks, bool in_flag_enable_leds); 


#endif /* __MA_API_RGB_LED_H */
/*****************************END OF FILE**************************************/