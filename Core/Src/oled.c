#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"



void displayInit(void) {
	ssd1306_Init();
}

void displayUpdate(void) {
	ssd1306_UpdateScreen();
}

void displayWrite(const char* str) {
	ssd1306_WriteString(str, Font_6x8, White);
}
