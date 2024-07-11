#include "s21_grep.h"

void pattern_add(Arguments* arg, char* pattern) {  // Добавляет паттерн

  // Проверяем на пустоту добавляемый паттерн
  if (pattern == NULL) {
    exit(EXIT_FAILURE);
  }

  int n = strlen(pattern);  // Узнаем длину паттерна

  if (arg->len_pattern == 0) {  // Если общая длина паттернов
    arg->pattern =
        malloc((n + 1) * sizeof(char));  // Выделяем память под первый паттерн

    // Проверка успешности выделения памяти
    if (arg->pattern == NULL) {
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n;
         i++) {  // Записываем паттерн в общую строку паттернов
      arg->pattern[i] = pattern[i];  // Присваиваем соответствующий символ
    }

    arg->len_pattern += n;  // Увеличиваем длину общей строки
  } else {
    arg->pattern =
        realloc(arg->pattern,
                arg->len_pattern +
                    (n + 1));  // Выделяем память под новый паттерн, копируя при
                               // этом старые паттерны в новое место памяти

    // Проверка успешности выделения памяти
    if (arg->pattern == NULL) {
      exit(EXIT_FAILURE);
    }

    arg->pattern[arg->len_pattern] = '|';  // Добавляем разделитель
    arg->len_pattern++;  // Увеличиваем размер строки

    int j = 0;  // Вспомогательная переменная для обхода нового паттерна
    for (int i = arg->len_pattern; i <= arg->len_pattern + n;
         i++) {  // Добавляем в конец прошой строки с паттернами новый паттерн
      arg->pattern[i] = pattern[j];  // Присваиваем соответствующий паттерн
      j++;  // Увеличиваем вспомогательную переменную
    }

    arg->len_pattern +=
        n;  // Увеличиваем длину строки с паттернами на длину нового паттерна
  }
}

void add_reg_from_file(
    Arguments* arg,
    char* filepath) {  // Добавляем регулярные выражения из файлв
  FILE* f = fopen(filepath, "r");  // Открываем файл для чтения
  if (f == NULL) {  // Если усть проблемы с файлом
    if (!arg->s) perror(filepath);  // Если не включён флаг без ошибок
    exit(EXIT_FAILURE);  // Завершаем программу
  }

  char* line = NULL;  // Строка с паттернами
  size_t len = 0;     // Длина строки
  int read = getline(&line, &len, f);  // Считываем строку из файла, read равен
                                       // количеству считанных символов
  while (read != -1) {  // Пока файл не кончился
    if (line[read - 1] == '\n')  // Если предпоследний символ перевод строки
      line[read - 1] = '\0';  // Устанавливаем его как конец строки
    pattern_add(arg, line);  // Добавляем к паттернам строку
    read = getline(&line, &len, f);  // Повторяем процесс снова
  }
  free(line);  // Очищаем строку
  fclose(f);   // Закрываем файл
}

Arguments define_arguments(int argc,
                           char* argv[]) {  // Обрабатываем флаги и паттерны
  int opt = 0;                              // Текущий флаг
  const char options[] = "e:ivclnhsf:o";  // Сбрабатываемые короткие флаги
  Arguments arg = {0, 0, 0, 0, 0,    0,
                   0, 0, 0, 0, NULL, 0};  // Создаём экземпляр структуры

  // Обработка флагов
  while ((opt = getopt(argc, argv, options)) !=
         -1) {  // Записываем результат getopt, пока не кончились флаги
    switch (opt)  // Смотрим какой текущий флаг
    {
      case 'e':
        arg.e = true;  // Устанавливаем соответствующий флаг
        pattern_add(&arg, optarg);  // Устанавливаем паттерн
        break;
      case 'i':
        arg.i = REG_ICASE;  // Устанавливаем режим игнорирования регистра
        break;
      case 'v':
        arg.v = true;  // Устанавливаем соответствующий флаг
        break;
      case 'c':
        arg.c = true;  // Устанавливаем соответствующий флаг
        break;
      case 'l':
        arg.l = true;  // Устанавливаем соответствующий флаг
        break;
      case 'n':
        arg.n = true;  // Устанавливаем соответствующий флаг
        break;
      case 'h':
        arg.h = true;  // Устанавливаем соответствующий флаг
        break;
      case 's':
        arg.s = true;  // Устанавливаем соответствующий флаг
        break;
      case 'f':
        arg.f = true;  // Устанавливаем соответствующий флаг
        add_reg_from_file(&arg,
                          optarg);  // Обрабатываем регулярные выражения с файла
        break;
      case 'o':
        arg.o = true;  // Устанавливаем соответствующий флаг
        break;
      case '?':              // Если что-то не так
        exit(EXIT_FAILURE);  // Завершаем программу
    }
  }

  if (arg.len_pattern == 0) {  // Если количество паттернов равно 0
    pattern_add(&arg, argv[optind]);  // Добавляем паттерн
    optind++;  // Увеличиваем индекс аргумента в считанной строке
  }

  if (argc - optind == 1) {  // Если у нас всего 1 файл
    arg.h = true;  // Не показываем имена файлов
  }

  return arg;  // Возвращаем наши аргументы
}

