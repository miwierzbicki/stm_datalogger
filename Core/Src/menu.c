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
#include "save.h"
#include "adc.h"

uint16_t encoderMin;
uint16_t encoderMax;
volatile Screen screen = MAIN_MENU;
int sensorAdcExt=0;
UART_HandleTypeDef *huart6_new;
RTC_HandleTypeDef *hrtc_new;
bool sdUnmounted;

void huart_ds_init(UART_HandleTypeDef *huart6) {
	huart6_new=huart6;
}

void sendRtcHandler(RTC_HandleTypeDef *hrtc) {
	hrtc_new=hrtc;
}

void backButton(uint8_t back_pos, uint8_t target_screen) {
	ssd1306_SetCursor(0, 56);
	ssd1306_WriteString("COFNIJ", Font_6x8, entrySelected(back_pos) ? Black : White);
	if(entryClicked(back_pos)) {
		encSetPos(0);
		screen = target_screen;
	}
}

void listAllItemsFromMenu(Menu *menu) {
	for (int i = 0; i < menu->entry_count; ++i) {
			ssd1306_SetCursor(0, i*10);
			if(entryClicked(i)) {
				screen = menu->entries[i].entry;
				break;
			}
			ssd1306_WriteString(menu->entries[i].entry_string, Font_7x10, entrySelected(i) ? Black : White);
		  }
}

void drawMainMenu(Menu *menu) {
	encSetRange(0, 4);
	listAllItemsFromMenu(menu);
	//ssd1306_SetCursor(0, 50);
	//ssd1306_WriteString(getRtcString(), Font_7x10, White); //do wyswietlenia daty i godziny

}

void drawSensorConfig(Menu *menu) {
	encSetRange(0, 3);
	listAllItemsFromMenu(menu);
	backButton(3, MAIN_MENU);
}


volatile bool dataOverwrite=false;
char strDataOverwrite[6];
char* unmountString="";
char* restartAlertString="";
void drawSdConfig(Menu *menu) {

	encSetRange(0, 3);
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("stan SD:", Font_7x10, White);
	ssd1306_SetCursor(70, 0);
	if(!sdReady) {
		ssd1306_WriteString(" ERROR", Font_7x10, White);
		ssd1306_SetCursor(0, 10);
		ssd1306_WriteString(getFresultString(fresult), Font_7x10, White);
		ssd1306_SetCursor(0, 20);
		ssd1306_WriteString("Start zablokow.", Font_7x10, White);
		leds[2].state=true;

	}
	else {
		ssd1306_WriteString(getFresultString(fresult), Font_7x10, White);
		ssd1306_SetCursor(0, 10);
		ssd1306_WriteString("Nadpisywanie: ", Font_7x10, entrySelected(0) ? Black : White);

		ssd1306_SetCursor(75, 10);
		sprintf(strDataOverwrite, "%s", dataOverwrite ? "true" : "false");
		ssd1306_WriteString(strDataOverwrite, Font_7x10, entrySelected(0) ? Black : White);
		ssd1306_SetCursor(0,20);
		ssd1306_WriteString("Odmontuj SD", Font_7x10, entrySelected(1) ? Black : White);

		if(entrySelected(1) && entryClicked(1)) {
			sd_demount();
			sdUnmounted=true;
			unmountString="                    ";
			unmountString="Odmont. SD OK";
			restartAlertString="                  ";
			restartAlertString="Konieczny RST!";
			leds[2].state=true;
		}
		ssd1306_SetCursor(0,30);
		ssd1306_WriteString(unmountString, Font_7x10, White);
		ssd1306_SetCursor(0,40);
		ssd1306_WriteString(restartAlertString, Font_7x10, White);
	}
	ssd1306_SetCursor(0, 46);
	ssd1306_WriteString("Pobierz adresy DS", Font_7x10, entrySelected(2) ? Black : White);
	backButton(3, MAIN_MENU);
}


void drawSensorConfigAdcExt(Menu *menu) {
	encSetRange(0, 4);
	listAllItemsFromMenu(menu);
	backButton(4, MAIN_MENU);
}
void drawSensorConfigAdcInt(Menu *menu) {
	encSetRange(0, 4);
	listAllItemsFromMenu(menu);
	backButton(4, MAIN_MENU);
}
void drawSensorConfigDS18(Menu *menu) {
	encSetRange(0, 1);
	listAllItemsFromMenu(menu);
	backButton(3, MAIN_MENU);
}


Sensors sensors[] = {
		{"ADC EXT CH0", false, 0, 0.0, false},
		{"ADC EXT CH1", false, 0, 0.0, false},
		{"ADC EXT CH2", false, 0, 0.0, false},
		{"ADC EXT CH3", false, 0, 0.0, false},
		{"ADC INT CH0", false, 0, 0.0, false},
		{"ADC INT CH1", false, 0, 0.0, false},
		{"ADC INT CH2", false, 0, 0.0, false},
		{"ADC INT CH3", false, 0, 0.0, false},
		{"DS18B20 #1", false, 0, 0.0, false},
		{"DS18B20 #2", false, 0, 0.0, false},
		{"DS18B20 #3", false, 0, 0.0, false}
};

