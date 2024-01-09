#include <stdio.h>
#include "ssd1306_conf.h"
#include "ssd1306_fonts.h"
#include "ssd1306_tests.h"
#include "ssd1306.h"
#include "oled.h"
#include "menu.h"
#include "main.h"
#include <string.h>
#include <stdbool.h>
#include "encoder.h"

#define BUF_SIZE 100
#define STR_SIZE 1000


typedef struct {
    char data[BUF_SIZE][STR_SIZE];
    int head;
    int tail;
} CircularBuffer;



void CircularBuffer_Init(CircularBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
}

volatile CircularBuffer cb;

void CircularBuffer_Add(CircularBuffer* cb, char* str) {
    strncpy(cb->data[cb->head], str, STR_SIZE);
    cb->head = (cb->head + 1) % BUF_SIZE;
    if (cb->head == cb->tail) {
        cb->tail = (cb->tail + 1) % BUF_SIZE;
    }
}

char* CircularBuffer_Read(CircularBuffer* cb) {
    if (cb->head == cb->tail) {
        printf("bufor jest pusty\n");
        return "";
    }
    char* str = cb->data[cb->tail];
    cb->tail = (cb->tail + 1) % BUF_SIZE;
    return str;
}
