/**
 * Copyright © 2013 Jean-François Hren <jfhren@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "display.h"

#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <ctype.h>

#define OUTPUT_AREA_ROW_SIZE 4
#define MAX_WIDTH 512

static int oldPC = -1;
static int oldPCCol = -1;
static int oldPCRow = -1;
static int oldNbCol = -1;
static int oldNbRow = -1;
static int oldColOffset = 0;
static int oldRowOffset = 0;

static int printColPos = 0;
static int printRowPos = 0;

static char* code = NULL;
static int codeSize = 0;
static int linesLength = -1;

static chtype buffer[OUTPUT_AREA_ROW_SIZE * MAX_WIDTH];


void initDisplay(char* c, int cs, int ll, int delay) {

    initscr();
    raw();
    noecho();
    curs_set(0);
    timeout(delay);
    refresh();

    code = c;
    codeSize = cs;
    linesLength = ll;

}


static void getPCColRow(char* code, int codeSize, int PC, int* col, int* row) {

    int i = 0;
    *col = 0;
    *row = 0;

    while(i < codeSize && (PC != i)) {
        if(code[i] == '\n') {
            *col = 0;
            ++(*row);
        } else 
            ++(*col);

        ++i;
    }

}


static void saveOutputArea(void) {

    int i = 0;
    for(; i < OUTPUT_AREA_ROW_SIZE; ++i) {
        move(oldNbRow + i + 1, 0);
        inchnstr(buffer + (i * MAX_WIDTH), MAX_WIDTH);
    }

}


static void restoreOutputArea(void) {

    int i = 0;
    for(; i < OUTPUT_AREA_ROW_SIZE; ++i) {
        move(oldNbRow + i + 1, 0);
        addchnstr(buffer + (i * MAX_WIDTH), MAX_WIDTH);
    }

}


void display(int PC) {

    int c = 0;
    int nbCol = 0;
    int nbRow = 0;
    int PCCol = 0;
    int PCRow = 0;
    int colOffset = 0;
    int rowOffset = 0;

    getmaxyx(stdscr, nbRow, nbCol);

    if(linesLength == -1)
        getPCColRow(code, codeSize, PC, &PCCol, &PCRow);
    else {
        PCCol = PC % linesLength;
        PCRow = PC / linesLength;
    }

    /* Taking space for the header */
    PCRow += 1;

    /* Taking space for output and separation*/
    nbRow -= (OUTPUT_AREA_ROW_SIZE + 1);

    if(nbCol > MAX_WIDTH)
        nbCol = MAX_WIDTH;

    if(PCCol >= nbCol)
        colOffset = PCCol - nbCol + 1;
    if(PCRow >= nbRow)
        rowOffset = PCRow - nbRow + 1;

    if((oldPC == -1) || (nbRow != oldNbRow) || (nbCol != oldNbCol) || (colOffset != oldColOffset) || (rowOffset != oldRowOffset)) {
        int i = 0;
        int crtRow = 1;
        int crtCol = 0;
        int crtColOffset = colOffset;
        int crtRowOffset = rowOffset;

        oldNbRow = nbRow;
        oldNbCol = nbCol;
        oldColOffset = colOffset;
        oldRowOffset = rowOffset;

        clear();

        move(0, nbCol - 17);
        printw("(Press q to quit)");

        for(; i < codeSize; ++i) {
            if(crtCol < nbCol) {
                if(!crtColOffset) {
                    mvaddch(crtRow, crtCol, (isprint(code[i]) ? code[i] : ' ') | ((i == PC)? A_REVERSE : 0));
                    ++crtCol;
                } else
                    --crtColOffset;
            }

            if(((linesLength != -1) && i && (((i + 1) % linesLength) == 0)) || (code[i] == '\n')) {
                if(!crtRowOffset)
                    ++crtRow;
                else
                    --crtRowOffset;

                if(crtRow == nbRow)
                    break;

                crtColOffset = colOffset;
                crtCol = 0;
            }
        }

        move(nbRow, 0);
        for(i = 0; i < (nbCol - 13)/2; ++i)
            addch('-');
        printw(" Output area ");
        for(i += 13; i < nbCol; ++i)
            addch('-');

        restoreOutputArea();
    } else {
        mvaddch(oldPCRow - rowOffset, oldPCCol - colOffset, isprint(code[oldPC]) ? code[oldPC] : ' ');
        mvaddch(PCRow - rowOffset, PCCol - colOffset, (isprint(code[PC]) ? code[PC] : ' ') | A_REVERSE);
    }

    refresh();

    c = getch();
    if((c == 'Q') || (c == 'q')) {
        uninitDisplay();
        exit(EXIT_SUCCESS);
    }

    oldPC = PC;
    oldPCCol = PCCol;
    oldPCRow = PCRow;
}


static void outputAreaNewLine(void) {

    chtype tmpBuffer[MAX_WIDTH];
    int i = 1;

    for(; i < OUTPUT_AREA_ROW_SIZE; ++i) {
        move(oldNbRow + i + 1, 0);
        inchnstr(tmpBuffer, MAX_WIDTH);
        move(oldNbRow + i, 0);
        addchnstr(tmpBuffer, MAX_WIDTH);
    }

    move(oldNbRow + OUTPUT_AREA_ROW_SIZE, 0);
    clrtoeol();

}


void ncursesPrintChar(char c) {

    if((c == '\n') || (printColPos >= (oldNbCol - 1))) {
        ++printRowPos;
        printColPos = 0;
    }

    if(printRowPos == OUTPUT_AREA_ROW_SIZE) {
        outputAreaNewLine();
        printRowPos = OUTPUT_AREA_ROW_SIZE - 1;
    }

    if(c != '\n') {
        move(oldNbRow + printRowPos + 1, printColPos);
        addch(c);
        ++printColPos;
        clrtobot();
    }
    refresh();

    saveOutputArea();

}


void ncursesPrintInt(int i) {

    if(printColPos >= (oldNbCol - 1)) {
        ++printRowPos;
        printColPos = 0;
    }

    if(printRowPos >= OUTPUT_AREA_ROW_SIZE) {
        outputAreaNewLine();
        printRowPos = OUTPUT_AREA_ROW_SIZE - 1;
    }

    move(oldNbRow + printRowPos + 1, printColPos);
    printw("%d",i);
    printColPos += sprintf(NULL, "%d", i);
    clrtobot();
    refresh();

    saveOutputArea();

}


void pauseNcurses(void) {

    def_prog_mode();
    endwin();

}

void resumeNcurses(void) {

    reset_prog_mode();
    refresh();

}

void uninitDisplay(void) {

    getch();
    endwin();

}
