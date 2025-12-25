#include <windows.h>
#include <stdio.h>

void main()
{
	HANDLE hIn,hOut;
	DWORD dw;
	INPUT_RECORD rec;

	hIn=GetStdHandle(STD_INPUT_HANDLE);
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);

	while (TRUE) {
		ReadConsoleInput(hIn,&rec,1,&dw);

		if (rec.EventType == KEY_EVENT && rec.Event.KeyEvent.bKeyDown == TRUE) {
			printf("%c",rec.Event.KeyEvent.uChar.AsciiChar);
		}

		if (rec.EventType == MOUSE_EVENT) {
			if ((rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) 
				&& (rec.Event.MouseEvent.dwEventFlags & MOUSE_MOVED)) {
				SetConsoleCursorPosition(hOut, rec.Event.MouseEvent.dwMousePosition);
				printf(".");
			}

			if (rec.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK) {
				break;
			}
		}
	}
}
