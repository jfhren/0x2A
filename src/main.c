/**
 * Copyright © 2013 Jean-François Hren <jfhren@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */

#include "interpreter.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "display.h"

int main(int argc, char* argv[]) {

    context c;
    char useNcurses = 0;
    int delay = 200;

    if((argc < 2) || (argc > 4) || ((argc >= 3) && strcmp("-d", argv[1]))) {
        fprintf(stderr, "Usage: %s [-d[ delay]] file\n", argv[0]);
        return EXIT_FAILURE;
    }

    if((argc >= 3) && (strcmp("-d", argv[1]) == 0)) {
        useNcurses = 1;
        if(argc == 4)
            delay = atoi(argv[2]);
    }

    if(initContext(argv[argc - 1], &c) == -1)
        return EXIT_FAILURE;

    if(useNcurses) {
        initDisplay(c.p.code, c.p.codeSize, c.p.linesLength, delay);
    }

    if(eval(&c, useNcurses) == -1) {
        uninitDisplay();
        freeContext(&c);
        return EXIT_FAILURE;
    }

    if(useNcurses)
        uninitDisplay();

    freeContext(&c);
    return EXIT_SUCCESS;

}
