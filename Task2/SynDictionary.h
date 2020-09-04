#pragma once

#include "SynWord.h"

class SynDictionary
{
public:
	SynDictionary();
	~SynDictionary();

	bool LoadDict(const char*);
	bool SaveDict(const char*);

	int FindWord(char*);
	int FindStartWord(char*);
	int GetCanonId(int);
	SynWord* GetById(int);
	void ReplaceCanon(char*, int);
	
	SynWord* add(char*, SynWord*);

private:
	std::vector<SynWord*> dict;
	
};

