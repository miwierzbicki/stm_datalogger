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
extern BYTE SD_SAVEMODE;
void sd_init();
uint32_t sd_totalspace();
uint32_t sd_freespace();
void sd_readfile();
void sd_writefile();
void sd_closefile();
void sd_demount();
