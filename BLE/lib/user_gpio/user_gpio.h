#ifndef USER_GPIO
#define USER_GPIO
#include <Arduino.h>

void user_set_dir(uint8_t gpio_pin, uint8_t mode); 
void user_set_level(uint8_t gpio_pin,uint8_t level);
void user_toggle_pin(uint8_t pin); 
#endif