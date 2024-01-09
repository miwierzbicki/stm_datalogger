#include <stdio.h>

#define BUF_SIZE 100
#define STR_SIZE 1000

typedef struct {
    char data[BUF_SIZE][STR_SIZE];
    int head;
    int tail;
} CircularBuffer;

extern CircularBuffer cb;

void CircularBuffer_Init(CircularBuffer* cb);
void CircularBuffer_Add(CircularBuffer* cb, char* str);
char* CircularBuffer_Read(CircularBuffer* cb);
