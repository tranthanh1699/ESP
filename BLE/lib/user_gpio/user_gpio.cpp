#include "user_gpio.h"

void user_set_dir(uint8_t gpio_pin, uint8_t mode)
{
    pinMode(gpio_pin, mode);
}

void user_set_level(uint8_t gpio_pin,uint8_t level)
{
    digitalWrite(gpio_pin, level);
}

void user_toggle_pin(uint8_t pin)
{
    digitalWrite(pin, !digitalRead(pin)); 
}