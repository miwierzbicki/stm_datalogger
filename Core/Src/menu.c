#include "menu.h"
#include <stdio.h>
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"
#include "oled.h"
#include "encoder.h"
#include "ds18b20.h"

uint16_t encoderMin;
uint16_t encoderMax;
Screen screen = MAIN_MENU; //instancja enuma Screen, aktualny stan menu
int sensorAdcExt=0;
UART_HandleTypeDef *huart6_new;

void huart_ds_init(UART_HandleTypeDef *huart6) {
	huart6_new=huart6;
}

void backButton(uint8_t back_pos, uint8_t target_screen, uint16_t encoder_pos) {
	ssd1306_SetCursor(0, 56);
	ssd1306_WriteString("COFNIJ", Font_6x8, entrySelected(back_pos) ? Black : White);
	if(entryClicked(back_pos)) {
		encSetPos(0);
		screen = target_screen;
	}
}

void listAllItemsFromMenu(Menu *menu) {
	for (int i = 0; i < menu->entry_count; ++i) { //-> bo entry_count na stercie cpu jest (heap)
			ssd1306_SetCursor(0, i*8);
			if(entryClicked(i)) {
				screen = menu->entries[i].entry;
				break;
			}
			ssd1306_WriteString(menu->entries[i].entry_string, Font_6x8, entrySelected(i) ? Black : White);
		  }
}

void drawMainMenu(Menu *menu) {
	encSetRange(0, 39);
	listAllItemsFromMenu(menu);
	uint32_t encVal = encoderGet();
	char charArVal[4];
	sprintf(charArVal, "%lu", encVal);
	ssd1306_SetCursor(0, 30);
	ssd1306_WriteString(charArVal, Font_16x24, White);

}

void drawSensorConfig(Menu *menu) {
	encSetRange(0, 3);
	listAllItemsFromMenu(menu);
	backButton(3, MAIN_MENU, 1);
}

void drawSdConfig(Menu *menu) {
	encSetRange(0, 1);

	listAllItemsFromMenu(menu);
	backButton(0, MAIN_MENU, 0);
}

void drawOnoffMeasure(Menu *menu) {
	//listAllItemsFromMenu(menu);
	encSetRange(0, 1);
	wire_reset();
	wire_write(0xcc);
	wire_write(0x44);
	HAL_Delay(95);
	wire_reset();
	wire_write(0xcc);
	wire_write(0xbe);
	int i;
	uint8_t rom_code[9];
	for (i = 0; i < 9; i++)
	  rom_code[i] = wire_read();
	char znak[20];
	float temp= ((rom_code[1]<<8) | (rom_code[0]));

	char tempStr[10];
	//memcpy(&temp, &rom_code[0], sizeof(temp));
	temp = temp/16.0f;

	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("ds18b20_1 value:", Font_7x10, White);
	ssd1306_SetCursor(0, 12);
	sprintf(tempStr, "%f\n\r", temp);
	ssd1306_WriteString(tempStr, Font_16x24, White);
	HAL_UART_Transmit(huart6_new, tempStr, strlen(tempStr), HAL_MAX_DELAY);
	backButton(1, MAIN_MENU, 1);
}


void drawSensorConfigAdcExt(Menu *menu) {
	listAllItemsFromMenu(menu);
	//if przycisk wcisniety
	//przypisanie wartoci z enkodera do zmiennej tej int globalnej
	//ustawienie wartosci enuma globalnego na nastepne menu czyli np SENSOR_CONFIG_ADC_EXT1 lub EXT2 itp.
//	if(enkoder==wcisniety) {
//		sensorAdcExt=wartosc z enkodera
//		screen = SENSOR_CONFIG_ADC_EXT1;
//	}
}
void drawSensorConfigAdcInt(Menu *menu) {
	listAllItemsFromMenu(menu);
	backButton(1, MAIN_MENU, 1);
}
void drawSensorConfigDS18(Menu *menu) {
	listAllItemsFromMenu(menu);
	backButton(1, MAIN_MENU, 1);
}

void drawSensorConfigGeneric(Menu *menu) {
	//tu wartosc z inta tego globalnego (wartosc z enkodera)
	//na jego podstawie wyprintowac tekst i ustawic jaki czujnik
	//displayWrite("Edit sensor: ");
	listAllItemsFromMenu(menu);
	backButton(1, MAIN_MENU, 1);
}

Menu menu[] = {
	[MAIN_MENU]={drawMainMenu, 3,
			{{SENSOR_CONFIG, "Konfig. czuj."},
			{SD_CONFIG, "SD konfig."},
			{ONOFF_MEASURE, "On/off"}
			}
	},
	[SENSOR_CONFIG]={drawSensorConfig, 3,
			{{SENSOR_CONFIG_ADC_EXT, "ADC EXT"},
			{SENSOR_CONFIG_ADC_INT, "ADC INT"},
			{SENSOR_CONFIG_DS18, "DS18B20"}
			}
	},

	[SENSOR_CONFIG_ADC_EXT] = {drawSensorConfigAdcExt, 4,
			{{SENSOR_CONFIG_ADC_EXT0, "ADC EXT CH0"},
			{SENSOR_CONFIG_ADC_EXT1, "ADC EXT CH1"},
			{SENSOR_CONFIG_ADC_EXT2, "ADC EXT CH2"},
			{SENSOR_CONFIG_ADC_EXT3, "ADC EXT CH3"}
			}
	},
	//do wywolywania menu generycznego z parametrami pomiaru (fs, on/off)
	[SENSOR_CONFIG_ADC_EXT0] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_ADC_EXT1] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_ADC_EXT2] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_ADC_EXT3] = {drawSensorConfigGeneric,0,{}},

	[SENSOR_CONFIG_ADC_INT] = {drawSensorConfigAdcInt, 4,
			{{SENSOR_CONFIG_ADC_INT0, "ADC INT CH0"},
			{SENSOR_CONFIG_ADC_INT1, "ADC INT CH1"},
			{SENSOR_CONFIG_ADC_INT2, "ADC INT CH2"},
			{SENSOR_CONFIG_ADC_INT3, "ADC INT CH3"}
			}
	},
	[SENSOR_CONFIG_ADC_INT0] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_ADC_INT1] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_ADC_INT2] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_ADC_INT3] = {drawSensorConfigGeneric,0,{}},

	[SENSOR_CONFIG_DS18] = {drawSensorConfigDS18, 3,
			{{SENSOR_CONFIG_DS18_1, "DS18 1"},
			{SENSOR_CONFIG_DS18_2, "DS18 2"},
			{SENSOR_CONFIG_DS18_3, "DS18 3"}
			}
	},
	[SENSOR_CONFIG_DS18_1] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_DS18_2] = {drawSensorConfigGeneric,0,{}},
	[SENSOR_CONFIG_DS18_3] = {drawSensorConfigGeneric,0,{}},
	[SD_CONFIG] = {drawSdConfig, 0, {}},
	[ONOFF_MEASURE] = {drawOnoffMeasure, 0, {}}
};

void displayMenu(void) {
	ssd1306_Fill(Black);

	menu[screen].function(&menu[screen]);
	ssd1306_UpdateScreen();
	//clearEncButton();
	HAL_Delay(1);

}
