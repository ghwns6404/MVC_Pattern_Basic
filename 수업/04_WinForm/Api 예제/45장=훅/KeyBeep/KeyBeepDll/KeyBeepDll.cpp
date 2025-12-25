#include <windows.h>

#pragma data_seg(".kbdata")
HINSTANCE hModule=NULL;
HHOOK hKeyHook=NULL;
HWND hWndBeeper=NULL;
#pragma data_seg()
#pragma comment (linker, "/SECTION:.kbdata,RWS")

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

/*
// 파일 맵핑을 사용하는 방법
struct tag_KBData {
	HINSTANCE hModule;
	HHOOK hKeyHook;
	HWND hWndBeeper;
};
tag_KBData *pData;
HANDLE hFMap;

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode>=0) {
		SendMessage(pData->hWndBeeper,WM_USER+1,wParam,lParam);
	}
	return CallNextHookEx(pData->hKeyHook,nCode,wParam,lParam);
}

extern "C" __declspec(dllexport) void InstallHook(HWND hWnd)
{
	pData->hWndBeeper=hWnd;
	pData->hKeyHook=SetWindowsHookEx(WH_KEYBOARD,KeyHookProc,pData->hModule,NULL);
}

extern "C" __declspec(dllexport) void UninstallHook()
{
	UnhookWindowsHookEx(pData->hKeyHook);
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpRes)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		hFMap=CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 
			0, sizeof(tag_KBData), "KeyBeeperFileMapping");
		pData=(tag_KBData *)MapViewOfFile(hFMap, FILE_MAP_WRITE,0,0,0);
		pData->hModule=hInst;
		break;
	case DLL_PROCESS_DETACH:
		UnmapViewOfFile(pData);
		CloseHandle(hFMap);
		break;
	}
	return TRUE;
}
//*/

