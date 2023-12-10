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

void adc_int_init(ADC_HandleTypeDef *hadc1) {
	hadc1_new=hadc1;
}

void adc_select_ch(uint32_t channel) { //trzeba wywolywac za kazdym razem przed konwersją adc!!!!!!!!
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
//  HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
//  value[1] = HAL_ADC_GetValue(hadc1_new);
//
//  HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
//  value[2] = HAL_ADC_GetValue(hadc1_new);
//
//  HAL_ADC_PollForConversion(hadc1_new, HAL_MAX_DELAY);
//  value[3] = HAL_ADC_GetValue(hadc1_new);
//  HAL_ADC_Stop(hadc1_new);
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
