#ifndef S21_GREP
#define S21_GREP
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

typedef struct 
{
   bool e;
   int i;
   bool v;
   bool c;
   bool l;
   bool n;
   bool h;
   bool s;
   bool f;
   bool o;

   char *pattern; // Паттерны
   int len_pattern; // Длина всех паттернов
}Arguments;

void pattern_add(Arguments* arg, char* pattern);
void print_match(regex_t* reg, char* line);
Arguments define_arguments(int argc, char *argv[]);
void process_file(Arguments arg, char* path, regex_t* reg);
void output(Arguments arg, int argc, char* argv[]);

#endif