#include <windows.h>
#include <iostream.h>
#include <stdio.h>

void main()
{
	HANDLE hIn,hOut;
	TCHAR buf[20],str[50];
	DWORD dw;

	hIn=GetStdHandle(STD_INPUT_HANDLE);
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
/*
	printf("input your name:");
	scanf("%20s",buf);
	printf("your name is %s\n",buf);

	cout << "input your name:";
	cin >> buf;
	cout << "your name is " << buf << endl;
//*/

//*
	WriteFile(hOut,"input your name:",16,&dw,NULL);
	memset(buf,0,sizeof(buf));
	ReadFile(hIn,buf,20,&dw,NULL);
	// ReadConsole(hIn,buf,20,&dw,NULL);
	wsprintf(str, "your name is %s",buf);
	WriteFile(hOut,str,lstrlen(str),&dw,NULL);
//*/
}
