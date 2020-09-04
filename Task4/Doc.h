#pragma once

#include "DateClass.h"

class Doc
{
public:
	Doc(int,int, int, int,int, int, int,int,int, bool);
	~Doc();

	int numberOfDoc;
	DateClass start;
	DateClass end;
	int numberOfWork;
	int money;
	bool finished;

private:
	Doc();
};

