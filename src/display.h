/**
 * Copyright © 2013 Jean-François Hren <jfhren@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

/* Initialize ncurses
 * code       : the code to display
 * codeSize   : the size of aforementioned code
 * linesLength: the length of the lines of code
 * delay      : the timeout delay for keyboard event
 */
void initDisplay(char* code, int codeSize, int linesLength, int delay);

/* Print the character in the output area. '\n' makes a new line
 * c: the character to print
 */
void ncursesPrintChar(char c);

/* Print the integer in the output area
 * i: the integer to print
 */
void ncursesPrintInt(int i);

/* Leave the ncurses mode temporarily
 */
void pauseNcurses(void);

/* Restore the ncurses left before
 */
void resumeNcurses(void);

/* Display the current position of the program counter
 */
void display(int PC);

/* uninitialize ncurses
 */
void uninitDisplay(void);