void process_file(Arguments arg, char* path,
                  regex_t* reg) {  //Обрабатываем файл
  FILE* file = fopen(path, "r");   // Открываем файл

  if (file == NULL) {  // Если возникли проблеммы с файлом
    if (!arg.s)        // Если не установлен флаг -s
      perror(path);      // Выводим ошибку
    exit(EXIT_FAILURE);  // Завершаем программу
  }

  char* line = NULL;   // Текущая строка в файле
  size_t len = 0;      // Длина строки
  int read = 0;        // Результат считывания
  int line_count = 1;  // Количество строк
  int c = 0;           // Количество совпадений

  while ((read = getline(&line, &len, file)) !=
         -1) {  // Построчно считываем файл

    int result = regexec(reg, line, 0, NULL,
                         0);  // Проверяем строку на регулярное выражение

    if ((result == 0 && !arg.v) ||
        (arg.v && result != 0)) {  // Если совпало регулярное выражение
      if (!arg.c && !arg.l) {  // Если не флаг -c и -l
        if (!arg.h)  // Если надо выводить имя файла
          printf("%s:", path);  // Выводим файл
        if (arg.n)  // Если надо выводить номер строки
          printf("%d:", line_count);  // Выводим номер строки
        if (arg.o) {                  // Если флаг -o
          print_match(reg, line);  // Вызываем соответствующую функцию
        } else {
          printf("%s", line);  // Выводим строку
        }
      }
      c++;  // Увеличиваем количество совпаадений
    }

    line_count++;  // Увеличиваем количество строк
  }

  free(line);  // Очищаем строку

  if (arg.c && !arg.i) {  // Если сработал флаг -c и не сработал -i
    if (!arg.h)  // Если не включен режим без вывода имён файлов
      printf("%s:", path);  // Выводим имя файла
    printf("%d\n", c);  // Выводим количество совпадений
  }
  if (arg.l && c > 0)  // Если флаг l и количество совпадений больше 0
    printf("%s\n", path);  // Выводим соответствующий файл

  fclose(file);  // Закрываем файл
}

void output(Arguments arg, int argc,
            char* argv[]) {  // Обрабатываем и выводим файл
  regex_t reg;  // Структура для регулярных выражений
  int error =
      regcomp(&reg, arg.pattern,
              REG_EXTENDED | arg.i);  // Компилируем регулярное выражение
                                      // (учитывая/неучитывая) регистр
  if (error)             // Если возникла ошибка;
    exit(EXIT_FAILURE);  // Завершаем программу

  for (int i = optind; i < argc; i++) {  // Обрабатываем файл за файлом
    process_file(arg, argv[i], &reg);  // Обработка файла
  }
  regfree(&reg);  // Очищаем структуру regex_t
}

void print_match(regex_t* reg, char* line) {  // Выводим совпадения
  regmatch_t match;  // Структура для корректной работы
  int offset = 0;  // Переменная для смещения

  while (1) {  // Смотрим все совпадения
    int result = regexec(reg, line + offset, 1, &match,
                         0);  // Проверяем строку на регулярное выражение

    if (result != 0)  //Если совпадений нет, выходим из цикла
      break;

    for (int i = match.rm_so + offset; i < match.rm_eo + offset;
         i++) {  // От начала совпадения до конца (со сдвигом потому что regexec
                 // ищет совпадения от смещения и индексы начала и конца будут
                 // также смещены)
      putchar(line[i]);  // Выводим соответствующий символ
    }

    putchar('\n');          // Добавляем перевод строки
    offset += match.rm_eo;  // Увеличиваем сдвиг на длину совпавшей подстроки
  }
}

int main(int argc, char* argv[]) {  // Вход в программу
  Arguments arg = define_arguments(argc, argv);  // Задаем флаги и паттерны

  output(arg, argc, argv);  // Обрабатываем и выводим файл
  return 0;
}