#include <windows.h>

BOOL Handler(DWORD dwEvent)
{
	switch (dwEvent) {
	case CTRL_C_EVENT:
		return TRUE;
	case CTRL_BREAK_EVENT:
		return FALSE;
	}
	return FALSE;
}

void main()
{
	HANDLE hIn,hOut;
	TCHAR str[11];
	TCHAR *Mes="Input Success";
	DWORD dw;

	hIn=GetStdHandle(STD_INPUT_HANDLE);
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)Handler,TRUE);

	memset(str,0,sizeof(str));
	ReadFile(hIn,str,10,&dw,NULL);
	WriteFile(hOut,Mes,lstrlen(Mes),&dw,NULL);
}
