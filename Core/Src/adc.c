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

ADC_HandleTypeDef *hadc1_new;
I2C_HandleTypeDef *hi2c2_new;

void adc_int_init(ADC_HandleTypeDef *hadc1) {
	hadc1_new=hadc1;
}

void adc_ext_init(I2C_HandleTypeDef *hi2c2) {
	hi2c2_new=hi2c2;
}


void adc_select_ch(uint32_t channel) { //trzeba wywolywac za kazdym razem przed konwersją adc bo inaczej będzie tylko ch0!
	//wartości channel -> ADC_CHANNEL_0, 1, 2, 3
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(hadc1_new, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
}


float getValueAdcExtCh0(void) {
	unsigned char dataPacket[6];
	int16_t valueFromRegister;
	dataPacket[0] = 0x01;
	dataPacket[1] = 0xC3;
	dataPacket[2] = 0x83;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 3, HAL_MAX_DELAY);
	dataPacket[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 1 ,HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(hi2c2_new, 0x48 << 1, dataPacket, 2, HAL_MAX_DELAY);
	valueFromRegister = (dataPacket[0] << 8 | dataPacket[1] );
	return valueFromRegister * (4.096/32768.0); //zakres pomiarowy/2^15 (bo 2x8 bitów)
	}

float getValueAdcExtCh1(void) {
	unsigned char dataPacket[6];
	int16_t valueFromRegister;
	dataPacket[0] = 0x01;
	dataPacket[1] = 0xD3;
	dataPacket[2] = 0x83;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 3, HAL_MAX_DELAY);
	dataPacket[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 1 ,HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(hi2c2_new, 0x48 << 1, dataPacket, 2, HAL_MAX_DELAY);
	valueFromRegister = (dataPacket[0] << 8 | dataPacket[1] );
	return valueFromRegister * (4.096/32768.0); //zakres pomiarowy/2^15 (bo 2x8 bitów)
}

float getValueAdcExtCh2(void) {
	unsigned char dataPacket[6];
	int16_t valueFromRegister;
	dataPacket[0] = 0x01;
	dataPacket[1] = 0xE3;
	dataPacket[2] = 0x83;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 3, HAL_MAX_DELAY);
	dataPacket[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 1 ,HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(hi2c2_new, 0x48 << 1, dataPacket, 2, HAL_MAX_DELAY);
	valueFromRegister = (dataPacket[0] << 8 | dataPacket[1] );
	return valueFromRegister * (4.096/32768.0); //zakres pomiarowy/2^15 (bo 2x8 bitów)
}

float getValueAdcExtCh3(void) {
	unsigned char dataPacket[6];
	int16_t valueFromRegister;
	dataPacket[0] = 0x01;
	dataPacket[1] = 0xF3;
	dataPacket[2] = 0x83;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 3, HAL_MAX_DELAY);
	dataPacket[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, 0x48 << 1, dataPacket, 1 ,HAL_MAX_DELAY);
	HAL_Delay(10);
	HAL_I2C_Master_Receive(hi2c2_new, 0x48 << 1, dataPacket, 2, HAL_MAX_DELAY);
	valueFromRegister = (dataPacket[0] << 8 | dataPacket[1] );
	return valueFromRegister * (4.096/32768.0); //zakres pomiarowy/2^15 (bo 2x8 bitów)
}

float getValueAdcIntCh0(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_0);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	return 3.3f * value / 4096.0f;

}
float getValueAdcIntCh1(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_1);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	return 3.3f * value / 4096.0f;

}
float getValueAdcIntCh2(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_2);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	return 3.3f * value / 4096.0f;
}
float getValueAdcIntCh3(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_3);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	return 3.3f * value / 4096.0f;
}
