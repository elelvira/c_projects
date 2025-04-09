#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_WORD 1001
#define MAX_ARGS 10



// Функция для приведения слова к нижнему регистру
void toLowercase(char *word) {
    for (int i = 0; word[i] != '\0'; ++i) {
        word[i] = tolower(word[i]);
    }
}

void processText(char *buffer, char *replaceString, char setN[][MAX_WORD], int setSize, int caseSensitive) {
    char word[MAX_WORD] = { 0 };    // Поле для слова

    for (int i = 0; buffer[i] != '\0'; ++i) {
        if (isalpha(buffer[i])) {
            int startWord = i;

            while (isalpha(buffer[i])) {
                word[i - startWord] = buffer[i];
                ++i;
            }
            word[i - startWord] = '\0';

            int isPrefix = 0;  // Флаг, 0 - префикс не существует, иначе 1

            // Контроль, содержится ли префикс в слове
            if (setSize != 0) {
                if (!caseSensitive) { // Если режим "-R", приводим слово и префикс к нижнему регистру
                    toLowercase(word);
                }
                for (int j = 0; j < setSize; ++j) {
                    char prefixCopy[MAX_WORD];
                    strcpy(prefixCopy, setN[j]);
                    if (!caseSensitive) { // Если режим "-R", приводим префикс к нижнему регистру
                        toLowercase(prefixCopy);
                    }
                    if (strstr(word, prefixCopy) == word) { // Попытка найти префикс в начале слова
                        isPrefix = 1;
                        break;
                    }
                }
            } else {
                isPrefix = 1;  // Если префиксы отсутствуют, устанавливаем флаг в 1
            }

            // Замена слова строкой replaceString, если префикс найден
            if (isPrefix) {
                for (int j = startWord, k = 0; j < i && replaceString[k] != '\0'; ++j, ++k) {
                    buffer[j] = replaceString[k];
                }
            }
        }
    }
    printf("%s", buffer);
}


// Функция для удаления цифр и знаков пунктуации из входного текста
void removeText(char *buffer, int removeDigitsFlag, int removePunctuationFlag) {
    for (int i = 0; buffer[i] != '\0'; ++i) {
        if (!removeDigitsFlag && !removePunctuationFlag) {
            printf("%c", buffer[i]); // Нет фильтров, просто выводим символы
        } else if (removeDigitsFlag && isdigit(buffer[i])) {
            continue; // Игнорируем цифры, если флаг удаления цифр установлен
        } else if (removePunctuationFlag && ispunct(buffer[i])) {
            continue; // Игнорируем знаки пунктуации, если флаг удаления пунктуации установлен
        } else {
            printf("%c", buffer[i]); // Выводим символы, которые не являются цифрами или знаками пунктуации
        }
    }
}

void processTextWithSplit(char *buffer, int lineNumber) {
    char word[MAX_WORD] = { 0 };    // Поле для слова
    int wordCount = lineNumber; // Используем номер строки как номер слова

    for (int i = 0; buffer[i] != '\0'; ++i) {
        if (isalpha(buffer[i])) {
            int startWord = i;

            // Извлекаем слово из строки buffer
            while (isalpha(buffer[i])) {
                word[i - startWord] = buffer[i];
                ++i;
            }
            word[i - startWord] = '\0';



            // Выводим слово построчно с указанием номера строки
            printf("%d. %-10s:%s\n", lineNumber, word, word);
            wordCount++; // Увеличиваем номер слова
        }
    }
}


int main(int argc, char *argv[]) {
    int register_mode_on = -1; // Режим по умолчанию "-r" (с учетом регистра)
    int removeDigitsFlag = 0;
    int removePunctuationFlag = 0;
    int s_rezim=0;


    int parameters_counter = 1;
    // Проверяем наличие режимов -r и -R
    while (parameters_counter < argc) {
        if (strcmp(argv[parameters_counter], "-r") == 0 || strcmp(argv[parameters_counter], "-R") == 0) {
            register_mode_on = (strcmp(argv[parameters_counter], "-r") == 0) ? 1 : 0; // Режим "-R" (без учета регистра)
            parameters_counter++; // Переходим к следующему аргументу
        } else {
            break; // Завершаем цикл, если не найден режим "-r" или "-R"
        }
    }

    // Проверяем наличие флагов -d и -p
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-d") == 0) {
            removeDigitsFlag = 1;
        } else if (strcmp(argv[i], "-p") == 0) {
            removePunctuationFlag = 1;
        } else if (strcmp(argv[i], "-s") == 0){
            s_rezim=1;
        }
    }

    char input[MAX_WORD];
    int lineNumber = 1;

    // Если включен режим -r или -R, получаем соответствующие аргументы
    char replace_string[MAX_WORD] = { 0 };  // Строка для замены
    char setN[MAX_ARGS][MAX_WORD];  // Массив для префиксов
    int setSize = 0;    // Размер множества префиксов

    if (register_mode_on != -1) {
        strcpy(replace_string, argv[parameters_counter]); // Копируем строку замены из аргументов командной строки
        parameters_counter++; // Переходим к следующим аргументам

        // Сохраняем префиксы в массив setN
        int index = 0;  // Индекс для массива слов
        while (parameters_counter < argc) {
            strcpy(setN[index++], argv[parameters_counter++]); // Копируем префиксы
        }
        setSize = index;    // Размер множества префиксов
    }

    // Считываем ввод с клавиатуры в цикле до получения команды для завершения

    while (fgets(input, MAX_WORD, stdin) != NULL) {
        // Проверяем, не введена ли команда для выхода
        if (strcmp(input, "\n") == 0) {
            break;
        }

        // Обработка текста с учетом установленных параметров
        else if (register_mode_on != -1) {
            processText(input, replace_string, setN, setSize, register_mode_on);
        }
        else if (s_rezim==1){
            processTextWithSplit(input, lineNumber);
            lineNumber++;

        }
        else if((removeDigitsFlag || removePunctuationFlag) || (removeDigitsFlag && removePunctuationFlag) || (!removeDigitsFlag && !removePunctuationFlag)){
            removeText(input, removeDigitsFlag, removePunctuationFlag);
        }

    }

    return 0;
}
