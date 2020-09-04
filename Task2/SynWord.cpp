#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#include "SynWord.h"

SynWord::SynWord(char* _word, SynWord* canonWord, int _id): id(_id)
{
	if (_word != nullptr)
	{
		word = new char[strlen(_word) + 1];
		strcpy(word, _word);
	}
	else
	{
		word = new char[strlen("#error") + 1];
		strcpy(word, "#error");
	}

	if (canonWord != nullptr)
	{
		canon = canonWord;
	}
	else
	{
		canon = nullptr;
	}
}

SynWord::~SynWord()
{
	delete[] word;
}