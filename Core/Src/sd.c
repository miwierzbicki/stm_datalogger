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
#include "fatfs.h" //sprawdzic czy to potrzebne XD

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
		send_uart("sd status: sd err\n\r");
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
			send_uart("opening file1.txt, data: \n\r");
			f_read(&fil, buffer, f_size(&fil), &br);
			send_uart(buffer);
			send_uart("\n\r");
		}
		else {
			send_uart("error while opening file... \n\r");
		}
	}
}

void sd_writefile() {
	if(sdReady) {
		fresult = f_open(&fil, "file1.txt", FA_CREATE_ALWAYS | FA_READ | FA_WRITE); // TU ZAPIS
		/* Writing text */
		f_puts("test test test", &fil);
		send_uart("file written \n\r");
	}
}

void sd_closefile() {
	if(sdReady) {
		fresult = f_close(&fil);
		send_uart("closing file\n\r");
	}
}
