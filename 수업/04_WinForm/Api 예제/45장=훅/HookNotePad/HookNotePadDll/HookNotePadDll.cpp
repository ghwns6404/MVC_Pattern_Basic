#include <windows.h>

#pragma data_seg(".npdata")
HINSTANCE hModule=NULL;
HHOOK hKeyHook=NULL;
HWND hWndBeeper=NULL;
#pragma data_seg()
#pragma comment (linker, "/SECTION:.npdata,RWS")

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode>=0) {
		SendMessage(hWndBeeper,WM_USER+1,wParam,lParam);
	}
	return CallNextHookEx(hKeyHook,nCode,wParam,lParam);
}

extern "C" __declspec(dllexport) void InstallHook(HWND hWnd)
{
	hWndBeeper=hWnd;
	hKeyHook=SetWindowsHookEx(WH_KEYBOARD,KeyHookProc,hModule,NULL);
}

extern "C" __declspec(dllexport) void UninstallHook()
{
	UnhookWindowsHookEx(hKeyHook);
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpRes)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		hModule=hInst;
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}