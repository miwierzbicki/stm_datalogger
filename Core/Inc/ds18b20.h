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

float getValueDs1(void);
float getValueDs2(void);
float getValueDs3(void); //te uzupelnic!
float ds18_get_temp(void);
void delay_us(uint32_t us);
void ds18_init(TIM_HandleTypeDef *htim10);

HAL_StatusTypeDef wire_reset(void);
void write_bit(int value);
int read_bit(void);
void wire_write(uint8_t byte);
uint8_t wire_read(void);
