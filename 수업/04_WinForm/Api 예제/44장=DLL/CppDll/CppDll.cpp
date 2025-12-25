#include <stdio.h>
#define DLLEXPORT
#include "CppDll.h"

void CInt::Inc()
{
	i++;
}

void CInt::Dec()
{
	i--;
}

int CInt::GetValue() const 
{ 
	return i; 
}

void CInt::SetValue(int ai) 
{ 
	i=ai; 
}

