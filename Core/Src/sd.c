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
FATFS fs;  // file system
FIL fil; // File
FILINFO fno;
FRESULT fresult;  // result
UINT br, bw;  // File read/write count

FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;

char buffer[128];
bool sdReady = false;
// functions
void sd_demount() {
	f_mount(NULL, "", 0);
}

void sd_init() {
	fresult = f_mount(&fs, "/", 1);

	if(fresult==FR_OK) {
		sdReady = true;
		send_uart("sd status: FR_OK\n\r");
	}
	else {
		sdReady = false;
		send_uart("sd status: SD ERR\n\r");
	}
}

uint32_t sd_freespace() {
	if(sdReady) {
		f_getfree("", &fre_clust, &pfs);
		return free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	}
	else {
		return 0;
	}

}

uint32_t sd_totalspace() {
	if(sdReady) {
		f_getfree("", &fre_clust, &pfs);
		return total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	}
	else {
		return 0;
	}

}

void sd_readfile() {
	if(sdReady) {
	fresult = f_open(&fil, "file1.txt", FA_READ); //tu odczyt
		if(fresult==FR_OK) {
			send_uart("<data>: ");
			memset(buffer, 0, sizeof(buffer)); //czyszczenie bufora
			f_read(&fil, buffer, f_size(&fil), &br);
			send_uart(buffer);
			send_uart("\n\r");
		}
		else {
			send_uart("<error  opening file>\n\r");
		}
	}
}
volatile BYTE SD_SAVEMODE;

void sd_writefile(char sdWriteBuff[100]) {
	if(dataOverwrite) {
		SD_SAVEMODE = FA_CREATE_ALWAYS | FA_READ | FA_WRITE;
	}
	else {
		SD_SAVEMODE = FA_OPEN_APPEND | FA_READ | FA_WRITE;
	}
	if(sdReady) {
		fresult = f_open(&fil, "file1.txt", SD_SAVEMODE); // TU ZAPIS
		f_puts(sdWriteBuff, &fil);
		send_uart("<file written>\n\r");
	}
}

void sd_closefile() {
	if(sdReady) {
		fresult = f_close(&fil);
		send_uart("<closing file>\n\r");
	}
}
