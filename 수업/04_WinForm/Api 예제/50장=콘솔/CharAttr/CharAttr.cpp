#include <windows.h>

void main()
{
	HANDLE hOut;
	TCHAR *str="Console Test";
	COORD Size,Coor;
	DWORD dw;

	hOut=GetStdHandle(STD_OUTPUT_HANDLE);

	// 색상있는 문자를 임의의 위치에 출력
	WORD Attr[]={0x91,0xa2,0x74,0x24,0x08,0x09,0x34,0xff,0xd1,0xe1,0x0d,0x75};
	Coor.X=50;
	Coor.Y=10;
	WriteConsoleOutputCharacter(hOut,str,lstrlen(str),Coor,&dw);
	WriteConsoleOutputAttribute(hOut,Attr,lstrlen(str),Coor,&dw);

	// 문자와 속성을 한꺼번에 출력
	SMALL_RECT rt={50,12,55,12};
	Size.X=5;
	Size.Y=1;
	Coor.X=0;
	Coor.Y=0;
	CHAR_INFO arCh[5]={{'K',0x07},{'O',0x70},{'R',0x93},{'E',0x67},{'A',0xe8}};
	WriteConsoleOutput(hOut,arCh,Size,Coor,&rt);

	// 채움
	Coor.X=5;
	Coor.Y=14;
	FillConsoleOutputCharacter(hOut,'*',10*5,Coor,&dw);
	FillConsoleOutputAttribute(hOut,0xe4,10*5,Coor,&dw);
}