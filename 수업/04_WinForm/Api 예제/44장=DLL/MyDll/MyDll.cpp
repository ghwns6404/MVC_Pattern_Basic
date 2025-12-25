/* 직접 엑스포트하기
extern "C" __declspec(dllexport) int AddInteger(int a, int b)
{
	return a+b;
}
//*/

//* 헤더 파일로 엑스포트하기
#define DLLEXPORT
#include "MyDll.h"

extern "C" MYDLLTYPE int AddInteger(int a, int b)
{
	return a+b;
}
//*/
