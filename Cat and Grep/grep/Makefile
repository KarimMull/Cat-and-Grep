
FLAGS = -Wall -Wextra -Werror -std=c11 

all: s21_grep

test: s21_grep
	cp s21_grep grep.tests
	sh grep.tests/greybaca_grep_test_1.sh
	sh grep.tests/greybaca_grep_test_2.sh
	rm -f grep.tests/s21_grep
	rm grep.txt s21_grep.txt

s21_cat: Makefile s21_grep.c s21_grep.h
	gcc $(FLAGS) s21_grep.c -o s21_grep

check:
	clang-format -style=Google -n *.c

rebuild:
	make clean
	make all

clean:
	rm -f s21_grep