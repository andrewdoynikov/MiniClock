#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
//=============================================================================
#include "rtos.h"
#include "ds3231.h"
#include "events.h"
#include "button.h"
#include "interface.h"
#include "4xled_display.h"
#include "pins.h"
//=============================================================================
#define DEBUGER		0
//=============================================================================
extern void (*pState)(unsigned char event);
#define SET_STATE(a) pState = a  // макрос для смены состояния
unsigned char blinks = 0;
uint16_t kbd_count = 0;
unsigned char hour = 0, min = 0, sec = 0, n_edit = 0, mode = 0, type = 0;
//=============================================================================
void set_blink(void)
{
  blinks = !blinks;
  LED_set_blink(blinks);
  RTOS_setTask(EVENT_TIMER_SECOND, 0, 0);
}
//=============================================================================
void KBD_scan(void)
{
  unsigned char key = 0;
  if (debounce_sw_set() == 1) {
    key = EVENT_KEY_SET;
  }
  if (debounce_sw_left() >= 1) {
    key = EVENT_KEY_LEFT;
  }
  if (debounce_sw_right() >= 1) {
    key = EVENT_KEY_RIGHT;
  }
  if (key != EVENT_NULL) {
    kbd_count = 0;
    RTOS_setTask(key, 0, 0);
    RTOS_setTaskFunc(KBD_scan, BUTTON_DELAY, 0);          
  } else {
    kbd_count++;
	if (mode == 1) {
	  if (kbd_count == 1000) {
        kbd_count = 0;
	    LED_set_blink_All(0);
		mode = 0;
        SET_STATE(run_main);
	    show_time();
	  }
    } else kbd_count = 0;
    RTOS_setTaskFunc(KBD_scan, BUTTON_SCAN_PERIOD, 0);    
  }
}
//=============================================================================
void run_main(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
      show_time();
    break;
    case EVENT_KEY_SET:
	  KBD_beep();
      RTC_get_time(&hour, &min, &sec);
	  LED_set_blink_pos(1, 1);
	  LED_set_blink_pos(1, 2);
	  n_edit = 0;
	  mode = 1;
	  SET_STATE(run_edit_time);
    break;
    case EVENT_KEY_LEFT:
	  KBD_beep();
	  type = !type;
      show_time();
    break;
    case EVENT_KEY_RIGHT:
	  KBD_beep();
	  type = !type;
      show_time();
    break;
  }
}
//=============================================================================
void run_edit_time(unsigned char event)
{
  switch(event) {
    case EVENT_TIMER_SECOND:
      show_hour(hour);
      show_min(min);
      LED_set_comma(1, 3);
      LED_set_comma(1, 4);
    break;
    case EVENT_KEY_SET:
	  KBD_beep();
	  n_edit++;
	  if (n_edit > 1) {
		LED_set_blink_All(0);
	    n_edit = 0;
        RTC_set_time(hour, min, 0); // Save time
	    show_time();
	    mode = 0;
	    SET_STATE(run_main);
	  } else {
 	    LED_set_blink_pos(0, 1);
	    LED_set_blink_pos(0, 2);
 	    LED_set_blink_pos(1, 3);
	    LED_set_blink_pos(1, 4);
	  }
    break;
    case EVENT_KEY_LEFT:
	  KBD_beep();
	  if (n_edit == 0) {
        if (hour > 0) hour--; else hour = 23;
	    RTOS_deleteTask(EVENT_TIMER_SECOND);
		blinks = 0;
		set_blink();
        show_hour(hour);
	  } else {
        if (min > 0) min--; else min = 59;
	    RTOS_deleteTask(EVENT_TIMER_SECOND);
		blinks = 0;
		set_blink();
        show_min(min);
	  }
    break;
    case EVENT_KEY_RIGHT:
	  KBD_beep();
	  if (n_edit == 0) {
        if (hour < 23) hour++; else hour = 0;
	    RTOS_deleteTask(EVENT_TIMER_SECOND);
		blinks = 0;
		set_blink();
        show_hour(hour);
	  } else {
        if (min < 59) min++; else min = 0;
	    RTOS_deleteTask(EVENT_TIMER_SECOND);
		blinks = 0;
		set_blink();
        show_min(min);
	  }
    break;
  }
}
//=============================================================================
void show_time(void)
{
  unsigned char hour, min, sec;
  RTC_get_time(&hour, &min, &sec);
  if (type == 0) {
    show_hour(hour);
    show_min(min);
  } else {
    show_hour(min);
    show_min(sec);
  }
  if (blinks == 1) {
    LED_set_comma(1, 3);
    LED_set_comma(1, 4);
  } else {
    LED_set_comma(0, 3);
    LED_set_comma(0, 4);
  }
  if ((min == 0) && (sec == 0)) { KBD_beep(); }
}
//=============================================================================
void show_hour(unsigned char value)
{
  uint8_t s[2], i;
  for (i = 0; i < 2; i++) {
    s[2 - i - 1] = '0' + (value % 10);
    value /= 10;
  }
  LED_set_char(s[0],1);
  LED_set_char(s[1],2);
}
//=============================================================================
void show_min(unsigned char value)
{
  uint8_t s[2], i;
  for (i = 0; i < 2; i++) {
    s[2 - i - 1] = '0' + (value % 10);
    value /= 10;
  }
  LED_set_char(s[0],3);
  LED_set_char(s[1],4);
}
//=============================================================================
//=============================================================================
