/**
    ******************************************************************************
    * @Company    : Mauro Almeida.
    * @file       : ma_api_interrupt_button.h
    * @author     : Mauro Almeida
    * @version	  : V0.0 
    * @date       : 20/02/2024
    * @brief      : Header file of Interrupt Button
    ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MA_API_INTERRUPT_BUTTON_H
#define __MA_API_INTERRUPT_BUTTON_H

/* Includes ------------------------------------------------------------------*/  
#include <Arduino.h>
#include <stdbool.h>
/* Define --------------------------------------------------------------------*/

/* Typedef -------------------------------------------------------------------*/
typedef struct {
    int pin;
    volatile bool pressed;
} st_button_configs_t;
/* Public objects ------------------------------------------------------------*/

#endif /* __MA_API_INTERRUPT_BUTTON_H */
/*****************************END OF FILE**************************************/