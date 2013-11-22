/**
 * Copyright © 2013 Jean-François Hren <jfhren@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

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
