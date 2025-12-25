#ifdef DLLEXPORT
#define MYDLLTYPE __declspec(dllexport)
#else
#define MYDLLTYPE __declspec(dllimport)
#endif

// 두 개의 정수를 더한다.
extern "C" MYDLLTYPE int AddInteger(int a, int b);
