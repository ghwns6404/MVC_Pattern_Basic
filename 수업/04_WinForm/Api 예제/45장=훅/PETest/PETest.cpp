#include <windows.h>
#include "resource.h"

unsigned Global=0x12345678;
TCHAR str[128];
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	LoadString(hInstance,IDS_STRING1,str,128);
	MessageBox(NULL,str,TEXT("PETest Message"),MB_OK);
	Global++;
	return 0;
}
