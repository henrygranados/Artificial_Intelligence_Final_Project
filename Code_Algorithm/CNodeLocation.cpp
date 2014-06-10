#include <iostream>
#include "Globals.h"
#include "CNodeLocation.h"

CNodeLocation NULLLOCATION;

void CNodeLocation::set (int j, int i)
{
	y = j;
	x = i;
}

char* CNodeLocation::print (void) 
{
	static char	str[16];

	snprintf  (str, 16, "(%02d, %02d)", y, x);
	return str;
}
