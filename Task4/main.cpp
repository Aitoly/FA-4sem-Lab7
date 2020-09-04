#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include<Windows.h>
#include <vector>
#include <cstdio>

#include "RedBlackTree.h"


void GetString(FILE* f, char* buff, int maxSize)
{
    if (!f || !buff)
        return;

    *buff = '\0';
    int count = 0;
    char c = ' ';

    c = getc(f);

    while (!feof(f) && c != '\n' && count < (maxSize - 2))
    {
        *buff++ = c;
        ++count;
        c = getc(f);
    }

    *buff = '\0';
}

std::vector<char*> GetWords(char* buff)
{
    std::vector<char*> words;

    if (!buff)
        return words;

    int i = 0;
    int maxI = strlen(buff);
    char locBuff[BUFSIZ];
    char* ptr = locBuff;
    *ptr = '\0';

    for (; i <= maxI; ++i)
    {
        switch (buff[i])
        {
        case '\0':
        case ' ':
        case '.':
            *ptr = '\0';
            if (strlen(locBuff) > 0)
            {
                char* word = nullptr;
                word = new char[strlen(locBuff) + 1];
                strcpy(word, locBuff);
                words.push_back(word);
                ptr = locBuff;
                *ptr = '\0';
            }
            break;
        case '№':
            break;
        case ';':
            break;
        default:
            *ptr++ = buff[i];
            break;
        }
    }

    return words;
}

bool isDgt(char c)
{
    if (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' ||
        c == '8' || c == '9')
        return true;
    return false;
}

bool isNum(char* buff)
{
    if (!buff)
        return false;

    int size = strlen(buff);
    for (int i = 0; i < size; ++i)
    {
        if(!isDgt(buff[i]))
            return false;
    }

    return true;
}

bool CreateAndAddDoc(char* buff, Employee* employee)
{
    if (!buff || !employee)
        return false;

    std::vector<char*> words = GetWords(buff);

    int numberOfDoc;
    int start_d, start_m, start_y;
    int end_d, end_m, end_y;
    int numberOfWork;
    int money;
    bool finished;

    int size = words.size();

    if (strcmp(words[0], "Договор"))
        return false;

    if (isNum(words[1]))//номер договора
    {
        numberOfDoc = atoi(words[1]);
    }
    else
    {
        return false;
    }

    if (strcmp(words[2], "нач"))
        return false;

    if (isNum(words[3]))//дата начала
    {
        start_d = atoi(words[3]);
    }
    else
    {
        return false;
    }

    if (isNum(words[4]))//месяц начала
    {
        start_m = atoi(words[4]);
    }
    else
    {
        return false;
    }

    if (isNum(words[5]))//год начала
    {
        start_y = atoi(words[5]);
    }
    else
    {
        return false;
    }

    if (strcmp(words[6], "кон"))
        return false;

    if (strcmp(words[7], "нв"))///договор закончен
    {
        finished = true;

        if (isNum(words[7]))//дата конца
        {
            end_d = atoi(words[7]);
        }
        else
        {
            return false;
        }

        if (isNum(words[8]))//месяц конца
        {
            end_m = atoi(words[8]);
        }
        else
        {
            return false;
        }

        if (isNum(words[9]))//год конца
        {
            end_y = atoi(words[9]);
        }
        else
        {
            return false;
        }

        if (strcmp(words[10], "Работа"))
            return false;

        if (isNum(words[11]))//номер работы
        {
            numberOfWork = atoi(words[11]);
        }
        else
        {
            return false;
        }

        if (strcmp(words[12], "Стоимость"))
            return false;

        if (isNum(words[13]))//Стоимость
        {
            money = atoi(words[13]);
        }
        else
        {
            return false;
        }
    }
    else///договор НЕ закончен
    {
        finished = false;

        SYSTEMTIME nowData;
        GetLocalTime(&nowData);
        end_d = nowData.wDay;
        end_m = nowData.wMonth;
        end_y = nowData.wYear;

        if (strcmp(words[8], "Работа"))
            return false;

        if (isNum(words[9]))//номер работы
        {
            numberOfWork = atoi(words[9]);
        }
        else
        {
            return false;
        }

        if (strcmp(words[10], "Стоимость"))
            return false;

        if (isNum(words[11]))//Стоимость
        {
            money = atoi(words[11]);
        }
        else
        {
            return false;
        }
    }

    employee->addDoc(numberOfDoc, start_d, start_m, start_y, end_d, end_m, end_y, numberOfWork, money, finished);

    return true;
}

