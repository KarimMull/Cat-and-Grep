#ifndef S21_CAT
#define S21_CAT
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  bool b;
  bool e;
  bool n;
  bool s;
  bool t;
  bool u;
  bool v;
} Flags;

void define_flags_and_files(int argc, char *argv[]);
void process_flag(Flags flags, FILE *file);
long int filesize(FILE *fp);

#endif