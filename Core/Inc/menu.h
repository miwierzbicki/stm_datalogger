#ifndef __menu_h
#define __menu_h

typedef enum {
	MAIN_MENU = 0,
	SENSOR_CONFIG,
	SENSOR_CONFIG_ADC_EXT,
	SENSOR_CONFIG_ADC_INT,
	SENSOR_CONFIG_DS18,
	SD_CONFIG,
	SD_CONFIG_SAVEMODE,
	ONOFF_MEASURE,
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
	SENSOR_CONFIG_DS18_3
}Screen;



typedef struct MenuEntry {
   Screen entry;
   const char *entry_string;
} MenuEntry;

typedef struct Menu Menu;

struct Menu {
   void (*function)( Menu *menu);
   int entry_count;
   MenuEntry entries[8];
};

void displayMenu(void);
#endif
