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

const uint8_t ds1addr[] = {0x28, 0x0, 0x2f, 0xfd, 0x5, 0x0, 0x0, 0x50};
const uint8_t ds2addr[] = {0x28, 0xc3, 0xa5, 0xfd, 0x5, 0x0, 0x0, 0xe0};
const uint8_t ds3addr[];


float getValueDs1(void) {
		oneWireReset();
		oneWireWrite(0x55);

		for(int i=0; i<8; i++) {
			oneWireWrite(ds1addr[i]);
		}
		oneWireWrite(0x44);
		HAL_Delay(95);
		oneWireReset();
		oneWireWrite(0x55);
		for(int i=0; i<8; i++) {
			oneWireWrite(ds1addr[i]);
		}
		oneWireWrite(0xbe);
		int i;
		uint8_t rom_code[9];
		for (i = 0; i < 9; i++)
		  rom_code[i] = oneWireRead();
		float temp= ((rom_code[1]<<8) | (rom_code[0]));

		//send_uart_float(temp);
		return temp = temp/16.0f;
}

float getValueDs2(void) {
	oneWireReset();
	oneWireWrite(0x55);

	for(int i=0; i<8; i++) {
		oneWireWrite(ds2addr[i]);
	}
	oneWireWrite(0x44);
	HAL_Delay(95);
	oneWireReset();
	oneWireWrite(0x55);
	for(int i=0; i<8; i++) {
		oneWireWrite(ds2addr[i]);
	}
	oneWireWrite(0xbe);
	int i;
	uint8_t rom_code[9];
	for (i = 0; i < 9; i++)
	  rom_code[i] = oneWireRead();
	float temp= ((rom_code[1]<<8) | (rom_code[0]));

	//send_uart_float(temp);
	return temp = temp/16.0f;

}

float getValueDs3(void) {
	return 0.0;
}


void ds18_init(TIM_HandleTypeDef *htim10) {
	htim10_new=htim10;
}


void delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(htim10_new, 0);
	  while (__HAL_TIM_GET_COUNTER(htim10_new) < us) {}
}

HAL_StatusTypeDef oneWireReset(void)
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

void bitWrite(int value)
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


int bitRead(void)
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

void oneWireWrite(uint8_t byte)
{
  int i;
  for (i = 0; i < 8; i++) {
    bitWrite(byte & 0x01);
    byte >>= 1;
  }
}

uint8_t oneWireRead(void)
{
  uint8_t value = 0;
  int i;
  for (i = 0; i < 8; i++) {
    value >>= 1;
    if (bitRead())
      value |= 0x80;
  }
  return value;
}