enum AlreadyGet { NO, FIO, OPEN };


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    RedBlackTree tree;

    FILE* in;
    AlreadyGet alreadyGet = NO;
    char buff[BUFSIZ];
    Employee* emp = nullptr;

    //////////// Read

    in = fopen("in.txt", "r");
    if (!in)
    {
        exit(1);
    }

    

    while (!feof(in))
    {
        GetString(in, buff, BUFSIZ);

        if (strlen(buff) > 0)
        {
            if (!strcmp(buff, "{"))
            {
                if (alreadyGet == FIO)
                {
                    alreadyGet = OPEN;
                }
                else
                {
                    //error
                }
            }
            else if (!strcmp(buff, "}"))
            {
                if (alreadyGet == OPEN && emp)
                {
                    if ((tree.findNode(emp)) != NULL)
                    {
                        printf("Insert error!!! %s is founded!\n", emp->GetName());
                        delete emp;//
                    }
                    else
                    {
                        tree.insertNode(emp);
                        printf("%s is inserted!\n", emp->GetName());
                    }

                    emp = nullptr;
                    alreadyGet = NO;
                }
                else
                {
                    //error
                }
            }
            else
            {
                switch (alreadyGet)
                {
                case NO:
                    emp = new Employee(buff);
                    alreadyGet = FIO;
                    break;
                case FIO:
                    //error
                    break;
                case OPEN:
                    if (emp)
                    {
                        if (!CreateAndAddDoc(buff, emp))
                        {
                            //error
                        }
                    }
                    else
                    {
                        //error
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

    fclose(in);

    ////// Work


    char cmd[BUFSIZ];
    emp = nullptr;
    RedBlackNode* nodeEmp = nullptr;
    printf("[пусто::][помощь] - узнать список комманд\n");

    do
    {
        if (emp)
        {
            printf("%s::", emp->GetName());
        }
        else
        {
            printf("пусто::");
        }

        
        gets_s(cmd);
        if (!strcmp(cmd, "помощь"))
        {
            printf("\t[пусто::][помощь] - узнать список команд\n");
            printf("\t[пусто::][искать] - найти и выбрать сотрудника\n");
            printf("\t[пусто::][больший] - самый продолжительный договор\n");
            printf("\t[сотрудник::][самыйдорогой] - самый дорогой договор\n");

            printf("\t[сотрудник::][список] - список договоров, с которыми работал сотрудник\n");
            printf("\t[сотрудник::][стоимость] - стоимость всех договоров\n");
            printf("\t[сотрудник::][больший] - самый продолжительный договор сотрудника\n");
            printf("\t[сотрудник::][самыйдорогой] - самый дорогой договор сотрудника\n");
            printf("\t[сотрудник::][удалить] - удалить сотрудника\n");
            printf("\t[сотрудник::][назад] - работать со всеми сотрудниками\n");
        }
        else if (!strcmp(cmd, "искать"))
        {
            if (emp)
            {
                printf("Сотрудник уже выбран! Вернитесь назад с помощью [назад].\n");
            }
            else
            {
                printf("Введите ФИО сотрудника: ");
                gets_s(cmd);
                Employee* findEmp = new Employee(cmd);
                nodeEmp = tree.findNode(findEmp);
                if (!nodeEmp)
                    printf("Такой сотрудник не найден!\n");
                else
                    emp = nodeEmp->data;
            }
        }
        else if (!strcmp(cmd, "список"))
        {
            if (emp)
            {
                emp->PrintDocs();
            }
            else
                printf("Сотрудник не выбран! Выберете сотрудника с помощью [искать].\n");
        }
        else if (!strcmp(cmd, "стоимость"))
        {
            if (emp)
            {
                printf("Суммарная стоимость договров сотрудника %s: %d.\n", emp->GetName(), emp->costOfDocs());
            }
            else
                printf("Сотрудник не выбран! Выберете сотрудника с помощью [искать].\n");
        }
        else if (!strcmp(cmd, "больший"))
        {
            if (emp)
            {
                printf("Максимальная продолжительность договра сотрудника %s: %d дней.\n", emp->GetName(), emp->longMax());
            }
            else
            {
                printf("Максимальная продолжительность договра: %d дней.\n", tree.longMax());
            }
        }
        else if (!strcmp(cmd, "самыйдорогой"))
        {
            if (emp)
            {
                printf("Максимальная стоимость договра сотрудника %s: %d.\n", emp->GetName(), emp->costMax());
            }
            else
            {
                printf("Максимальная стоимость договра: %d.\n", tree.costMax());
            }
        }
        else if (!strcmp(cmd, "удалить"))
        {
            if (emp)
            {
                tree.deleteNode(nodeEmp);
                emp = nullptr;
                nodeEmp = nullptr;
            }
            else
                printf("Сотрудник не выбран! Выберете сотрудника с помощью [искать].\n");
        }
        else if (!strcmp(cmd, "назад"))
        {
            if (emp)
            {
                emp = nullptr;
                nodeEmp = nullptr;
            }
            else
                printf("Сотрудник не выбран! Выберете сотрудника с помощью [искать].\n");
        }
        else if (!strcmp(cmd, "стоп"))
        {
            printf("Заввершение работы\n");
        }
        else
        {
            printf("Неизвестная комманда\n");
        }

    } while (strcmp(cmd, "стоп"));


}