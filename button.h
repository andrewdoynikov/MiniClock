#ifndef BUTTON_H
#define BUTTON_H
//=============================================================================
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
//=============================================================================
#include "pins.h"
//=============================================================================
#define BEEP_TIME		20
//=============================================================================
// пользовательские функции 
// инициализация. 
// вызывается в начале программы
void KBD_init(void);
unsigned char debounce_sw_left(void);
unsigned char debounce_sw_set(void);
unsigned char debounce_sw_right(void);
void KBD_beep(void);
//=============================================================================
#endif //BUT_H

