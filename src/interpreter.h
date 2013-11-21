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
