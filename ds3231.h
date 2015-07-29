//=============================================================================
#ifndef DS3231_H
#define DS3231_H	          1
//=============================================================================
#include <avr/io.h>
//=============================================================================
#define DS_SLAVE_ADDR_W 	0xD0   // I2C адрес для записи
#define DS_SLAVE_ADDR_R 	0xD1   // I2C адрес для чтения
#define DS_PORT_SQW			PORTD  // Порт куда подключен вывод SQW модуля
#define DS_DDR_SQW			DDRD   // Порт куда подключен вывод SQW модуля
#define DS_PIN_SQW			2      // Пин куда подключен вывод SQW модуля
#define DS_PIN_PORT			PIND
//=============================================================================
// functions
//=============================================================================
uint8_t dec2bcd(uint8_t d);
uint8_t bcd2dec(uint8_t b);
uint8_t bcd2bin(uint8_t b);
//=============================================================================
void RTC_init(void);
void RTC_get_time(uint8_t* hour, uint8_t* min, uint8_t* sec);
void RTC_set_time(uint8_t hour, uint8_t min, uint8_t sec);
void RTC_get_date(uint8_t* wday, uint8_t* day, uint8_t* mes, uint8_t* year);
void RTC_set_date(uint8_t wday, uint8_t day, uint8_t mes, uint8_t year);
float RTC_getTemperatura(void);
uint8_t RTC_read_byte(uint8_t offset);
void RTC_write_byte(uint8_t b, uint8_t offset);
uint8_t RTC_day_of_week(uint8_t day, uint8_t month, uint8_t year);
//=============================================================================
#endif
