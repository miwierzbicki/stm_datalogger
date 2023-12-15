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


void adc_select_ch(uint32_t channel) { //trzeba wywolywac za kazdym razem przed konwersją adc bo inaczej będzie tylko ch0!!!!!!!!
	//wartości channel = ADC_CHANNEL_0, 1, 2, 3
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = channel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(hadc1_new, &sConfig) != HAL_OK)
	{
	Error_Handler();
	}
}

void getValAdc(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_1);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	float voltage = 3.3f * value / 4096.0f;
	char adcStr[200];
	sprintf(adcStr, "a0=%lu (%.3f V)\n\r", value, voltage);
	send_uart(adcStr);

	adc_select_ch(ADC_CHANNEL_3);
    HAL_ADC_Start(hadc1_new);
    HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
    value = HAL_ADC_GetValue(hadc1_new);
    voltage = 3.3f * value / 4096.0f;
    sprintf(adcStr, "a3=%lu (%.3f V)\n\r", value, voltage);
    send_uart(adcStr);
}



float getValueAdcExtCh0(void) {
	#define ADS1115_ADDRESS 0x48
	unsigned char ADSwrite[6];
	int16_t reading;
	volatile float voltage;
	const float voltageConv = 6.114 / 32768.0;
	ADSwrite[0] = 0x01;
	ADSwrite[1] = 0xC1; //11000001
	ADSwrite[2] = 0x83; //10000011 LSB

	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1, ADSwrite, 3, 100);
	ADSwrite[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1 , ADSwrite, 1 ,100);
	HAL_Delay(20);

	HAL_I2C_Master_Receive(hi2c2_new, ADS1115_ADDRESS <<1, ADSwrite, 2, 100);
	reading = (ADSwrite[0] << 8 | ADSwrite[1] );
	if(reading < 0) {
		reading = 0;
	}
	return voltage = reading * voltageConv;
//	char str[20];
//	sprintf(str, "ext_ch0=%.3f V\n\r", voltage);
//	send_uart(str);

}
float getValueAdcExtCh1(void) {
#define ADS1115_ADDRESS 0x48
	unsigned char ADSwrite[6];
	int16_t reading;
	volatile float voltage;
	const float voltageConv = 6.114 / 32768.0;
	ADSwrite[0] = 0x01;
	ADSwrite[1] =  0xD1; //11000001
	ADSwrite[2] = 0x83; //10000011 LSB

	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1, ADSwrite, 3, 100);
	ADSwrite[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1 , ADSwrite, 1 ,100);
	HAL_Delay(20);

	HAL_I2C_Master_Receive(hi2c2_new, ADS1115_ADDRESS <<1, ADSwrite, 2, 100);
	reading = (ADSwrite[0] << 8 | ADSwrite[1] );
	if(reading < 0) {
		reading = 0;
	}
	return voltage = reading * voltageConv;
}
float getValueAdcExtCh2(void) {
#define ADS1115_ADDRESS 0x48
	unsigned char ADSwrite[6];
	int16_t reading;
	volatile float voltage;
	const float voltageConv = 6.114 / 32768.0;
	ADSwrite[0] = 0x01;
	ADSwrite[1] = 0xE1; //11000001
	ADSwrite[2] = 0x83; //10000011 LSB

	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1, ADSwrite, 3, 100);
	ADSwrite[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1 , ADSwrite, 1 ,100);
	HAL_Delay(20);

	HAL_I2C_Master_Receive(hi2c2_new, ADS1115_ADDRESS <<1, ADSwrite, 2, 100);
	reading = (ADSwrite[0] << 8 | ADSwrite[1] );
	if(reading < 0) {
		reading = 0;
	}
	return voltage = reading * voltageConv;
}
float getValueAdcExtCh3(void) {
#define ADS1115_ADDRESS 0x48
	unsigned char ADSwrite[6];
	int16_t reading;
	volatile float voltage;
	const float voltageConv = 6.114 / 32768.0;
	ADSwrite[0] = 0x01;
	ADSwrite[1] = 0xF1; //11000001
	ADSwrite[2] = 0x83; //10000011 LSB

	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1, ADSwrite, 3, 100);
	ADSwrite[0] = 0x00;
	HAL_I2C_Master_Transmit(hi2c2_new, ADS1115_ADDRESS << 1 , ADSwrite, 1 ,100);
	HAL_Delay(20);

	HAL_I2C_Master_Receive(hi2c2_new, ADS1115_ADDRESS <<1, ADSwrite, 2, 100);
	reading = (ADSwrite[0] << 8 | ADSwrite[1] );
	if(reading < 0) {
		reading = 0;
	}
	return voltage = reading * voltageConv;
}
float getValueAdcIntCh0(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_0);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	float voltage = 3.3f * value / 4096.0f;
	//sprintf(adcStr, "a3=%lu (%.3f V)\n\r", value, voltage);
	return voltage;
	//send_uart(adcStr);
}
float getValueAdcIntCh1(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_1);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	float voltage = 3.3f * value / 4096.0f;
	//sprintf(adcStr, "a3=%lu (%.3f V)\n\r", value, voltage);
	return voltage;
	//send_uart(adcStr);
}
float getValueAdcIntCh2(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_2);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	float voltage = 3.3f * value / 4096.0f;
	//sprintf(adcStr, "a3=%lu (%.3f V)\n\r", value, voltage);
	return voltage;
	//send_uart(adcStr);
}
float getValueAdcIntCh3(void) {
	uint32_t value;
	adc_select_ch(ADC_CHANNEL_3);
	HAL_ADC_Start(hadc1_new);
	HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
	value = HAL_ADC_GetValue(hadc1_new);
	float voltage = 3.3f * value / 4096.0f;
	//sprintf(adcStr, "a3=%lu (%.3f V)\n\r", value, voltage);
	return voltage;
	//send_uart(adcStr);
}
