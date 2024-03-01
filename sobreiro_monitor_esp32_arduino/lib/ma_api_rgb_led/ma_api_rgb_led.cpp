/**
  ******************************************************************************
  * @Company    : Mauro Almeida.
  * @file       : ma_api_rgb_led.c
  * @author     : Mauro Almeida
  * @version	  : V0.0 
  * @date       : 20/02/2024
  * @brief      : API of RGB LED
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/ 
// C language standard library

// Mauro Almeida driver library

// API library
#include "ma_api_rgb_led.h"

/*******************************************************************************
							HOW TO USE THIS API
********************************************************************************

1. 	First, you should include in your .c file the 
    "ma_api_rgb_led.h" file.

2.  Call ma_api_rgb_led_init(DESIRED-PIN-RED, DESIRED-PIN-GREN, DESIRED-PIN-BLUE) 
    to initialize and setup the RGB Led parameters.

3.  Call ma_api_rgb_led_on(DESIRED-COLOR) to turn RGB Led in a Desired color.

4.  Call ma_api_rgb_led_off() to turn off RGB Led.

5.  Call ma_api_rgb_led_blink(DESIRED-COLOR, TIME-ON, TIME-OFF, DESIRED-BLINK-QTY)
    to Blink RGB Led.

*******************************************************************************/

/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

typedef struct {
  uint8_t redPin;
  uint8_t greenPin;
  uint8_t bluePin;
} st_rgb_led_configs_t;

/* Private variables ---------------------------------------------------------*/
st_rgb_led_configs_t stRgbLedConfigs = {0};

/* Private function prototypes -----------------------------------------------*/  

/* Public objects ------------------------------------------------------------*/

/* Body of private functions -------------------------------------------------*/

/**
  * @Func       : ma_api_rgb_led_init
  * @brief      : Initialize RGB LED pins
  * @pre-cond.  : None
  * @post-cond. : RGB LED pins initialized
  * @parameters : in_redPin - Pin number for the red LED
  *               in_greenPin - Pin number for the green LED
  *               in_bluePin - Pin number for the blue LED
  * @retval     : 0 = Success && -1 == Invalid Pin
  */
int8_t ma_api_rgb_led_init(uint8_t in_redPin, uint8_t in_greenPin, uint8_t in_bluePin) 
{
  if (in_redPin < 0 || in_greenPin < 0 || in_bluePin < 0) {
    return -1;
  }

  stRgbLedConfigs.redPin = in_redPin;
  stRgbLedConfigs.greenPin = in_greenPin;
  stRgbLedConfigs.bluePin = in_bluePin;

  pinMode(in_redPin, OUTPUT);
  pinMode(in_greenPin, OUTPUT);
  pinMode(in_bluePin, OUTPUT);

  ma_api_rgb_led_off();

  return 0;
}

/**
  * @Func       : ma_api_rgb_led_on
  * @brief      : Turn on RGB LED with specified color
  * @pre-cond.  : ma_api_rgb_led_init
  * @post-cond. : RGB LED turned on with specified color
  * @parameters : color - Color of the LED in RGB format
  * @retval     : None
  */
void ma_api_rgb_led_on(e_rgb_colors in_color, bool in_flag_enable_leds) 
{
  if(in_flag_enable_leds != true) return;
  analogWrite(stRgbLedConfigs.redPin, (in_color >> 16) & 0xFF);
  analogWrite(stRgbLedConfigs.greenPin, (in_color >> 8) & 0xFF);
  analogWrite(stRgbLedConfigs.bluePin, in_color & 0xFF);
}

/**
  * @Func       : ma_api_rgb_led_off
  * @brief      : Turn off RGB LED
  * @pre-cond.  : ma_api_rgb_led_init
  * @post-cond. : RGB LED turned off
  * @parameters : None
  * @retval     : None
  */
void ma_api_rgb_led_off(void) 
{
  analogWrite(stRgbLedConfigs.redPin, 0);
  analogWrite(stRgbLedConfigs.greenPin, 0);
  analogWrite(stRgbLedConfigs.bluePin, 0);
}

/**
  * @Func       : ma_api_rgb_led_blink
  * @brief      : Blink RGB LED with specified color, timing, and number of blinks
  * @pre-cond.  : ma_api_rgb_led_init
  * @post-cond. : RGB LED blinked with specified color, timing, and number of blinks
  * @parameters : in_color - Color of the LED in RGB format
  *               in_timeHigh - Duration for LED ON state in milliseconds
  *               in_timeLow - Duration for LED OFF state in milliseconds
  *               in_numBlinks - Number of times the LED should blink
  * @retval     : None
  */
void ma_api_rgb_led_blink(e_rgb_colors in_color, unsigned long in_timeHigh, unsigned long in_timeLow, int in_numBlinks, bool in_flag_enable_leds) 
{
  if(in_flag_enable_leds != true) return;
  static unsigned long previousMillis = 0;
  static bool ledState = false;
  static int blinkCount = 0;

  while (blinkCount < in_numBlinks * 2) 
  {
    unsigned long currentMillis = millis();
    if (ledState == false) 
    {
      if (currentMillis - previousMillis >= in_timeLow) 
      {
        previousMillis = currentMillis;
        ledState = true;
        ma_api_rgb_led_on(in_color, in_flag_enable_leds);
      }
    } 
    else
    {
      if (currentMillis - previousMillis >= in_timeHigh) 
      {
        previousMillis = currentMillis;
        ledState = false;
        ma_api_rgb_led_off();
        blinkCount++;
      }
    }
  } 
}

/*****************************END OF FILE**************************************/