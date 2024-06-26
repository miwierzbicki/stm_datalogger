#include <stdio.h>
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"
#include "oled.h"
#include "menu.h"
#include <string.h>
#include <stdbool.h>
#include "encoder.h"
#include "ds18b20.h"
#include "string.h"
#include "main.h"
#include "fatfs.h"

//do obslugi fatfs
FATFS fs;
FIL fil;
volatile FRESULT fresult;
UINT br, bw;

bool generalErrorLED = false;
char buffer[128];
bool sdReady = false;


void sd_demount() {
	fresult = f_mount(NULL, "", 0);
	send_uart("<drive unmounted>\n\r");
}

void sd_init() {
	fresult = f_mount(&fs, "/", 1);

	if(fresult==FR_OK) {
		sdReady = true;
		send_uart("SD status: FR_OK\n\r");
		send_uart("<DEVICE READY>\n\r");
	}
	else {
		sdReady = false;
		send_uart("sd status: SD ERR\n\r");
		send_uart("<DEVICE ERROR>\n\r");
		leds[1].state=true;
	}
}


void sd_readfile() {
	if(sdReady) {
	fresult = f_open(&fil, "file1.txt", FA_READ);
		if(fresult==FR_OK) {
			send_uart("<data>: ");
			memset(buffer, 0, sizeof(buffer));
			f_read(&fil, buffer, f_size(&fil), &br);
			send_uart(buffer);
			send_uart("\n\r");
		}
		else {
			send_uart("<sd_readfile: error  opening file>\n\r");
		}
	}
}
volatile BYTE SD_SAVEMODE;
static unsigned int file_number = 1;
FRESULT sd_openfile() {
	char filename[12];
	if(sdReady) {
		sprintf(filename, "%03u.txt", file_number);
		fresult = f_open(&fil, filename, FA_OPEN_APPEND | FA_READ | FA_WRITE);

		if(fresult!=FR_OK) {
			sdReady=false;
			send_uart("\r<cannot open file! sd_openfile>\n\r");

			leds[1].state=true;
		}
		else {
			file_number++;
			send_uart("<file opened>\n\r");
			//sd_writeline("timestamp,adc_ext_ch0,adc_ext_ch1,adc_ext_ch2,adc_ext_ch3,adc_int_ch0,adc_int_ch1,adc_int_ch2,adc_int_ch3,ds18b20_1,ds18b20_2,ds18b20_3\n");
		}
		return fresult;
	}
	else {send_uart("<sdReady returned false! sd_openfile>"); return fresult; leds[1].state=true;}
	return FR_DISK_ERR;
}
static unsigned int line_count = 0;
FRESULT sd_writeline(const char* sdWriteBuff) {
	if(sdReady) {
			//f_puts(sdWriteBuff, &fil);
			if(f_puts(sdWriteBuff, &fil)<0) {
				return FR_DISK_ERR;
				send_uart("f_puts error <0 \n\r");
				leds[1].state=true;
			}
			line_count++;
			if(line_count>=20000) {
				line_count=0;
				sd_closefile();
				sd_openfile();
				sd_writeline("timestamp,adc_ext_ch0,adc_ext_ch1,adc_ext_ch2,adc_ext_ch3,adc_int_ch0,adc_int_ch1,adc_int_ch2,adc_int_ch3,ds18b20_1,ds18b20_2,ds18b20_3\n");
			}

			f_sync(&fil);
			send_uart("\r<line written>\n\r");
			return FR_OK;
		}
	else {send_uart("<sd_writeline: cannot write line>");leds[1].state=true;}
	return FR_DISK_ERR;
}


void sd_closefile(void) {
	if(sdReady) {
		fresult = f_close(&fil);
		send_uart("\r<file closed>\n\r");
	}
}

const char* getFresultString(FRESULT fr)
{
    switch (fr)
    {
        case FR_OK:                   return "FR_OK";
        case FR_DISK_ERR:             return "FR_DISK_ERR";
        case FR_INT_ERR:              return "FR_INT_ERR";
        case FR_NOT_READY:            return "FR_NOT_READY";
        case FR_NO_FILE:              return "FR_NO_FILE";
        case FR_NO_PATH:              return "FR_NO_PATH";
        case FR_INVALID_NAME:         return "FR_INVALID_NAME";
        case FR_DENIED:               return "FR_DENIED";
        case FR_EXIST:                return "FR_EXIST";
        case FR_INVALID_OBJECT:       return "FR_INVALID_OBJECT";
        case FR_WRITE_PROTECTED:      return "FR_WRITE_PROTECTED";
        case FR_INVALID_DRIVE:        return "FR_INVALID_DRIVE";
        case FR_NOT_ENABLED:          return "FR_NOT_ENABLED";
        case FR_NO_FILESYSTEM:        return "FR_NO_FILESYSTEM";
        case FR_MKFS_ABORTED:         return "FR_MKFS_ABORTED";
        case FR_TIMEOUT:              return "FR_TIMEOUT";
        case FR_LOCKED:               return "FR_LOCKED";
        case FR_NOT_ENOUGH_CORE:      return "FR_NOT_ENOUGH_CORE";
        case FR_TOO_MANY_OPEN_FILES:  return "FR_TOO_MANY_OPEN_FILES";
        case FR_INVALID_PARAMETER:    return "FR_INVALID_PARAMETER";
        default:                      return "Unknown Error";
    }
}

void parseAddress(const char *addressString, uint8_t *addressArray) {
    char *token = strtok((char *)addressString, "{}, ");
    int i = 0;
    while (token != NULL) {
        addressArray[i++] = (uint8_t)strtol(token, NULL, 16);
        token = strtok(NULL, "{}, ");
    }
}
#define MAX_FILE_SIZE 70
void readAddressFromFile(const char *filename, uint8_t *addressArray) {
    char fileBuffer[MAX_FILE_SIZE];
    memset(fileBuffer, 0, MAX_FILE_SIZE);
    fresult = f_open(&fil, filename, FA_READ);
    if (fresult == FR_OK) {
        f_read(&fil, fileBuffer, MAX_FILE_SIZE, &br);
        f_sync(&fil);
        f_close(&fil);
        parseAddress(fileBuffer, addressArray);
    }
}

