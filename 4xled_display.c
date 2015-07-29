#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
//=============================================================================
#include "4xled_display.h"
#include "pins.h"
//=============================================================================
#define TIMER0_COUNTER	0xE0
//=============================================================================
unsigned char symvol[] = 
{
  (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // 0 [0]
  (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // 1 [1]
  (1<<SEG_A)|(1<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // 2 [2]
  (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // 3 [3]
  (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // 4 [4]
  (1<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // 5 [5]
  (1<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // 6 [6]
  (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // 7 [7]
  (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // 8 [8]
  (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // 9 [9]
  (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // - [10]
  (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // = [11]
  (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // i [12]  
  (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // t [13] 
  (1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // E [14]
  (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // r [15]
  (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // o [16]
  (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // c [17]
  (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // n [18]  
  (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // d [19]
  (1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // F [20]
  (1<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // A [21]
  (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // b [22]
  (1<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // C [23]
  (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // u [24]
  (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // U [25]
  (0<<SEG_A)|(0<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // h [26]
  (0<<SEG_A)|(1<<SEG_B)|(1<<SEG_C)|(0<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(1<<SEG_G)|(0<<SEG_H), // H [27]
  (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H), // l [28]
  (0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(1<<SEG_D)|(1<<SEG_E)|(1<<SEG_F)|(0<<SEG_G)|(0<<SEG_H)  // L [29]
};
//=============================================================================
#define IND_EMPTY  ((0<<SEG_A)|(0<<SEG_B)|(0<<SEG_C)|(0<<SEG_D)|(0<<SEG_E)|(0<<SEG_F)|(0<<SEG_G)|(0<<SEG_H))
//=============================================================================
#if (LED_TYPE == COMMON_ANODE)
volatile uint8_t ind_buf[IND_NUM] = {0xFF,0xFF,0xFF,0xFF};
#else
volatile uint8_t ind_buf[IND_NUM] = {0x00,0x00,0x00,0x00};
#endif
volatile uint8_t ind_blink[IND_NUM] = {0,0,0,0};
volatile uint8_t ind_comma[IND_NUM] = {0,0,0,0};
volatile uint8_t ind_count = 0;
volatile uint8_t blink = 0;
//=============================================================================
void LED_init(void)
{
  DDR(DIGIT_1) |= DIGIT_1_LINE;
  PORT(DIGIT_1) &= ~DIGIT_1_LINE;
  DDR(DIGIT_2) |= DIGIT_2_LINE;
  PORT(DIGIT_2) &= ~DIGIT_2_LINE;
  DDR(DIGIT_3) |= DIGIT_3_LINE;
  PORT(DIGIT_3) &= ~DIGIT_3_LINE;
  DDR(DIGIT_4) |= DIGIT_4_LINE;
  PORT(DIGIT_4) &= ~DIGIT_4_LINE;
  DDR(SEGMENT_PORT) = 0xFF;
  PORT(SEGMENT_PORT) = 0xFF;

  // Timer0 defines
  TCCR0B |= (1 << CS02)|(1 << CS00);    // прескалер - 1024
  TIFR0 = (1 << TOV0);          		// очищаем флаг прерывания таймера Т0
  TIMSK0 |= (1 << TOIE0);       		// разрешаем прерывание по переполнению
  TCNT0 = TIMER0_COUNTER;          		// загружаем начальное зн. в счетный регистр
  LED_clear();
  LED_set_comma_All(0);
  LED_set_blink_All(0);
}
//=============================================================================
void LED_write(unsigned char value)
{
}
//=============================================================================
ISR(TIMER0_OVF_vect)
{
  unsigned char ch;
//  PORT(DIGIT_1) &= ~DIGIT_1_LINE;       // гасим все разряды
//  PORT(DIGIT_2) &= ~DIGIT_2_LINE;       // гасим все разряды
//  PORT(DIGIT_3) &= ~DIGIT_3_LINE;       // гасим все разряды
//  PORT(DIGIT_4) &= ~DIGIT_4_LINE;       // гасим все разряды
  ch = ind_buf[ind_count];
  if (ind_comma[ind_count] == 1) ch |= (1 << SEG_H);
  if (ind_blink[ind_count] == 1) {
    if (blink == 0) ch = 0x00;
  }
#if (LED_TYPE == COMMON_ANODE)
  ch = ~ch; 
#endif
  PORT(DIGIT_1) &= ~DIGIT_1_LINE;       // гасим все разряды
  PORT(DIGIT_2) &= ~DIGIT_2_LINE;       // гасим все разряды
  PORT(DIGIT_3) &= ~DIGIT_3_LINE;       // гасим все разряды
  PORT(DIGIT_4) &= ~DIGIT_4_LINE;       // гасим все разряды
  PORT(SEGMENT_PORT) = ch;
  switch(ind_count) {
    case 0:
      PORT(DIGIT_1) |= DIGIT_1_LINE;    // включаем разряд
    break;
    case 1:
      PORT(DIGIT_2) |= DIGIT_2_LINE;    // включаем разряд
    break;
    case 2:
      PORT(DIGIT_3) |= DIGIT_3_LINE;    // включаем разряд
    break;
    case 3:
      PORT(DIGIT_4) |= DIGIT_4_LINE;    // включаем разряд
    break;
  }
  ind_count++;
  if (ind_count > 3) ind_count = 0;
  TCNT0 = TIMER0_COUNTER;          		// загружаем начальное зн. в счетный регистр
}
//=============================================================================
unsigned char LED_convert(unsigned char value)
{
  switch(value) {
    case '0':
	  return symvol[0];
    break;
    case '1':
	  return symvol[1];
    break;
    case '2':
	  return symvol[2];
    break;
    case '3':
	  return symvol[3];
    break;
    case '4':
	  return symvol[4];
    break;
    case '5':
	  return symvol[5];
    break;
    case '6':
	  return symvol[6];
    break;
    case '7':
	  return symvol[7];
    break;
    case '8':
	  return symvol[8];
    break;
    case '9':
	  return symvol[9];
    break;
    case '-':
	  return symvol[10];
    break;
    case '=':
	  return symvol[11];
    break;
    case 'I':
    case 'i':
	  return symvol[12];
    break;
    case 'T':
    case 't':
	  return symvol[13];
    break;
    case 'e':
    case 'E':
	  return symvol[14];
    break;
    case 'R':
    case 'r':
	  return symvol[15];
    break;
    case 'O':
    case 'o':
	  return symvol[16];
    break;
    case 'c':
	  return symvol[17];
    break;
    case 'N':
    case 'n':
	  return symvol[18];
    break;
    case 'D':
    case 'd':
	  return symvol[19];
    break;
    case 'f':
    case 'F':
	  return symvol[20];
    break;
    case 'a':
    case 'A':
	  return symvol[21];
    break;
    case 'B':
    case 'b':
	  return symvol[22];
    break;
    case 'C':
	  return symvol[23];
    break;
    case 'u':
	  return symvol[24];
    break;
    case 'U':
	  return symvol[25];
    break;
    case 'h':
	  return symvol[26];
    break;
    case 'H':
	  return symvol[27];
    break;
    case 'l':
	  return symvol[28];
    break;
    case 'L':
	  return symvol[29];
    break;
	default:
	  return IND_EMPTY;
    break;
  }
}
//=============================================================================
void LED_set_blink(unsigned char value)
{
  blink = value;
}
//=============================================================================
void LED_set_char(unsigned char c, unsigned char pos)
{
  if (pos > IND_NUM) return;
  ind_buf[pos - 1] = LED_convert(c);
}
//=============================================================================
void LED_set_comma(unsigned char value, unsigned char pos)
{
  if (pos > IND_NUM) return;
  ind_comma[pos - 1] = value;
}
//=============================================================================
void LED_set_blink_All(unsigned char value)
{
  ind_blink[0] = value;
  ind_blink[1] = value;
  ind_blink[2] = value;
  ind_blink[3] = value;
}
//=============================================================================
void LED_set_comma_All(unsigned char value)
{
  ind_comma[0] = value;
  ind_comma[1] = value;
  ind_comma[2] = value;
  ind_comma[3] = value;
}
//=============================================================================
void LED_clear(void)
{
  ind_buf[0] = IND_EMPTY;
  ind_buf[1] = IND_EMPTY;
  ind_buf[2] = IND_EMPTY;
  ind_buf[3] = IND_EMPTY;
  LED_set_comma_All(0);
  LED_set_blink_All(0);
}
//=============================================================================
void LED_set_blink_pos(unsigned char value, unsigned char pos)
{
  if (pos > IND_NUM) return;
  ind_blink[pos - 1] = value;
}
//=============================================================================
