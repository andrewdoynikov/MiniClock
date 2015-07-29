#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//=============================================================================
#include "button.h"
#include "pins.h"
//=============================================================================
void KBD_init(void)
{
  DDR(BEEPER) |= BEEPER_LINE;
  PORT(BEEPER) |= BEEPER_LINE;
  DDR(BUTTON_LEFT) &= ~(1 << BUTTON_LEFT_LINE);
  PORT(BUTTON_LEFT) |= (1 << BUTTON_LEFT_LINE);
  DDR(BUTTON_SET) &= ~(1 << BUTTON_SET_LINE);
  PORT(BUTTON_SET) |= (1 << BUTTON_SET_LINE);
  DDR(BUTTON_RIGHT) &= ~(1 << BUTTON_RIGHT_LINE);
  PORT(BUTTON_RIGHT) |= (1 << BUTTON_RIGHT_LINE);
}
//=============================================================================
unsigned char debounce_sw_left(void)
{
  static uint16_t state1 = 0; //holds present state
  state1 = (state1 << 1) | (! bit_is_clear(PIN(BUTTON_LEFT), BUTTON_LEFT_LINE)) | 0xE000;
  if (state1 == 0xF000) return 1;
  if (state1 == 0xE000) return 2;
  return 0;
}
//=============================================================================
unsigned char debounce_sw_set(void)
{
  static uint16_t state2 = 0; //holds present state
  state2 = (state2 << 1) | (! bit_is_clear(PIN(BUTTON_SET), BUTTON_SET_LINE)) | 0xE000;
  if (state2 == 0xF000) return 1;
  if (state2 == 0xE000) return 2;
  return 0;
}
//=============================================================================
unsigned char debounce_sw_right(void)
{
  static uint16_t state3 = 0; //holds present state
  state3 = (state3 << 1) | (! bit_is_clear(PIN(BUTTON_RIGHT), BUTTON_RIGHT_LINE)) | 0xE000;
  if (state3 == 0xF000) return 1;
  if (state3 == 0xE000) return 2;
  return 0;
}
//=============================================================================
void KBD_beep(void)
{
  PORT(BEEPER) &= ~BEEPER_LINE;
  _delay_ms(BEEP_TIME);
  PORT(BEEPER) |= BEEPER_LINE;
}
//=============================================================================



