#ifndef __encoder_h
#define __encoder_h

#include "menu.h"
#include "main.h"
#include <stdio.h>
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"
#include "oled.h"
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

bool entryClicked(int16_t id);
void encoderInit(TIM_HandleTypeDef *htim1);
void encSetRange(uint16_t min, uint16_t max);
void encSetPos(uint16_t encPos);
uint32_t encoderGet(void);
bool entrySelected(int16_t id);
#endif
