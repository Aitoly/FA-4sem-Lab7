#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <iostream>

#include "SynWord.h"
#include "SynDictionary.h"

SynDictionary::SynDictionary()
{}

SynDictionary::~SynDictionary()
{
	int size = dict.size();

	for (int i = 0; i < size; ++i)
	{
		delete dict[i];
	}
}

bool IsDigit(char c)
{
	return c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0';
}

// LOAD & SAVE

bool SynDictionary::LoadDict(const char* file)
{
	FILE* f;

	f = fopen(file, "r");
	if (!f)
	{
		return false;
	}

	char buff[BUFSIZ];
	char* ptr = 0;
	SynWord* canonWord = nullptr;

	while (!feof(f))
	{
		char c = ' ';
		c = getc(f);
		ptr = buff;

		while (!feof(f) && c != '\n' && c != ' ' && c != '\t')
		{
			*ptr = c;
			++ptr;
			c = getc(f);
		}
		*ptr = '\0';

		switch (c)
		{
		case '\n':
			add(buff, canonWord);
			canonWord = nullptr;
			break;

		case ' ':
			add(buff, canonWord);
			break;

		case '\t':
			canonWord = add(buff, canonWord);
			break;

		default:
			add(buff, canonWord);
			canonWord = nullptr;
			break;
		}
	}

	return true;
}

bool SynDictionary::SaveDict(const char* file)
{
	FILE* f;

	f = fopen(file, "w");
	if (!f)
	{
		return false;
	}

	int size = dict.size();

	for (int i = 0; i < size; ++i)
	{
		if (!dict[i]->canon)
		{
			fprintf(f, "\n%s\t", dict[i]->word);
			for (int j = 0; j < size; ++j)
			{
				if(dict[j]->canon && dict[j]->canon->id == i)
					fprintf(f, "%s ", dict[j]->word);
			}
			
		}
	}
}

//	PUBLIC

int SynDictionary::GetCanonId(int id)
{
	if (id == -1)
		return -1;

	if (dict[id]->canon)
	{
		return dict[id]->canon->id;
	}
	else
	{
		return id;
	}
}

int SynDictionary::FindWord(char* word)
{
	int size = dict.size();

	for (int i = 0; i < size; ++i)
	{
		if (!strcmp(word, dict[i]->word))
		{
			return i;
		}
	}

	return -1;
}

int SynDictionary::FindStartWord(char* word)
{
	if (!word || strlen(word) < 1)
		return -1;

	int size = dict.size();

	for (int i = 0; i < size; ++i)
	{
		if (strlen(word) <= strlen(dict[i]->word))
		{
			char* dictW = new char[strlen(dict[i]->word) + 1];
			strcpy(dictW, dict[i]->word);
			dictW[strlen(word)] = '\0';
			if (!strcmp(word, dictW))
			{
				return i;
			}
		}
	}

	return -1;
}

SynWord* SynDictionary::GetById(int i)
{
	if (i >= 0 && i < dict.size())
		return dict[i];
	return nullptr;
}

void SynDictionary::ReplaceCanon(char* newCanon, int oldCanon)
{
	SynWord* toAdd = new SynWord(newCanon, nullptr, dict.size());
	dict.push_back(toAdd);
	
	SynWord* oldCanon_ = GetById(oldCanon);

	int size = dict.size();
	for (int i = 0; i < size; ++i)
	{
		if (i == oldCanon || dict[i]->canon == oldCanon_)
		{
			dict[i]->canon = toAdd;
		}
	}
}

// PRIVATE

SynWord* SynDictionary::add(char* _word, SynWord* canonWord)
{
	if (strlen(_word) > 0)
	{
		SynWord* toAdd = new SynWord(_word, canonWord, dict.size());
		dict.push_back(toAdd);
		return toAdd;
	}
	return nullptr;
}