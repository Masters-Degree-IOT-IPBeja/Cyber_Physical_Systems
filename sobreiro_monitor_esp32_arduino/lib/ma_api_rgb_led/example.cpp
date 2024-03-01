#include <Arduino.h>
#include "ma_api_rgb_led.h"

// Define the pins for RGB LED
#define RED_PIN   5
#define GREEN_PIN 6
#define BLUE_PIN  7

void setup() {
  // Initialize RGB LED
  ma_api_rgb_led_init(RED_PIN, GREEN_PIN, BLUE_PIN);
}

void loop() {
  // Blink the RGB LED in blue color, with 500ms ON and 500ms OFF, for 3 times
  ma_api_rgb_led_blink(BLUE, 500, 500, 3, true);

  // Delay for 2 seconds between each blink cycle
  delay(2000);
}
