#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include<Windows.h>

#include "Doc.h"

#include "Employee.h"

Employee::Employee(char* _fio)
{
	if (_fio)
	{
		fio = new char[strlen(_fio) + 1];
		strcpy(fio, _fio);
	}
	else
	{
		fio = nullptr;
	}
}

Employee::~Employee()
{
	delete[] fio;

	int size = docs.size();
	for (int i = 0; i < size; ++i)
	{
		delete docs[i];
	}
}

const char* Employee::GetName() const
{
	return fio;
}

void Employee::addDoc(int _numberOfDoc, int start_d, int start_m, int start_y, int end_d, int end_m, int end_y,
	int _numberOfWork, int _money, bool _finished)
{
	docs.push_back(new Doc(_numberOfDoc, start_d, start_m, start_y, end_d, end_m, end_y, _numberOfWork, _money, _finished));
}

void Employee::PrintDocs()
{
	int size = docs.size();
	for (int i = 0; i < size; ++i)
	{
		if (docs[i]->finished)
		{
			printf("Договор №%d нач. %d.%d.%d кон. %d.%d.%d Работа %d Стоимость %d;\n",
				docs[i]->numberOfDoc, 
				docs[i]->start.d, docs[i]->start.m, docs[i]->start.y,
				docs[i]->end.d, docs[i]->end.m, docs[i]->end.y,
				docs[i]->numberOfWork,
				docs[i]->money);
		}
		else
		{
			printf("Договор №%d нач. %d.%d.%d кон. нв Работа %d Стоимость %d;\n",
				docs[i]->numberOfDoc,
				docs[i]->start.d, docs[i]->start.m, docs[i]->start.y,
				docs[i]->numberOfWork,
				docs[i]->money);
		}
	}
}

int Employee::costOfDocs()
{
	int cost = 0;
	int size = docs.size();

	for (int i = 0; i < size; ++i)
	{
		cost += docs[i]->money;
	}

	return cost;
}

int Employee::costMax()
{
	int cost = 0;
	int size = docs.size();

	for (int i = 0; i < size; ++i)
	{
		if(docs[i]->money > cost)
		cost = docs[i]->money;
	}

	return cost;
}

void Employee::costMax(int& max)
{
	int size = docs.size();

	for (int i = 0; i < size; ++i)
	{
		if (docs[i]->money > max)
			max = docs[i]->money;
	}
}

int Employee::longMax()
{
	int longest = 0;
	int size = docs.size();
	unsigned long long day = 864000000000;

	for (int i = 0; i < size; ++i)
	{
		unsigned long long start = 0;
		unsigned long long end = 0;
		SYSTEMTIME data;
		
		////	START
		data.wDay = docs[i]->start.d;
		data.wMonth = docs[i]->start.m;
		data.wYear = docs[i]->start.y;
		data.wHour = 0;
		data.wMinute = 0;
		data.wSecond = 0;
		data.wMilliseconds = 0;

		{
			FILETIME f;
			SystemTimeToFileTime(&data, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			start = lv_Large.QuadPart;
		}

		////	END
		data.wDay = docs[i]->end.d;
		data.wMonth = docs[i]->end.m;
		data.wYear = docs[i]->end.y;
		data.wHour = 0;
		data.wMinute = 0;
		data.wSecond = 0;
		data.wMilliseconds = 0;

		{
			FILETIME f;
			SystemTimeToFileTime(&data, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			end = lv_Large.QuadPart;
		}

		unsigned long long time = (end - start) / day;
		int timeInt = (int)time;

		if (timeInt > longest)
			longest = timeInt;
	}

	return longest;
}

void Employee::longMax(int& max)
{
	int size = docs.size();
	unsigned long long day = 864000000000;

	for (int i = 0; i < size; ++i)
	{
		unsigned long long start = 0;
		unsigned long long end = 0;
		SYSTEMTIME data;

		////	START
		data.wDay = docs[i]->start.d;
		data.wMonth = docs[i]->start.m;
		data.wYear = docs[i]->start.y;
		data.wHour = 0;
		data.wMinute = 0;
		data.wSecond = 0;
		data.wMilliseconds = 0;

		{
			FILETIME f;
			SystemTimeToFileTime(&data, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			start = lv_Large.QuadPart;
		}

		////	END
		data.wDay = docs[i]->end.d;
		data.wMonth = docs[i]->end.m;
		data.wYear = docs[i]->end.y;
		data.wHour = 0;
		data.wMinute = 0;
		data.wSecond = 0;
		data.wMilliseconds = 0;

		{
			FILETIME f;
			SystemTimeToFileTime(&data, &f);
			ULARGE_INTEGER    lv_Large;
			lv_Large.LowPart = f.dwLowDateTime;
			lv_Large.HighPart = f.dwHighDateTime;

			end = lv_Large.QuadPart;
		}

		unsigned long long time = (end - start) / day;
		int timeInt = (int)time;

		if (timeInt > max)
			max = timeInt;
	}
}







bool Employee::operator<(const Employee& second)
{
	if (!fio && !second.fio)
		return false;

	if (!fio)
		return true;

	if (!second.fio)
		return false;

	return (strcmp(fio, second.fio) < 0);
}

bool Employee::operator==(const Employee& second)
{
	if (!fio && !second.fio)
		return true;

	if (!fio)
		return false;

	if (!second.fio)
		return false;

	return !strcmp(fio, second.fio);
}