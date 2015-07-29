#ifndef PINS_H
#define PINS_H
//=============================================================================
#include <avr/io.h>
//=============================================================================
#define CONCAT(x,y)			x ## y

#define DDR(x)				CONCAT(DDR,x)
#define PORT(x)				CONCAT(PORT,x)
#define PIN(x)				CONCAT(PIN,x)

// Beeper
#define BEEPER				B
#define BEEPER_LINE			(1 << 4)

// Keyboard
#define BUTTON_LEFT			B
#define BUTTON_LEFT_LINE	5
#define BUTTON_SET			C
#define BUTTON_SET_LINE		0
#define BUTTON_RIGHT		C
#define BUTTON_RIGHT_LINE	1
#define BUTTON_DELAY	    100
#define BUTTON_SCAN_PERIOD  20

// LED digits
#define DIGIT_1				B
#define DIGIT_1_LINE		(1 << 1)
#define DIGIT_2				B
#define DIGIT_2_LINE		(1 << 0)
#define DIGIT_3				B
#define DIGIT_3_LINE		(1 << 3)
#define DIGIT_4				B
#define DIGIT_4_LINE		(1 << 2)

#define SEGMENT_PORT		D
#define COMMON_ANODE		0
#define COMMON_CATODE		1
#define LED_TYPE			COMMON_ANODE
//=============================================================================
#endif /* PINS_H */
