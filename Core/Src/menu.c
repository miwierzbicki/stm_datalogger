#include "menu.h"
#include <stdio.h>
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"
#include "oled.h"
#include "encoder.h"
#include "ds18b20.h"
#include "sd.h"
#include "main.h"

uint16_t encoderMin;
uint16_t encoderMax;
volatile Screen screen = MAIN_MENU; //instancja enuma Screen, aktualny stan menu
int sensorAdcExt=0;
UART_HandleTypeDef *huart6_new;
RTC_HandleTypeDef *hrtc_new;

void huart_ds_init(UART_HandleTypeDef *huart6) {
	huart6_new=huart6;
}

void sendRtcHandler(RTC_HandleTypeDef *hrtc) {
	hrtc_new=hrtc;
}

void backButton(uint8_t back_pos, uint8_t target_screen, uint16_t encoder_pos) { //usunac encoder_pos bo nieuzywane
	ssd1306_SetCursor(0, 56);
	ssd1306_WriteString("COFNIJ", Font_6x8, entrySelected(back_pos) ? Black : White);
	if(entryClicked(back_pos)) {
		encSetPos(0);
		//confirm=true;
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

volatile bool dataOverwrite=false;
char strDataOverwrite[6];
void drawSdConfig(Menu *menu) {
	encSetRange(0, 1);
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("SD status:", Font_7x10, White);
	ssd1306_SetCursor(70, 0);
	if(!sdReady) {
		ssd1306_WriteString(" error", Font_7x10, White);
	}
	else {
		ssd1306_WriteString(" OK", Font_7x10, White);
	}
	ssd1306_SetCursor(0, 10);
	ssd1306_WriteString("Overwrite: ", Font_7x10, White);
	if(entrySelected(0) && entryClicked(0)) {
				if(dataOverwrite==false) {
					dataOverwrite=true;
				}
				else {
					dataOverwrite=false;
				}
	}
	ssd1306_SetCursor(75, 10);
	sprintf(strDataOverwrite, "%s", dataOverwrite ? "true" : "false");
	ssd1306_WriteString(strDataOverwrite, Font_7x10, entrySelected(0) ? Black : White);
	backButton(1, MAIN_MENU, 0);
}



void drawSensorConfigAdcExt(Menu *menu) {
	encSetRange(0, 4);
	listAllItemsFromMenu(menu);
	backButton(4, MAIN_MENU, 4);
}
void drawSensorConfigAdcInt(Menu *menu) {
	encSetRange(0, 4);
	listAllItemsFromMenu(menu);
	backButton(4, MAIN_MENU, 4);
}
void drawSensorConfigDS18(Menu *menu) {
	encSetRange(0, 1);
	listAllItemsFromMenu(menu);
	backButton(3, MAIN_MENU, 3);
}



Sensors sensors[] = {
		{"ADC EXT CH0", false, 0},
		{"ADC EXT CH1", false, 0},
		{"ADC EXT CH2", false, 0},
		{"ADC EXT CH3", false, 0},
		{"ADC INT CH0", false, 0},
		{"ADC INT CH1", false, 0},
		{"ADC INT CH2", false, 0},
		{"ADC INT CH3", false, 0},
		{"DS18B20 #1", false, 0},
		{"DS18B20 #2", false, 0},
		{"DS18B20 #3", false, 0}
};

uint16_t samplingRates[] = {10,50,100,500};
volatile uint8_t  samplingIndex = 0;
volatile int counter=0;
char sampl1str[10]="???";

void drawSensorOptions(uint8_t index) {
	ssd1306_WriteString(sensors[index].name, Font_11x18, White);
	ssd1306_SetCursor(0, 18);
	char isEnabledStr[20];
	if(entrySelected(0) && entryClicked(0)) {
		if(sensors[index].isEnabled==false) {
			sensors[index].isEnabled=true;
		}
		else {
			sensors[index].isEnabled=false;
		}
	}
	sprintf(isEnabledStr, "enabled: %s", sensors[index].isEnabled ? "true" : "false");
	ssd1306_WriteString(isEnabledStr, Font_7x10, entrySelected(0) ? Black : White);
	ssd1306_SetCursor(0, 28);
	ssd1306_WriteString("period [ms]: ", Font_7x10, White);
	ssd1306_SetCursor(90, 28);
	sprintf(sampl1str, "%d\n\r", sensors[index].samplingRate);
	ssd1306_WriteString(sampl1str, Font_7x10, entrySelected(1) ? Black : White);
	if(entrySelected(1) && entryClicked(1)) { //tu sie dzieje cos dziwnego
		//send_uart("klikniete\n\r");
		counter++;
		if(counter>4) {
			counter=1;
		}
		sensors[index].samplingRate = samplingRates[counter-1];
	}

}


void drawSensorConfigGeneric(Menu *menu) {
	encSetRange(0, 2);
	ssd1306_SetCursor(0, 0);
	drawSensorOptions(screen-SENSOR_CONFIG_ADC_EXT0);
	backButton(2, MAIN_MENU, 2);
}


void ch1Enable(void) {
	for(int i=0; i<10; i++) {
		if(sensors[i].samplingRate==10 && sensors[i].isEnabled) {
		  send_uart("10ms\n\r");
		}
	}
}

void ch2Enable(void) {
	for(int i=0; i<10; i++) {
		if(sensors[i].samplingRate==50 && sensors[i].isEnabled) {
		  send_uart("50ms\n\r");
		}
	}
}

void ch3Enable(void) {
	for(int i=0; i<10; i++) {
		if(sensors[i].samplingRate==100 && sensors[i].isEnabled) {
		  send_uart("100ms\n\r");
		}
	}
}

void ch4Enable(void) {
	for(int i=0; i<10; i++) {
		if(sensors[i].samplingRate==500 && sensors[i].isEnabled) {
		  send_uart("500ms\n\r");
		}
	}
}





void drawOnoffMeasure(Menu *menu) {
	//listAllItemsFromMenu(menu);
//	encSetRange(0, 1);
//	float temp = ds18_get_temp();
//	char tempStr[10];
//	ssd1306_SetCursor(0, 0);
//	ssd1306_WriteString("ds18b20_1 value:", Font_7x10, White);
//	ssd1306_SetCursor(0, 12);
//	sprintf(tempStr, "%.2f \n\r", temp);
//	ssd1306_WriteString(tempStr, Font_16x24, White);
//	HAL_UART_Transmit(huart6_new, tempStr, strlen(tempStr), HAL_MAX_DELAY);
	ssd1306_SetCursor(0, 0);
	uint8_t currPos=0;
	char sensorDetailsStr[30];
	for(int i=0; i<11; i++) {
		if(sensors[i].isEnabled) {
			sprintf(sensorDetailsStr, "%s: %d\n\r", sensors[i].name, sensors[i].samplingRate);
			ssd1306_SetCursor(0, currPos+8);
			currPos=currPos+8;
			ssd1306_WriteString(sensorDetailsStr, Font_6x8, White); //za mało miejsca na ekranie -> (???)
		}
	}

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	HAL_RTC_GetTime(hrtc_new, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(hrtc_new, &date, RTC_FORMAT_BIN);
	char rtcTimeStr[50];
	char rtcDateStr[50];

	sprintf(rtcTimeStr, "%02d:%02d:%02d", time.Hours, time.Minutes, time.Seconds);
	sprintf(rtcDateStr, "%02d/%02d/%02d", date.Date, date.Month, date.Year);
	ssd1306_SetCursor(0, 16);
	//ssd1306_WriteString(rtcTimeStr, Font_7x10, White);
	ssd1306_SetCursor(0, 26);
	//ssd1306_WriteString(rtcDateStr, Font_7x10, White);

	backButton(1, MAIN_MENU, 1);

}


Menu menu[] = {
	[MAIN_MENU]={drawMainMenu, 3,
			{{SENSOR_CONFIG, "Konfig. czuj."},
			{SD_CONFIG, "SD konfig."},
			{ONOFF_MEASURE, "Start pomiaru"}
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
