#include <windows.h>

#pragma data_seg(".ksh")
__declspec(dllexport) TCHAR buf[1024]={0,};
#pragma data_seg()
#pragma comment (linker, "/SECTION:.ksh,RWS")
