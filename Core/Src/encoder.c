#include "menu.h"
#include <stdio.h>
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"
#include "oled.h"
#include <stdbool.h>
#include "encoder.h"



TIM_HandleTypeDef *htim1_new;

void encSetRange(uint16_t min, uint16_t max) {
	__HAL_TIM_SET_AUTORELOAD(htim1_new, max);
}

void encSetPos(uint16_t encPos) {
	__HAL_TIM_SET_COUNTER(htim1_new, encPos);
}

uint32_t encoderGet(void) {
	return __HAL_TIM_GET_COUNTER(htim1_new);
}

void encoderInit(TIM_HandleTypeDef *htim1) {
	htim1_new=htim1;
}

bool entrySelected(int16_t id) {
    if (encoderGet() == id) {
        return true;
    }
    return false;
}

bool entryClicked(int16_t id) {
    if (encoderGet() == id && isClicked()) {
        return true;
    }
    return false;
}

