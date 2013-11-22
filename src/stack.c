/**
 * Copyright © 2013 Jean-François Hren <jfhren@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "stack.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int initStack(stack* s) {

    int* tmp = (int*)malloc(sizeof(int) * STACK_BLOCK_SIZE);
    if(!tmp) {
        perror("initStack");
        return -1;
    }

    memset(tmp, 0x2A, sizeof(int) * STACK_BLOCK_SIZE);

    s->data = tmp;
    s->size = STACK_BLOCK_SIZE;
    s->position = -1;

    return 0;

}


int pop(stack* s, int* errorCode) {

    if(s->position == -1) {
        *errorCode = -1;
        return 0;
    }

    return s->data[s->position--];

}


int push(stack* s, int value) {

    if(s->position == (s->size - 1)) {
        int* tmp = realloc(s->data, sizeof(int) * (s->size + STACK_BLOCK_SIZE));
        if(!tmp) {
            perror("push");
            return -1;
        }

        s->data = tmp;
        s->size += STACK_BLOCK_SIZE;
        memset(s->data + s->position + 1, 0x2A, sizeof(int) * STACK_BLOCK_SIZE);
    }

    ++s->position;
    s->data[s->position] = value;

    return 0;

}


void freeStack(stack* s) {

    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->position = -1;

}
