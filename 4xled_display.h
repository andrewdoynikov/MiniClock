#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H
//=============================================================================
#define SEG_A		7
#define SEG_B		5
#define SEG_C		0
#define SEG_D		2
#define SEG_E		3
#define SEG_F		6
#define SEG_G		1
#define SEG_H		4

#define IND_NUM     4
//=============================================================================
void LED_init(void);
unsigned char LED_convert(unsigned char value);
void LED_write(unsigned char value);
void LED_set_blink(unsigned char value);
void LED_set_blink_pos(unsigned char value, unsigned char pos);
void LED_set_comma(unsigned char value, unsigned char pos);
void LED_set_char(unsigned char c, unsigned char pos);
void LED_set_blink_All(unsigned char value);
void LED_set_comma_All(unsigned char value);
void LED_clear(void);
//=============================================================================
#endif
