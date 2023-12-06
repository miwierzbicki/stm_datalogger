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


#define BUFFER_SIZE 1000

typedef struct {
	uint8_t buffer[BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
} CircularBuffer;

void CircularBuffer_Init(CircularBuffer *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
}

CircularBuffer csvLineBuffer;


bool CircularBuffer_Add(CircularBuffer *buffer, uint8_t data) {
    // czy ma miejsce
    if (((buffer->head + 1) % BUFFER_SIZE) != buffer->tail) {
        buffer->buffer[buffer->head] = data;
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
        return true;
    }
    return false;
    // dopisać nadpisywanie danych gdy pełny
}


// Funkcja pobierająca element z bufora
uint8_t CircularBuffer_Get(CircularBuffer *buffer) {
    // Sprawdź, czy bufor nie jest pusty
    if (buffer->head != buffer->tail) {
        uint8_t data = buffer->buffer[buffer->tail];
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
        return data;
    }
    //jeżeli pusty
    return 0;
}

void buff(void) {

	CircularBuffer_Init(&csvLineBuffer);
	// Przykładowe użycie bufora kołowego
	CircularBuffer_Add(&csvLineBuffer, 10);
	//do bufora bedzie dodawana gotowa linijka stringa do .csv
	CircularBuffer_Add(&csvLineBuffer, 20);
	CircularBuffer_Add(&csvLineBuffer, 'B');
	//uint8_t data = CircularBuffer_Get(&csvLineBuffer);
	char temp[10];
	sprintf(temp, "%u \n\r", CircularBuffer_Get(&csvLineBuffer));
	send_uart(temp);
	sprintf(temp, "%u \n\r", CircularBuffer_Get(&csvLineBuffer));
	send_uart(temp);
	sprintf(temp, "%u \n\r", CircularBuffer_Get(&csvLineBuffer));
	send_uart(temp);

}

//void printBuffor(void) {
//	send_uart(CircularBuffer_Get(&csvLineBuffer));
//	send_uart("\n\r");
//}
