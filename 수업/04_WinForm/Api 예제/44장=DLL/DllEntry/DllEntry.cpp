#include <windows.h>
TCHAR *temp;
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpRes)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		temp=(TCHAR *)malloc(1024);
		if (temp == NULL)
			return FALSE;
		break;
	case DLL_PROCESS_DETACH:
		if (temp != NULL)
			free(temp);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) void SwapString(TCHAR *a, TCHAR *b)
{
	lstrcpy(temp,a);
	lstrcpy(a,b);
	lstrcpy(b,temp);
}
