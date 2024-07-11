#include "s21_cat.h"

int main(int argc, char *argv[]) {
  define_flags_and_files(argc, argv);  // Обрабатываем файлы
  return 0;
}

void define_flags_and_files(int argc, char *argv[]) {
  // Структура для корректной работы getopt_long 
  struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                  {"number", 0, NULL, 'n'},
                                  {"squeeze-blank", 0, NULL, 's'},
                                  {NULL, 0, NULL, 0}};
  int current_flag = 0;  // Текущий флаг
  const char *short_options = "bevEnstT";  // Сбрабатываемые короткие флаги
  Flags flags = {false, false, false, false,
                 false, false, false};  // Создаём экземпляр структуры

  // Обработка флагов
  while ((current_flag =
              getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {  // Записываем в текущий флаг результат getopt_long, пока не
                // кончились флаги
    switch (current_flag)  // Смотрим какой текущий флаг
    {
      case 'b':
        flags.b = true;  // Устанавливаем соответствующий флаг
        break;
      case 'e':
        flags.e = true;  // Устанавливаем соответствующий флаг
        flags.v = true;  // Устанавливаем соответствующий флаг
        break;
      case 'E':
        flags.e = true;  // Устанавливаем соответствующий флаг
        break;
      case 'n':
        flags.n = true;  // Устанавливаем соответствующий флаг
        break;
      case 's':
        flags.s = true;  // Устанавливаем соответствующий флаг
        break;
      case 't':
        flags.t = true;  // Устанавливаем соответствующий флаг
        flags.v = true;  // Устанавливаем соответствующий флаг
        break;
      case 'T':
        flags.t = true;  // Устанавливаем соответствующий флаг
        break;
      case 'v':
        flags.v = true;  // Устанавливаем соответствующий флаг
        break;
      case '?':              // Если что-то не так
        exit(EXIT_FAILURE);  // Завершаем программу
    }
  }

  // Обработка файлов
  if (optind < argc) {  // Если индекс на котором закончились параметры не
                        // последний из всего количества аргументов

    for (int index_of_file = optind; index_of_file < argc;
         index_of_file++) {  // Если индекс на котором закончились флаги меньше
                             // количества аргуметов
      FILE *file;            // Создаём объект для файла

      if ((file = fopen(argv[index_of_file], "r")) !=
          NULL)  // Если открытие файла с этим индексом прошло нормально
      {
        process_flag(flags, file);  // Обработать флаг
        fclose(file);               // Закрываем файд
      } else {
        perror(argv[index_of_file]);  // Выводим что такого файла нет
        exit(EXIT_FAILURE);  // Завершаем программу
      }
    }
  }
}

// Применяет флаги к файлу
void process_flag(Flags flags, FILE *file) {
  long N = filesize(file);  // Размер файла в байтах (количество символов)
  char processed_file[N];  // Файл в виде строки после флагов (объявляем заранее
                           // с большим количеством элементов)
  int counter = 1;  // Номер строки
  int i = 0;        // Индекс символа

  while ((processed_file[i] = getc(file)) !=
         EOF) {  // Считываем посимвольно файл

    if (flags.s) {  // Если сработал флаг -s (находиться вначале потому что при
                    // применении с другими флагами срабатывает первее)
      if (i == 1) {  // Если это 2 символ
        if (processed_file[i] == '\n' &&
            processed_file[i - 1] ==
                '\n') {  // Если 1 и 2 символы это перевод строки
          continue;  // Пропускаем итерацию цикла, чтобы не выводить перевод
                     // строки
        }
      }
      if (i != 0 && i != 1) {
        if (processed_file[i] == '\n' && processed_file[i - 1] == '\n' &&
            processed_file[i - 2] ==
                '\n') {  // Если три подряд символа это переводы строки
          continue;  // Пропускаем итерацию цикла, чтобы не выводить перевод
                     // строки
        }
      }
    }

    if (flags.b || flags.n) {  // Если сработал флаг -b или -n

      if (flags.b) {  // Если сработал флаг -b

        if (i == 0) {  // Если это первый символ в файле
          if (processed_file[i] !=
              '\n') {  // Если первый символ это не перевод строки
            printf("%6d\t", counter);  // Выводим номер строки с отступом
            counter++;  // Увеличиваем номер строки
          }
        } else {  // Если это не первый символ в файле
          if (processed_file[i - 1] == '\n' &&
              processed_file[i] !=
                  '\n') {  // Если текущий символ не перевод строки, а
                           // предыдущий символ наоборот
            printf("%6d\t", counter);  // Выводим номер строки с отступом
            counter++;  // Увеличиваем номер строки
          }
        }

      } else {  // Если сработал флаг -n (при этом если есть сразу два флага то
                // выполнится -b)
        if (i == 0) {  // Если это первый символ в файле
          printf("%6d\t", counter);  // Выводим номер строки с отступом
          counter++;  // Увеличиваем номер строки
        } else {  // Если это не первый символ в файле
          if (processed_file[i - 1] ==
              '\n') {  // Если предыдущий символ перевод строки
            printf("%6d\t", counter);  // Выводим номер строки с отступом
            counter++;  // Увеличиваем номер строки
          }
        }
      }
    }

    if (flags.v) {  // Если сработал флаг -v
      if (processed_file[i] != '\n' &&
          processed_file[i] !=
              '\t') {  // Если текущий символ не перенос строки и не табуляция
        if (processed_file[i] < 0) {  // Если код символа меньше 0
          printf("M-");               // Печатаем M-
          processed_file[i] =
              processed_file[i] & 0x7F;  // Сделать текущий символ выводимым
        }
        if (processed_file[i] < 32) {  // Если код символа меньше 32
          putchar('^');                // Печатаем ^
          processed_file[i] += 64;  // Увеличиваем код символа на 64
        } else {
          if (processed_file[i] == 127) {  // Если код символа равен 127
            putchar('^');                  // Печатаем ^
            processed_file[i] = '?';  // Присваиваем текущему символу значение ?
          }
        }
      }
    }

    if (flags.e) {  // Если сработал флаг -e
      if (processed_file[i] == '\n') {  // Если текущий элемент перевод строки
        printf("%c", '$');  // Выводим символ $
      }
    }

    if (flags.t) {                      // Если сработал флаг -t
      if (processed_file[i] == '\t') {  // Если текущий элемент табуляция
        printf("%c", '^');  // Выводим символ ^
        processed_file[i] = 'I';  // В текущий символ записываем I
      }
    }

    printf("%c", processed_file[i]);  // Выводим текущий символ

    i++;  // Увеличиваем индекс символа
  }
}
long int filesize(FILE *fp) {  // Возвращает размер файла в байтах

  long int save_pos, size_of_file;  // Объявляем нужные для работы переменные

  save_pos = ftell(fp);  // Понятия не имею что делает, но работает
  fseek(fp, 0L, SEEK_END);  // Понятия не имею что делает, но работает
  size_of_file = ftell(fp);  // Понятия не имею что делает, но работает
  fseek(fp, save_pos, SEEK_SET);  // Понятия не имею что делает, но работает
  return size_of_file;  // Возвращает размер файла
}
