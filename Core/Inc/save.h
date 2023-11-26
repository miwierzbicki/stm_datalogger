#include <stdio.h>

#define BUFFER_SIZE 1000

typedef struct {
	uint8_t buffer[BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
} CircularBuffer;
void CircularBuffer_Init(CircularBuffer *buffer);
void CircularBuffer_Add(CircularBuffer *buffer, uint8_t data);
uint8_t CircularBuffer_Get(CircularBuffer *buffer);
void buff(void);
