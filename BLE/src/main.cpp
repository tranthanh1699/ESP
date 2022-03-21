#include <Arduino.h>
#include "user_ble.h"
#include "user_gpio.h"

unsigned long lastime; 

void ble_callback_function(String str)
{
  Serial.println("Callback Receiver: " + str); 
}

void setup() {
  Serial.begin(115200);
  bleSetUserCallback((void*)ble_callback_function); 
  bleInit(); 
  lastime = millis(); 
}
void loop() 
{
  static int index = 0; 
  bleHandle(); 
  if(millis() - lastime >= 3000)
  {
    bleSendNotify("Hello From ESP: " + String(index)); 
    index++;
    lastime = millis(); 
  }
}