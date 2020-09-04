#pragma once

#include "Doc.h"

class Employee
{
public:
	Employee(char*);
	~Employee();

	bool operator==(const Employee&);
	bool operator<(const Employee&);

	const char* GetName() const;
	void addDoc(int, int, int, int, int, int, int, int, int, bool);
	
	void PrintDocs();

	int costOfDocs();

	int costMax();
	void costMax(int&);

	int longMax();
	void longMax(int&);

private:
	Employee();

	char* fio;
	std::vector<Doc*> docs;
};


