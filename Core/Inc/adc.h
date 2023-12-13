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

extern ADC_HandleTypeDef *hadc1_new;
float getValueAdcExtCh0(void);
float getValueAdcExtCh1(void);
float getValueAdcExtCh2(void);
float getValueAdcExtCh3(void);
float getValueAdcIntCh0(void);
float getValueAdcIntCh1(void);
float getValueAdcIntCh2(void);
float getValueAdcIntCh3(void);
void adc_int_init(ADC_HandleTypeDef *hadc1);
void getValAdc(void);
