#include <avr/io.h>
#include "ds3231.h"
#include "i2c.h"
//=============================================================================
uint8_t dec2bcd(uint8_t d)
{
  return ((d / 10 * 16) + (d % 10));
}
//=============================================================================
uint8_t bcd2dec(uint8_t b)
{
  return ((b / 16 * 10) + (b % 16));
}
//=============================================================================
uint8_t bcd2bin(uint8_t b)
{
  return ((b / 16 * 10) + (b % 16));
}
//=============================================================================
// Чтение байта относительно 0 регистра
//=============================================================================
uint8_t RTC_read_byte(uint8_t offset)
{
  uint8_t ret;
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(offset);
  I2C_stop();
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_R);
  ret = I2C_read(0);
  I2C_stop();
  return ret;
}
//=============================================================================
// Запись байта относительно 0 регистра
//=============================================================================
void RTC_write_byte(uint8_t b, uint8_t offset)
{
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(offset);
  I2C_write(b);
  I2C_stop();
}
//=============================================================================
// Инициализация модуля RTC DS3231
//=============================================================================
void RTC_init(void)
{
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(0x0E);
  I2C_write(0x20);
  I2C_write(0x08);
  I2C_stop();
  RTC_write_byte(0x81, 0x0A); // срабатывание каждый день
  uint8_t val = RTC_read_byte(0x0E);
  val |= ((1 << 2) | (1 << 0));
  RTC_write_byte(val, 0x0E);
}

//=============================================================================
// Чтение времени
//=============================================================================
void RTC_get_time(uint8_t* hour, uint8_t* min, uint8_t* sec)
{
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(0);
  I2C_stop();
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_R);
  *sec = bcd2dec(I2C_read(1)); 
  *min = bcd2dec(I2C_read(1)); 
  *hour = bcd2dec(I2C_read(0)); 
  I2C_stop();
}
//=============================================================================
// Установка времени
//=============================================================================
void RTC_set_time(uint8_t hour, uint8_t min, uint8_t sec)
{
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(0);
  I2C_write(dec2bcd(sec)); // seconds
  I2C_write(dec2bcd(min)); // minutes
  I2C_write(dec2bcd(hour)); // hours
  I2C_stop();
}
//=============================================================================
// Чтение даты
//=============================================================================
void RTC_get_date(uint8_t* wday, uint8_t* day, uint8_t* mes, uint8_t* year)
{
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(0x03);
  I2C_stop();
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_R);
  *wday = bcd2dec(I2C_read(1)); 
  *day = bcd2dec(I2C_read(1)); 
  *mes = bcd2dec(I2C_read(1)); 
  *year = bcd2dec(I2C_read(0)); 
  I2C_stop();
}
//=============================================================================
// Установка даты
//=============================================================================
void RTC_set_date(uint8_t wday, uint8_t day, uint8_t mes, uint8_t year)
{
  uint8_t dw = RTC_day_of_week(day, mes, year);
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(0x03);
  I2C_write(dec2bcd(dw + 1));
  I2C_write(dec2bcd(day));
  I2C_write(dec2bcd(mes));
  I2C_write(dec2bcd(year));
  I2C_stop();
}
//=============================================================================
// Чтение температуры
//=============================================================================
float RTC_getTemperatura(void)
{
  uint8_t temp;
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_W);
  I2C_write(0x11);
  I2C_stop();
  I2C_start();
  I2C_write(DS_SLAVE_ADDR_R);
  temp = I2C_read(1); // High MSB
  return (float)(temp) + 0.25 * (I2C_read(0) >> 6);
}
//=============================================================================
uint8_t RTC_day_of_week(uint8_t day, uint8_t month, uint8_t year)
{
  if (month < 3) {
    month += 12;
    year--;
  }
  return (((13 * month + 3) / 5 + day + year + year / 4 - year / 100 + year / 400) % 7);
}
//=============================================================================