MapSensors mapSensors[] = {
		{"ADC EXT CH0", getValueAdcExtCh0},
		{"ADC EXT CH1", getValueAdcExtCh1},
		{"ADC EXT CH2", getValueAdcExtCh2},
		{"ADC EXT CH3", getValueAdcExtCh3},
		{"ADC INT CH0", getValueAdcIntCh0},
		{"ADC INT CH1", getValueAdcIntCh1},
		{"ADC INT CH2", getValueAdcIntCh2},
		{"ADC INT CH3", getValueAdcIntCh3},
		{"DS18B20 #1", getValueDs1},
		{"DS18B20 #2", getValueDs2},
		{"DS18B20 #3", getValueDs3}

};

uint16_t samplingRates[] = {500,1000,2000,4000};
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
	sprintf(isEnabledStr, "Stan: %s", sensors[index].isEnabled ? "ON" : "OFF");
	ssd1306_WriteString(isEnabledStr, Font_7x10, entrySelected(0) ? Black : White);
	ssd1306_SetCursor(0, 28);
	ssd1306_WriteString("Freq [ms]: ", Font_7x10, White);
	ssd1306_SetCursor(70, 28);
	sprintf(sampl1str, "%d\n\r", sensors[index].samplingRate);
	ssd1306_WriteString(sampl1str, Font_7x10, entrySelected(1) ? Black : White);
	if(entrySelected(1) && entryClicked(1)) {
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
	backButton(2, MAIN_MENU);
}
volatile char result[1000]="";
volatile float value;
volatile char temp[50];
void ch1Enable(void) {
	for(int i=0; i<11; i++) {
		if(sensors[i].samplingRate==500 && sensors[i].isEnabled) {
			for(int j=0; j<sizeof(mapSensors)/sizeof(MapSensors); j++) {
				if(strcmp(sensors[i].name, mapSensors[j].sensorName)==0) {
					value = mapSensors[j].function();
					sensors[i].lastValue=value;
					sensors[i].hasValue=true;
				}
			}
		}
		else {
		}
	}
}

void ch2Enable(void) {
	for(int i=0; i<11; i++) {
			if(sensors[i].samplingRate==1000 && sensors[i].isEnabled) {
				for(int j=0; j<sizeof(mapSensors)/sizeof(MapSensors); j++) {
					if(strcmp(sensors[i].name, mapSensors[j].sensorName)==0) {
						value = mapSensors[j].function();
						sensors[i].lastValue=value;
						sensors[i].hasValue=true;
					}
				}
			}
			else {}
		}
}

void ch3Enable(void) {
	for(int i=0; i<11; i++) {
			if(sensors[i].samplingRate==2000 && sensors[i].isEnabled) {
				for(int j=0; j<sizeof(mapSensors)/sizeof(MapSensors); j++) {
					if(strcmp(sensors[i].name, mapSensors[j].sensorName)==0) {
						value = mapSensors[j].function();
						sensors[i].lastValue=value;
						sensors[i].hasValue=true;
					}
				}
			}
			else {}
		}
}

void ch4Enable(void) {
	for(int i=0; i<11; i++) {
			if(sensors[i].samplingRate==4000 && sensors[i].isEnabled) {
				for(int j=0; j<sizeof(mapSensors)/sizeof(MapSensors); j++) {
					if(strcmp(sensors[i].name, mapSensors[j].sensorName)==0) {
						value = mapSensors[j].function();
						sensors[i].lastValue=value;
						sensors[i].hasValue=true;
					}
				}
			}
			else {}
		}
}


void debugAdcInt(Menu *menu) {
	encSetRange(0,1);
	ssd1306_SetCursor(0, 0);
	char str[40];
	sprintf(str, "int ch0=%.3f V", getValueAdcIntCh0());
	ssd1306_WriteString(str, Font_7x10, White);
	ssd1306_SetCursor(0, 10);
	sprintf(str, "int ch1=%.3f V", getValueAdcIntCh1());
	ssd1306_WriteString(str, Font_7x10, White);
	ssd1306_SetCursor(0, 20);
	sprintf(str, "int ch2=%.3f V", getValueAdcIntCh2());
	ssd1306_WriteString(str, Font_7x10, White);
	ssd1306_SetCursor(0, 30);
	sprintf(str, "int ch3=%.3f V", getValueAdcIntCh3());
	ssd1306_WriteString(str, Font_7x10, White);

	ssd1306_SetCursor(0, 50);
	sprintf(str, "ds18_2=%.2f C", getValueDs2());
	ssd1306_WriteString(str, Font_7x10, White);
	backButton(1, MAIN_MENU);
}

