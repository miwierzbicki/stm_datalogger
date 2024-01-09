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

extern bool sdReady;
extern volatile FRESULT fresult;
extern BYTE SD_SAVEMODE;
void sd_init();

void sd_readfile();
FRESULT sd_openfile();
FRESULT sd_writeline(const char* sdWriteBuff);
void sd_closefile();
void sd_demount();
const char* getFresultString(FRESULT fr);
