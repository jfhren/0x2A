/**
 * Copyright © 2013 Jean-François Hren <jfhren@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#define LEFT -1
#define RIGHT 1
#define UP -2
#define DOWN 2

#include "stack.h"

typedef struct {
    char* code;
    int codeSize;
    int linesLength;
} program;

typedef struct {
    program p;
    stack data;
    stack call;
    int PC;
    char direction;
} context;
#endif

int initContext(const char* filename, context* c);

int eval(context* c, char useNcurses);

void freeContext(context* c);