void debugAdcExt(Menu *menu) {
	encSetRange(0,1);
	char str[40];
	sprintf(str, "ext ch0=%.3f V", getValueAdcExtCh0());
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString(str, Font_7x10, White);
	ssd1306_SetCursor(0, 10);
	sprintf(str, "ext ch1=%.3f V", getValueAdcExtCh1());
	ssd1306_WriteString(str, Font_7x10, White);
	ssd1306_SetCursor(0, 20);
	sprintf(str, "ext ch2=%.3f V", getValueAdcExtCh2());
	ssd1306_WriteString(str, Font_7x10, White);
	ssd1306_SetCursor(0, 30);
	sprintf(str, "ext ch3=%.3f V", getValueAdcExtCh3());
	ssd1306_WriteString(str, Font_7x10, White);

	backButton(1, MAIN_MENU);
}

void testAllSensors(Menu *menu) {
	int numSensors = sizeof(sensors) / sizeof(sensors[0]);
	    for (int i = 0; i < numSensors; ++i) {
	        sensors[i].isEnabled = true;
	        sensors[i].samplingRate = 10;
	    }
	backButton(1, MAIN_MENU);
}

char *measureStatusStr="";
char* restartDetSdString="";
void drawOnoffMeasure(Menu *menu) {

	uint8_t currPos;
	if(!sdReady) {
		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString("SD ERROR!", Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		ssd1306_WriteString(getFresultString(fresult), Font_7x10, White);
		ssd1306_SetCursor(0, 28);
		ssd1306_WriteString("WYMAG. RESET!", Font_7x10, White);
		leds[2].state=true;
		currPos=0;
	}
	else if(sdUnmounted) {
		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString("WYMAG.", Font_11x18, White);
		ssd1306_SetCursor(0, 18);
		ssd1306_WriteString("RESTART", Font_11x18, White);
		ssd1306_SetCursor(0, 36);
		ssd1306_WriteString("po odmont. SD!", Font_6x8, White);
		leds[2].state=true;
	}
	else {
		leds[2].state=false;
		ssd1306_SetCursor(0, 0);
		currPos=-8;
		//char sensorDetailsStr[30];
		encSetRange(0,2);

		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString("START", Font_11x18, entrySelected(0) ? Black : White);
		ssd1306_SetCursor(0, 18);
		ssd1306_WriteString("STOP", Font_11x18, entrySelected(1) ? Black : White);


		if(entrySelected(0) && entryClicked(0)) {
			if(debug) {
				measureStatusStr="                ";
				measureStatusStr="Juz rozpoczeto!";
			}
			else {
				if(sd_openfile()==FR_OK) {
					if(sd_writeline("<NEW_MEASURE_BEGIN>\n")==FR_OK) {
						sd_writeline("timestamp,adc_ext_ch0,adc_ext_ch1,adc_ext_ch2,adc_ext_ch3,adc_int_ch0,adc_int_ch1,adc_int_ch2,adc_int_ch3,ds18b20_1,ds18b20_2,ds18b20_3\n");
						debug=true;
						measureStatusStr="                ";
						measureStatusStr="Pomiar rozpocz.";
					}
				}
				else {
					measureStatusStr="                ";
					measureStatusStr="Nie wykryt. SD!";
					restartDetSdString="Wymag. RST";
					leds[2].state=true;
				}
			}
		}
		if(entrySelected(1) && entryClicked(1)) {
			debug=false;
			sd_closefile();
			//sd_demount();
			measureStatusStr="                ";
			measureStatusStr="Pomiar zakonczony";
		}
		ssd1306_SetCursor(0, 36);
		ssd1306_WriteString(measureStatusStr, Font_6x8, White);
		ssd1306_SetCursor(0, 46);
		ssd1306_WriteString(restartDetSdString, Font_6x8, White);
	}

	backButton(2, MAIN_MENU);

}

Menu menu[] = {
	[MAIN_MENU]={drawMainMenu, 6,
			{{SENSOR_CONFIG, "Konfig. czuj."},
			{SD_CONFIG, "Konfiguracja"},
			{ONOFF_MEASURE, "Start pomiaru"},
			{DEBUG_ADC_INT, ">ADC INT"},
			{DEBUG_ADC_EXT, ">ADC EXT"},
			{DEBUG_ADC_EXT, ">DS18B20"},
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
	[ONOFF_MEASURE] = {drawOnoffMeasure, 0, {}},
	[DEBUG_ADC_INT] = {debugAdcInt, 0, {}},
	[DEBUG_ADC_EXT] = {debugAdcExt, 0, {}}
};

void displayMenu(void) {
	ssd1306_Fill(Black);
	menu[screen].function(&menu[screen]);
	ssd1306_UpdateScreen();
	HAL_Delay(1);
}
