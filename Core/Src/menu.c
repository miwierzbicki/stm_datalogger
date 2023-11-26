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
volatile Screen screen = MAIN_MENU; //instancja enuma Screen, aktualny stan menu
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
	encSetRange(0, 0);
	listAllItemsFromMenu(menu);
	backButton(0, MAIN_MENU, 0);
}

void drawOnoffMeasure(Menu *menu) {
	//listAllItemsFromMenu(menu);
	encSetRange(0, 1);
	float temp = ds18_get_temp();
	char tempStr[10];
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("ds18b20_1 value:", Font_7x10, White);
	ssd1306_SetCursor(0, 12);
	sprintf(tempStr, "%f \n\r", temp);
	ssd1306_WriteString(tempStr, Font_16x24, White);
	HAL_UART_Transmit(huart6_new, tempStr, strlen(tempStr), HAL_MAX_DELAY);
	backButton(1, MAIN_MENU, 1);
}


void drawSensorConfigAdcExt(Menu *menu) {
	encSetRange(0, 4);
	listAllItemsFromMenu(menu);
	backButton(4, MAIN_MENU, 4);
	//if przycisk wcisniety
	//przypisanie wartoci z enkodera do zmiennej tej int globalnej
	//ustawienie wartosci enuma globalnego na nastepne menu czyli np SENSOR_CONFIG_ADC_EXT1 lub EXT2 itp.
//	if(enkoder==wcisniety) {
//		sensorAdcExt=wartosc z enkodera
//		screen = SENSOR_CONFIG_ADC_EXT1;
//	}
}
void drawSensorConfigAdcInt(Menu *menu) {
	encSetRange(0, 4);
	listAllItemsFromMenu(menu);
	backButton(4, MAIN_MENU, 4);
}
void drawSensorConfigDS18(Menu *menu) {
	encSetRange(0, 3);
	listAllItemsFromMenu(menu);
	backButton(3, MAIN_MENU, 3);
}

typedef struct {
	char name[20];
	bool isEnabled;
} Sensors;

volatile Sensors sensors[] = {

		{"ADC EXT CH0", false}, //0
		{"ADC EXT CH1", false}, //1
		{"ADC EXT CH2", false}, //2
		{"ADC EXT CH3", false},
		{"ADC INT CH0", false},
		{"ADC INT CH1", false}, //5
		{"ADC INT CH2", false},
		{"ADC INT CH3", false}, //7
		{"DS18B20 #1", false}, //8
		{"DS18B20 #2", false}, //9
		{"DS18B20 #3", false} //10
};

void drawInterruptTimes() {

}

int samplingRates[] = {100,500,1000,5000};
volatile int sampling1;
volatile int samplingIndex = 0;
volatile int counter=0;
char sampl1str[10]="???";
void drawSensorConfigGeneric(Menu *menu) {
	encSetRange(0, 2);
	ssd1306_SetCursor(0, 0);
	if(screen==SENSOR_CONFIG_ADC_EXT0) {
		ssd1306_WriteString(sensors[0].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];

		if(entrySelected(0) && entryClicked(0)) {
			if(sensors[0].isEnabled==false) {
				sensors[0].isEnabled=true;
			}
			else {
				sensors[0].isEnabled=false;
			}
		}
		sprintf(isEnabledStr, "enabled: %s", sensors[0].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, entrySelected(0) ? Black : White);
		ssd1306_SetCursor(0, 28);
//		char tempStrcat[15];
		ssd1306_WriteString("period [ms]: ", Font_7x10, White);
		ssd1306_SetCursor(90, 28);
		ssd1306_WriteString(sampl1str, Font_7x10, entrySelected(1) ? Black : White);
		if(entrySelected(1) && entryClicked(1)) { //tu sie dzieje cos dziwnego
			send_uart("klikniete\n\r");
			counter++;
			if(counter>4) {
				counter=1;
			}
			sampling1 = samplingRates[counter-1];

			sprintf(sampl1str, "%d\n\r", sampling1);
			send_uart(sampl1str);
//			tempStrcat[15] = "period: ";
//			strcat(tempStrcat, sampl1str);


		}

//		if(sampling1==samplingRate[0]) {
//			ssd1306_WriteString(, Font_7x10, entrySelected(1) ? Black : White);
//		}
	}
	else if(screen==SENSOR_CONFIG_ADC_EXT1) {
		ssd1306_WriteString(sensors[1].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[1].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);

	}
	else if(screen==SENSOR_CONFIG_ADC_EXT2) {
		ssd1306_WriteString(sensors[2].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[2].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_ADC_EXT3) {
		ssd1306_WriteString(sensors[3].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[3].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_ADC_INT0) {
		ssd1306_WriteString(sensors[4].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[4].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_ADC_INT1) {
		ssd1306_WriteString(sensors[5].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[5].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_ADC_INT2) {
		ssd1306_WriteString(sensors[6].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[6].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_ADC_INT3) {
		ssd1306_WriteString(sensors[7].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[7].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_DS18_1) {
		ssd1306_WriteString(sensors[8].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[8].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_DS18_2) {
		ssd1306_WriteString(sensors[9].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[9].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else if(screen==SENSOR_CONFIG_DS18_3) {
		ssd1306_WriteString(sensors[10].name, Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		char isEnabledStr[20];
		sprintf(isEnabledStr, "enabled: %s", sensors[10].isEnabled ? "true" : "false");
		ssd1306_WriteString(isEnabledStr, Font_7x10, White);
	}
	else {

	}


	ssd1306_WriteString("", Font_7x10, White);

	backButton(2, MAIN_MENU, 2);
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
