#ifndef STACK_H
#define STACK_H

#define STACK_BLOCK_SIZE 16

typedef struct {
    int* data;
    int size;
    int position;
} stack;
#endif

int initStack(stack* s);

void freeStack(stack* s);

int pop(stack* s, int* errorCode);

int push(stack* s, int value);
