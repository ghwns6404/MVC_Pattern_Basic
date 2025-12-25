#include <windows.h>
DWORD TlsIndex;

//*
BOOL WINAPI DllMain(HINSTANCE hInst, DWORD fdwReason, LPVOID lpRes)
{
	LPVOID data;
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		TlsIndex=TlsAlloc();
		break;
	case DLL_PROCESS_DETACH:
		if (data) free(data);
		TlsFree(TlsIndex);
		break;
	case DLL_THREAD_ATTACH:
		data=calloc(sizeof(int),0);
		TlsSetValue(TlsIndex,data);
		break;
	case DLL_THREAD_DETACH:
		data=TlsGetValue(TlsIndex);
		if (data) free(data);
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) int GetCount()
{
	int *p;

	p=(int *)TlsGetValue(TlsIndex);
	(*p)++;
	return *p;
}
//*/

/*
extern "C" __declspec(dllexport) int GetCount()
{
	__declspec(thread) static int count=0;
	return ++count;
}
//*/