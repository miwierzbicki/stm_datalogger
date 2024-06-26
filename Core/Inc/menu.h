#ifndef __menu_h
#define __menu_h
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
#include "adc.h"

typedef enum {
	MAIN_MENU = 0,
	SENSOR_CONFIG,
	SENSOR_CONFIG_ADC_EXT,
	SENSOR_CONFIG_ADC_INT,
	SENSOR_CONFIG_DS18,
	SD_CONFIG,
	DS18,
	SD_CONFIG_SAVEMODE,
	ONOFF_MEASURE,
	DEBUG_ADC_INT,
	DEBUG_ADC_EXT,
	DEBUG_DS18,
	SENSOR_CONFIG_ADC_EXT0,
	SENSOR_CONFIG_ADC_EXT1,
	SENSOR_CONFIG_ADC_EXT2,
	SENSOR_CONFIG_ADC_EXT3,
	SENSOR_CONFIG_ADC_INT0,
	SENSOR_CONFIG_ADC_INT1,
	SENSOR_CONFIG_ADC_INT2,
	SENSOR_CONFIG_ADC_INT3,
	SENSOR_CONFIG_DS18_1,
	SENSOR_CONFIG_DS18_2,
	SENSOR_CONFIG_DS18_3,
	MAX_SCREEN

}Screen;


void huart_ds_init(UART_HandleTypeDef *huart6);
void sendRtcHandler(RTC_HandleTypeDef *hrtc);
extern volatile bool dataOverwrite;
typedef struct {
	char name[20];
	bool isEnabled;
	uint16_t samplingRate;
	float lastValue;
	bool hasValue;
} Sensors;

extern Sensors sensors[];

typedef struct {
	char sensorName[20];
	float (*function)();
} MapSensors;

void ch1Enable(void);
void ch2Enable(void);
void ch3Enable(void);
void ch4Enable(void);
typedef struct MenuEntry {
   Screen entry;
   const char *entry_string;
} MenuEntry;

typedef struct Menu Menu;

struct Menu {
   void (*function)( Menu *menu, bool ifFirstEntry);
   int entry_count;
   MenuEntry entries[8];
};


void displayMenu(void);
#endif
