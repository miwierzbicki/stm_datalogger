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

CircularBuffer myBuffer;


bool CircularBuffer_Add(CircularBuffer *buffer, uint8_t data) {
    // Sprawdź, czy bufor nie jest pełny
    if (((buffer->head + 1) % BUFFER_SIZE) != buffer->tail) {
        buffer->buffer[buffer->head] = data;
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
        return true;
    }
    return false;
    // Jeśli bufor jest pełny, można obsłużyć to w odpowiedni sposób (np. zignorować nowe dane lub nadpisać stare)
}


// Funkcja pobierająca element z bufora
uint8_t CircularBuffer_Get(CircularBuffer *buffer) {
    // Sprawdź, czy bufor nie jest pusty
    if (buffer->head != buffer->tail) {
        uint8_t data = buffer->buffer[buffer->tail];
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
        return data;
    }
    // Jeśli bufor jest pusty, można obsłużyć to w odpowiedni sposób (np. zwrócić specjalną wartość lub zablokować wykonanie)
    return 0;
}

void buff(void) {

	CircularBuffer_Init(&myBuffer);
	// Przykładowe użycie bufora kołowego
	CircularBuffer_Add(&myBuffer, 10);
	CircularBuffer_Add(&myBuffer, 20);
	CircularBuffer_Add(&myBuffer, 'B');
	//uint8_t data = CircularBuffer_Get(&myBuffer);
	char temp[10];
	sprintf(temp, "%u \n\r", CircularBuffer_Get(&myBuffer));
	send_uart(temp);
	sprintf(temp, "%u \n\r", CircularBuffer_Get(&myBuffer));
	send_uart(temp);
	sprintf(temp, "%u \n\r", CircularBuffer_Get(&myBuffer));
	send_uart(temp);

}

//void printBuffor(void) {
//	send_uart(CircularBuffer_Get(&myBuffer));
//	send_uart("\n\r");
//}
