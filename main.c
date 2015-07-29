#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
//=============================================================================
#include "i2c.h"
#include "ds3231.h"
#include "rtos.h"
#include "events.h"
#include "interface.h"
#include "pins.h"
#include "4xled_display.h"
#include "button.h"
//=============================================================================
#define DEBUG		0
//=============================================================================
int main(void)
{
  I2C_init();
  LED_init();
  KBD_init();
  RTC_init();
  RTOS_init();
  RTOS_setTaskFunc(set_blink, 0, 500);       // моргание
  RTOS_setTaskFunc(KBD_scan, 10, 0);         // запускаем опрос кнопок
  sei(); 				        			 // Разрешили прерывания
  LED_clear();
#if (DEBUG == 1)
  RTC_set_time(23, 58, 50); // Test time
  LED_clear();
#endif
  while (1)
  {
    RTOS_dispatchTask();    			     // Вызываем диспетчер в цикле.    
  }
}
//=============================================================================
