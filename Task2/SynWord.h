#pragma once

class SynWord
{
public:
	SynWord(char*, SynWord*, int);
	~SynWord();

	char* word;
	SynWord* canon;
	int id;

private:
	SynWord();
};

