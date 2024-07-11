
FLAGS = -Wall -Wextra -Werror -std=c11 

all: s21_cat

test: s21_cat
	cp s21_cat test_files
	sh test_files/tests.sh
	rm -f test_files/s21_cat

s21_cat: Makefile s21_cat.c s21_cat.h
	gcc $(FLAGS) s21_cat.c -o s21_cat

check:
	clang-format -style=Google -n *.c

rebuild:
	make clean
	make all

clean:
	rm -f s21_cat
