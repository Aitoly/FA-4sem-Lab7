#include "DateClass.h"

#include "Doc.h"

Doc::Doc(int _numberOfDoc,
	int start_d, int start_m, int start_y,
	int end_d, int end_m, int end_y,
	int _numberOfWork,
	int _money, 
	bool _finished)
	: numberOfDoc(_numberOfDoc), start(start_d, start_m, start_y), end(end_d, end_m, end_y), 
	numberOfWork(_numberOfWork), money(_money), finished(_finished)
{
}

Doc::~Doc()
{}
