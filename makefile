CC := gcc
CFLAGS := -W -Wall -ansi -pedantic -Werror -g
LIBS := -lncurses

all: mkd bin/interpreter

bin/interpreter: obj/main.o obj/interpreter.o obj/stack.o obj/display.o obj/get_file_content.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

mkd:
	mkdir -p obj/
	mkdir -p bin/

clean:
	rm -Rf obj/ bin/
