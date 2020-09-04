#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include<string>
#include<Windows.h>

#include "SynDictionary.h"

using namespace std;

//////////// Global vars
FILE* in;
FILE* out;
FILE* log_out;
//


bool isEnd(char c)
{
    return c == ' ' || c == ',' || c == ':' || c == '.' || c == '\n' || c == '!' || c == '?' || c == '«' || c == '»' ||
        c == ';' || c == '\t';
}

char* createFileOutName(char* fileInName)
{
    char* fileOutName = new char[strlen(fileInName) + 5];
    strcpy(fileOutName, fileInName);
    char* ptr = fileOutName + strlen(fileInName) - 4;

    *ptr++ = '-';
    *ptr++ = 'o';
    *ptr++ = 'u';
    *ptr++ = 't';

    *ptr++ = '.';
    *ptr++ = 't';
    *ptr++ = 'x';
    *ptr++ = 't';

    *ptr = '\0';

    return fileOutName;
}

char* addToBuff(char* buff, char c)
{
    if (c == '\n')
        return buff;

    if (buff == nullptr)
    {
        char* newBuff = new char[2];
        newBuff[0] = c;
        newBuff[1] = '\0';
        return newBuff;
    }

    char* newBuff = new char[strlen(buff) + 2];
    strcpy(newBuff, buff);

    int i = strlen(newBuff);
    newBuff[i] = c;
    newBuff[i+1] = '\0';

    delete[] buff;
    return newBuff;
}

void LearnWord(SynDictionary& dict, char* buff, char c, bool isAuto)
{
    if (isAuto)
    {
        fprintf(out, "%s%c", buff, c);
        fprintf(log_out, "%s\n", buff);
    }
    else
    {
        char* canonBuff = new char[1];
        *canonBuff = '\0';
        bool done = false;
        while (!done)
        {
            int enterId = dict.FindStartWord(canonBuff);
            int canonId = dict.GetCanonId(enterId);

            system("cls");
            printf("Найденно слово \"%s\", которое отстутвует в словаре синонимов. ", buff);
            printf("Введите к какому слову оно может являться синонимом:\n");
            printf("\t#!!!# Вводите буквы по одной!\n");
            printf("\t#!!!# Для очистки ввода введите 0\n");
            printf("\t#!!!# Если вы согласны с подсказкой введите 1 (слово присоединится к канонической форме)\n");
            printf("\t#!!!# Если вы согласны с подсказкой введите 2 (слово заменит каноническую форму)\n");
            printf("\t#!!!# Если вы хотите добить это слово как новое введите 3\n");
            if (enterId == -1)
            {
                printf("Совпадений не найденно\n");
            }
            else
            {
                //printf("Возможно вы имели ввиду: %s\n", dict.GetById(enterId)->word);

                printf("Возможно вы имели ввиду: %s (Каноническая форма: %s)\n", dict.GetById(enterId)->word, dict.GetById(canonId)->word);
            }

            printf("Вводимое слово: %s_", canonBuff);


            char ch = ' ';
            scanf_s("%c", &ch);

            switch (ch)
            {
            case '0':
                delete[] canonBuff;
                canonBuff = new char[1];
                *canonBuff = '\0';
                break;
            case '1':
                if (enterId != -1)
                {
                    dict.add(buff, dict.GetById(canonId));
                    done = true;
                    fprintf(out, "%s%c", dict.GetById(canonId)->word, c);
                }
                break;
            case '2':
                if (canonId != -1)
                {
                    dict.ReplaceCanon(buff, canonId);
                    done = true;
                    fprintf(out, "%s%c", buff, c);
                }
                break;
            case '3':
                dict.add(buff, nullptr);
                done = true;
                fprintf(out, "%s%c", buff, c);
                break;
            default:
                canonBuff = addToBuff(canonBuff, ch);
                break;
            }
        }

        delete[] canonBuff;
    }
}







int main(int argc, char** argv)
{
    if (argc < 2)
    {
        return 0;
    }

    const char* dictFileName = "syn--.txt";

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    SynDictionary dict;
    if (!dict.LoadDict(dictFileName))
    {
        exit(3);
    }



    for (int fileId = 1; fileId < argc; ++fileId)
    {

        in = fopen(argv[fileId], "r");
        if (!in)
        {
            printf("Error! File %s not founded!\n", argv[fileId]);
            continue;
        }

        char* fileOutName = createFileOutName(argv[fileId]);
        out = fopen(fileOutName, "w");
        if (!out)
        {
            fclose(in);
            exit(2);
        }
        delete[] fileOutName;

        log_out = fopen("bad-words.txt", "w");
        if (!log_out)
        {
            fclose(in);
            fclose(out);
            exit(3);
        }

        char buff[BUFSIZ];
        char* startWord = nullptr;
        char* endWord = nullptr;

        char c = ' ';
        char* ptr = buff;

        bool isAuto = true;
        {
            char c;
            printf("Начать в обучающем режиме?(при отрицальном ответе будет выбран автоматический режим).\n");
            scanf_s("%c", &c);
            if (c == 'y')
            {
                isAuto = false;
            }
            else
            {
                isAuto = true;
            }
        }

        while (!feof(in))
        {
            //INPUT
            c = getc(in);

            while (!feof(in))
            {
                if (isEnd(c))
                {
                    // Start work

                    *ptr = '\0';
                    int id = dict.FindWord(buff);

                    if (id != -1)
                    {
                        SynWord* canonWord = dict.GetById(id)->canon;

                        if (canonWord)
                            fprintf(out, "%s%c", canonWord->word, c);
                        else
                            fprintf(out, "%s%c", buff, c);
                    }
                    else
                    {
                        LearnWord(dict, buff, c, isAuto);
                    }

                    ptr = buff;
                    *buff = '\0';
                    c = getc(in);

                    // End work
                }
                else
                {
                    *ptr = c;
                    ++ptr;
                    c = getc(in);
                }
            }

        }// End of wile
        *ptr = '\0';

        if (strlen(buff) > 0)
        {
            int id = dict.FindWord(buff);

            if (id != -1)
            {
                SynWord* canonWord = dict.GetById(id)->canon;

                if (canonWord)
                    fprintf(out, "%s%c", canonWord->word, c);
                else
                    fprintf(out, "%s%c", buff, c);
            }
            else
            {
                LearnWord(dict, buff, c, isAuto);
            }
        }   


        fclose(in);
        fclose(out);
        fclose(log_out);

        dict.SaveDict(dictFileName);
    }

    return 0;
}