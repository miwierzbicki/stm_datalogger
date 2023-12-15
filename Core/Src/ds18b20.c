#include <stdio.h>
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"
#include "oled.h"
#include "menu.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "encoder.h"

TIM_HandleTypeDef *htim10_new;



float getValueDs1(void) {
	wire_reset();
		wire_write(0xcc);
		wire_write(0x44);
		HAL_Delay(95);
		wire_reset();
		wire_write(0xcc);
		wire_write(0xbe);
		int i;
		uint8_t rom_code[9];
		for (i = 0; i < 9; i++)
		  rom_code[i] = wire_read();
		float temp= ((rom_code[1]<<8) | (rom_code[0]));

		//send_uart_float(temp);
		return temp = temp/16.0f;
}

float getValueDs2(void) {
	wire_reset();
		wire_write(0xcc);
		wire_write(0x44);
		HAL_Delay(95);
		wire_reset();
		wire_write(0xcc);
		wire_write(0xbe);
		int i;
		uint8_t rom_code[9];
		for (i = 0; i < 9; i++)
		  rom_code[i] = wire_read();
		float temp= ((rom_code[1]<<8) | (rom_code[0]));


		return temp = temp/16.0f;

}

float getValueDs3(void) {
	wire_reset();
		wire_write(0xcc);
		wire_write(0x44);
		HAL_Delay(95);
		wire_reset();
		wire_write(0xcc);
		wire_write(0xbe);
		int i;
		uint8_t rom_code[9];
		for (i = 0; i < 9; i++)
		  rom_code[i] = wire_read();
		float temp= ((rom_code[1]<<8) | (rom_code[0]));


		return temp = temp/16.0f;
}


void ds18_init(TIM_HandleTypeDef *htim10) {
	htim10_new=htim10;
}



float ds18_get_temp(void) {
	wire_reset();
	wire_write(0xcc);
	wire_write(0x44);
	HAL_Delay(95);
	wire_reset();
	wire_write(0xcc);
	wire_write(0xbe);
	int i;
	uint8_t rom_code[9];
	for (i = 0; i < 9; i++)
	  rom_code[i] = wire_read();
	float temp= ((rom_code[1]<<8) | (rom_code[0]));


	return temp = temp/16.0f;
}

void delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(htim10_new, 0);
	  while (__HAL_TIM_GET_COUNTER(htim10_new) < us) {}
}

HAL_StatusTypeDef wire_reset(void)
{
  int rc;

  HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_RESET);
  delay_us(480);
  HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_SET);
  delay_us(70);
  rc = HAL_GPIO_ReadPin(DSO_DATA_GPIO_Port, DSO_DATA_Pin);
  delay_us(410);

  if (rc == 0)
    return HAL_OK;
  else
    return HAL_ERROR;
}

void write_bit(int value)
{
  if (value) {
    HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_RESET);
    delay_us(6);
    HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_SET);
    delay_us(64);
  } else {
    HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_RESET);
    delay_us(60);
    HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_SET);
    delay_us(10);
  }
}


int read_bit(void)
{
  int rc;
  HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_RESET);
  delay_us(6);
  HAL_GPIO_WritePin(DSO_DATA_GPIO_Port, DSO_DATA_Pin, GPIO_PIN_SET);
  delay_us(9);
  rc = HAL_GPIO_ReadPin(DSO_DATA_GPIO_Port, DSO_DATA_Pin);
  delay_us(55);
  return rc;
}

void wire_write(uint8_t byte)
{
  int i;
  for (i = 0; i < 8; i++) {
    write_bit(byte & 0x01);
    byte >>= 1;
  }
}

uint8_t wire_read(void)
{
  uint8_t value = 0;
  int i;
  for (i = 0; i < 8; i++) {
    value >>= 1;
    if (read_bit())
      value |= 0x80;
  }
  return value;
}
