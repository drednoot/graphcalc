CC=gcc
FLAGS=-Wall -Wextra -Werror -std=c11

STACK_OBJ=./backend/double_stack.o ./backend/lexem_stack.o
PARSER_OBJ=./backend/parser.o
ALGORITHM_OBJ=./backend/algorithm.o

all: backend.a build_graphics

backend.a: ${STACK_OBJ} ${PARSER_OBJ} ${ALGORITHM_OBJ}
	ar rcs $@ $^

utility.a: utility.o
	ar rcs $@ $^

generate_graphics_makefile:
	qmake -o graphics.mk

create_build:
	mkdir -p ./build

build_graphics: generate_graphics_makefile create_build backend.a utility.a
	make -f graphics.mk all
	cp smartcalc ./build/

dist: generate_graphics_makefile create_build
	make -f graphics.mk dist
	mv *.tar.gz ./build/

./%.o: ./%.c
	$(CC) $(FLAGS) $(DEBUGFLAGS) -c $^ -o $@

clean_backend:
	rm -rf backend/*.o
	rm -rf backend/*.gcda
	rm -rf backend/*.gcno

clean_graphics:
	rm -rf .qmake.stash
	rm -rf moc_*
	rm -rf graphics.mk
	rm -rf .cache
	rm -rf .tmp
	rm -rf qrc_icons.cpp

clean_build:
	rm -rf ./build

clean: clean_backend clean_graphics
	rm -rf *.a
	rm -rf *.o
	rm -rf *.dSYM
	rm -rf test
	rm -rf main
	rm -rf smartcalc
	rm -rf gcov_report
	rm -rf *.gcno
	rm -rf vgcore.*
