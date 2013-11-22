/**
 * Copyright © 2013 Jean-François Hren <jfhren@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "interpreter.h"
#include "stack.h"
#include "get_file_content.h"
#include "display.h"


static int duplicate(context* c);
static int delete(context* c);
static int negate(context* c);
static int openingBracket(context* c);
static int closingBracket(context* c);
static int greaterThan(context* c);
static int conditionalVerticalBounce(context* c);
static int getCharacter(context* c, char useNcurses);
static int getInteger(context* c, char useNcurses);
static int invokeFunction(context* c);
static int returnFromFunction(context* c);
static int formatCode(context* c, char* code);
static int conditionalHorizontalBounce(context* c);
static void slashChange(context* c);
static void antislashChange(context* c);


static int checkFileExtension(const char* filename) {

    int length = strlen(filename);
    return ((length > 5) && (filename[length - 5] == '.') && (filename[length - 4] == '0') && ((filename[length - 3] == 'x') || (filename[length - 3] == 'X')) && (filename[length - 2] == '2') && ((filename[length - 1] == 'a') || (filename[length - 1] == 'A'))) ? 0 : -1;

}


int initContext(const char* filename, context* c) {

    char* code = NULL;

    if(checkFileExtension(filename) == -1) {
        fprintf(stderr, "Wrong file extension\n");
        return -1;
    }

    code = getFileContent(filename);
    if(!code)
        return -1;

    if(formatCode(c, code) == -1) {
        free(code);
        return -1;
    }

    free(code);

    if(initStack(&(c->data)) == -1)
        return -1;

    if(initStack(&(c->call)) == -1)
        return -1;

    c->PC = 0;
    c->direction = RIGHT;

    return 0;

}


void freeContext(context* c) {

    free(c->p.code);
    freeStack(&(c->data));
    freeStack(&(c->call));

}


static void incrementPC(context* c) {

    switch(c->direction) {
        case LEFT:
            c->PC -= 1;
            return;

        case RIGHT:
            c->PC += 1;
            return;

        case UP:
            c->PC -= c->p.linesLength;
            return;

        case DOWN:
            c->PC += c->p.linesLength;
            return;
    }

}


static int sum(context* c) {

    int op1 = 0;
    int op2 = 0;
    int errorCode = 0;

    op1 = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr,"Not enough operand for the addition\n");
        return -1;
    }

    op2 = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "Not enough operand for the addition\n");
        return -1;
    }

    if(push(&(c->data), op1 + op2) == -1)
        return -1;

    return 0;

}


static int subtraction(context* c) {

    int op1 = 0;
    int op2 = 0;
    int errorCode = 0;

    op1 = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr,"Not enough operand for the soustraction\n");
        return -1;
    }

    op2 = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "Not enough operand for the soustraction\n");
        return -1;
    }

    if(push(&(c->data), op2 - op1) == -1)
        return -1;

    return 0;

}


static int printCharacter(context* c, char useNcurses) {

    int value = 0;
    int errorCode = 0;

    value = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "There is nothing to print\n");
        return -1;
    }

    if(useNcurses)
       ncursesPrintChar((char)value);
    else 
        printf("%c", (char)value);

    return 0;

}


static int printInteger(context* c, char useNcurses) {

    int value = 0;
    int errorCode = 0;

    value = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "There is nothing to print\n");
        return -1;
    }

    if(useNcurses)
        ncursesPrintInt(value);
    else
        printf("%d", value);

    return 0;

}


int eval(context* c, char useNcurses) {

    while((-1 < c->PC) && (c->PC < c->p.codeSize)) {

        char operator = c->p.code[c->PC];

        if(useNcurses)
            display(c->PC);

        if(isdigit(operator)) {
            if(push(&(c->data), operator - '0') == -1)
                return -1;
        } else if(('A' < operator) && (operator <= 'Z') && (operator != 'V')) {
            if(invokeFunction(c) == -1)
                return -1;
        } else if(('a' < operator) && (operator <= 'z') && (operator != 'v')) {
                incrementPC(c);
                continue;
        } else
            switch(operator) {
                case '<':
                    c->direction = LEFT;
                    break;
                case '>':
                    c->direction = RIGHT;
                    break;

                case 'v':
                    c->direction = DOWN;
                    break;

                case '^':
                    c->direction = UP;
                    break;

                case '/':
                    slashChange(c);
                    break;

                case '\\':
                    antislashChange(c);
                    break;

                case '+':
                    if(sum(c) == -1)
                        return -1;
                    break;

                case '-':
                    if(subtraction(c) == -1)
                        return -1;
                    break;

                case '\'':
                    if(printCharacter(c, useNcurses) == -1)
                        return -1;
                    break;

                case '.':
                    if(printInteger(c, useNcurses) == -1)
                        return -1;
                    break;

                case ' ':
                case '\n':
                    break;

                case '%':
                    if(duplicate(c) == -1)
                        return -1;
                    break;

                case '*':
                    if(delete(c) == -1)
                        return -1;
                    break;

                case '|':
                    if(conditionalVerticalBounce(c) == -1)
                        return -1;
                    break;

                case '_':
                    if(conditionalHorizontalBounce(c) == -1)
                        return -1;
                    break;

                case '~':
                    incrementPC(c);
                    break;

                case 'a':
                case 'A':
                    if(push(&(c->data), operator) == -1)
                        return -1;
                    break;

                case '[':
                    if(openingBracket(c) == -1)
                        return -1;
                    break;

                case ']':
                    if(closingBracket(c) == -1)
                        return -1;
                    break;

                case '!':
                    if(negate(c) == -1)
                        return -1;
                    break;

                case '`':
                    if(greaterThan(c) == -1)
                        return -1;
                    break;

                case '@':
                    if(getCharacter(c, useNcurses) == -1)
                        return -1;
                    break;

                case '=':
                    if(getInteger(c, useNcurses) == -1)
                        return -1;
                    break;

                case '#':
                    if(returnFromFunction(c) == -1)
                        return 0;
                    break;

                default:
                    return -1;
            }

        incrementPC(c);

    }

    return -1;

}


static int duplicate(context* c) {

    int value = 0;
    int errorCode = 0;

    value = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "There is nothing to clone\n");
        return -1;
    }

    if(push(&(c->data), value) == -1)
        return -1;

    if(push(&(c->data), value) == -1)
        return -1;

    return 0;

}


static int delete(context* c) {

    int errorCode = 0;

    pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "There is nothing to remove\n");
        return -1;
    }

    return 0;

}


static int negate(context* c) {

    int value = 0;
    int errorCode = 0;

    value = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "There is nothing to negate\n");
        return -1;
    }
    if(value) {
        if(push(&(c->data), 0) == -1)
            return -1;
    } else {
        if(push(&(c->data), 1) == -1)
            return -1;
    }

    return 0;

}


static int getMatchingClosingBracket(context* c) {

    char* pos1 = strchr(c->p.code + c->PC + 1, ']');
    char* pos2 = strchr(c->p.code + c->PC + 1, '[');

    if(!pos1)
        return -1;

    while(pos2 && pos2 < pos1) {
        pos1 = strchr(pos1 + 1, ']');
        pos2 = strchr(pos2 + 1, '[');
    }

    return (int)(pos1 - c->p.code);

}


static int openingBracket(context* c) {

    if((c->direction == DOWN) || (c->direction == UP)) {
        return 0;
    } else {
        int value = 0;
        int errorCode = 0;

        value = pop(&(c->data), &errorCode);
        if(errorCode == -1) {
            fprintf(stderr, "There is nothing to test\n");
            return -1;
        }
        if(((c->direction == RIGHT) && !value) || ((c->direction == LEFT) && value)) {
            int addressMatchingClosingBracket = getMatchingClosingBracket(c);
            if(addressMatchingClosingBracket == -1) {
                fprintf(stderr,"Missing closing bracket");
                return -1;
            }
            c->PC = addressMatchingClosingBracket;
        }

    }

    return 0;

}


static int getMatchingOpeningBracket(context* c) {

    char* pos1 = (char*)(c->p.code + c->PC - 1);
    char* pos2 = NULL;

    while((pos1 >= c->p.code) && (*pos1 != '['))
        --pos1;

    pos2 = pos1;

    while(pos1 >= c->p.code) {
        if(*pos1 == '[') {
            pos2 = strchr(pos2 + 1, ']');
            if(pos2 == (char*)(c->p.code + c->PC))
                return (int)(pos1 - c->p.code);
        }
        --pos1;
    }

    return -1;

}


static int closingBracket(context* c) {

    if((c->direction == DOWN) || (c->direction == UP)) {
        return 0;
    } else {
        int value = 0;
        int errorCode = 0;

        value = pop(&(c->data), &errorCode);
        if(errorCode == -1) {
            fprintf(stderr, "There is nothing to test\n");
            return -1;
        }
        if(((c->direction == RIGHT) && value) || ((c->direction == LEFT) && !value)) {
            int addressMatchingOpeningBracket = getMatchingOpeningBracket(c);
            if(addressMatchingOpeningBracket == -1)
                return -1;
            c->PC = addressMatchingOpeningBracket;
        }

    }

    return 0;
}


static int greaterThan(context* c) {

    int op1 = 0;
    int op2 = 0;
    int errorCode = 0;

    op1 = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "There is not enough value to compare\n");
        return -1;
    }

    op2 = pop(&(c->data), &errorCode);
    if(errorCode == -1) {
        fprintf(stderr, "There is not enough value to compare\n");
        return -1;
    }

    if(push(&(c->data), (op2 > op1) ? 1 : 0) == -1)
        return -1;

    return 0;

}


static int conditionalVerticalBounce(context* c) {

    if((c->direction == DOWN) || (c->direction == UP)) {
        return 0;
    } else {
        int value = 0;
        int errorCode = 0;

        value = pop(&(c->data), &errorCode);
        if(errorCode == -1) {
            fprintf(stderr, "There is nothing to test\n");
            return -1;
        }

        if(value) {
            if(c->direction == RIGHT)
                c->direction = LEFT;
            else 
                c->direction = RIGHT;
        }
    }

    return 0;

}


static int getCharacter(context* c, char useNcurses) {

    int character = 0;

    if(useNcurses)
        pauseNcurses();

    character = getchar();

    if(useNcurses)
        resumeNcurses();

    if(isgraph(character) || isspace(character)) {
        if(push(&(c->data), character) == -1)
            return -1;
    } else {
        if(push(&(c->data), 0) == -1)
            return -1;
    }

    return 0;

}


static int getInteger(context* c, char useNcurses) {

    int integer = 0;
    char buffer[128];
    int i = 0;

    if(useNcurses)
        pauseNcurses();

    printf("Enter an integer: ");
    fflush(stdout);

    buffer[i] = getchar();
    while(buffer[i] != 10) {
        i++;
        if(i == 128)
            break;
        buffer[i] = getchar();
    }

    if(useNcurses)
        resumeNcurses();

    if(i == 128)
        i = 127;
    buffer[i] = '\0';
    integer = atoi(buffer);

    if(push(&(c->data), integer) == -1)
        return -1;

    return 0;

}


static int invokeFunction(context* c) {

    int functionPosition = -1;
    char function = c->p.code[c->PC] + 32;

    if((c->direction == RIGHT) || (c->direction == DOWN)) {
        char* position = strchr(c->p.code + c->PC + 1, function);
        if(position)
            functionPosition = (int)(position - c->p.code);
    } else {
        int position = c->PC - 1;
        while((position > -1) && (c->p.code[position] != function))
            --position;
        if(position != -1)
            functionPosition = position;
    }

    if(functionPosition == -1) {
        fprintf(stderr, "Could not find the function %c", function);
        return -1;
    } else {
        if(push(&(c->call), c->PC) == -1)
            return -1;
        c->PC = functionPosition;
    }

    return 0;

}


static int returnFromFunction(context* c) {

    int newPC = 0;
    int errorCode = 0;

    newPC = pop(&(c->call), &errorCode);
    if(errorCode == -1)
        return -1;
    c->PC = newPC;

    return 0;

}


static int formatCode(context* c, char* code) {

    char* prevPos = code;
    char* crtPos = NULL;
    int nbLines = 1;
    c->p.linesLength = 0;

    while((crtPos = strchr(prevPos, '\n')) != NULL) {
        int length = (int)(crtPos - prevPos);
        nbLines++;
        if(length > c->p.linesLength)
            c->p.linesLength = length;
        prevPos = crtPos + 1;
    }

    if(nbLines == 1)
        c->p.linesLength = strlen(code);
    else {
        int length = strlen(prevPos);
        if(length > c->p.linesLength)
            c->p.linesLength = length;
    }

    c->p.code = (char*)malloc((c->p.linesLength * nbLines) + 1);
    if(!c->p.code)
        return -1;

    if(nbLines == 1) {
        memcpy(c->p.code, code, c->p.linesLength + 1);
    } else {
        int length = 0;
        int i = 0;

        prevPos = code;
        crtPos = NULL;

        while((crtPos = strchr(prevPos, '\n')) != NULL) {
            length = (int)(crtPos - prevPos);
            memcpy(c->p.code + i, prevPos, length);
            memset(c->p.code + i + length, ' ', c->p.linesLength - length);
            i += c->p.linesLength;
            prevPos = crtPos + 1;
        }
        length = strlen(prevPos);
        memcpy(c->p.code + i, prevPos, length);
        memset(c->p.code + i + length, ' ', c->p.linesLength - length);
        c->p.code[i + c->p.linesLength] = '\0';
    }

    c->p.codeSize = c->p.linesLength * nbLines;

    return 0;

}


static int conditionalHorizontalBounce(context* c) {

    if((c->direction == LEFT) || (c->direction == RIGHT)) {
        return 0;
    } else {
        int value = 0;
        int errorCode = 0;

        value = pop(&(c->data), &errorCode);
        if(errorCode == -1) {
            fprintf(stderr, "There is nothing to test\n");
            return -1;
        }

        if(value) {
            if(c->direction == DOWN)
                c->direction = UP;
            else 
                c->direction = DOWN;
        }
    }

    return 0;

}


static void slashChange(context* c) {

    switch(c->direction) {
        case LEFT:
            c->direction = DOWN;
            return;

        case RIGHT:
            c->direction = UP;
            return;

        case UP:
            c->direction = RIGHT;
            return;

        case DOWN:
            c->direction = LEFT;
            return;
    }

}


static void antislashChange(context* c) {

    switch(c->direction) {
        case LEFT:
            c->direction = UP;
            return;

        case RIGHT:
            c->direction = DOWN;
            return;

        case UP:
            c->direction = LEFT;
            return;

        case DOWN:
            c->direction = RIGHT;
            return;
    }

}
