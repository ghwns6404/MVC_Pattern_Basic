#include <windows.h>
#include "Util.h"

BOOL IsMatch(TCHAR *Path, TCHAR *Pattern)
{
	TCHAR Pat[MAX_PATH];
	TCHAR *t,*p;
	TCHAR Ext[_MAX_EXT];
	TCHAR Name[MAX_PATH];
	TCHAR *p1,*p2;
	BOOL bOther;

	_splitpath(Path,NULL,NULL,Name,Ext);
	lstrcat(Name,Ext);
	CharUpper(Name);

	t=Pattern;
	for (;;) {
		p=Pat;
		while (*t!=';' && *t!=0) {
			*p=*t;
			p++;
			t++;
		}
		*p=0;

		CharUpper(Pat);
		// 패턴이 *.ext 인 경우 확장자만 비교한다. 이렇게 하지 않으면 01.name.ext 파일은 일치하지 않은 것으로 오판한다.
		// 근본적인 문제는 .이 더 이상 파일 이름과 확장자의 구분자가 아니라 파일명의 일부가 될 수 있다는 점이다.
		if (Pat[0]=='*' && Pat[1]=='.') {
			if (lstrcmpi(Pat+1,Ext)==0) {
				return TRUE;
			}
		}
		p1=Name;
		p2=Pat;
		for (;;) {
			if (*p2=='?') {
				p1++;
				p2++;
			} else if (*p2=='*') {
				p2++;
				while (*p1!=*p2 && *p1!=0) 
					p1++;
			} else {
				if (*p1!=*p2) {
					break;
				}
				p1++;
				p2++;
			}

			if (*p1==0 && *p2==0) {
				return TRUE;
			}
			if (*p1!=0 && *p2==0) {
				break;
			}
			if (*p1==0 && *p2!=0) {
				bOther=FALSE;
				while (*p2) {
					if (*p2!='.' && *p2!='*') {
						bOther=TRUE;
					}
					p2++;
				}
				if (bOther==FALSE) {
					return TRUE;
				}
				break;
			}
		}

		if (*t==0) {
			break;
		}
		t++;
	}
	return FALSE;
}

BOOL bContFIF=TRUE;
void FindInFiles(TCHAR *Path, TCHAR *Pattern, DWORD Flags, FIFCALLBACK pCallBack, LPVOID pCustom)
{
	TCHAR SrchPath[MAX_PATH];
	TCHAR szFinded[MAX_PATH];
	WIN32_FIND_DATA wfd;
	HANDLE hSrch;
	BOOL nResult=TRUE;

	lstrcpy(SrchPath, Path);
	if (SrchPath[lstrlen(SrchPath)-1] == '\\') {
		SrchPath[lstrlen(SrchPath)-1]=0;
	}
	lstrcat(SrchPath, "\\*.*");
	hSrch=FindFirstFile(SrchPath,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE) {
		return;
	}

	while (nResult) {
		wsprintf(szFinded,"%s\\%s",Path,wfd.cFileName);
		// 디렉토리중에 .과 ..은 제외한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..")) {
				if (
					((wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0 || (Flags & FIF_INCHID))
					&& ((Flags & FIF_DIRFILTER)==0 || IsMatch(szFinded, Pattern))
					) {
					pCallBack(szFinded,wfd.dwFileAttributes,pCustom);
				}
				// 현재, 부모 제외 코드 삭제
				if (Flags & FIF_DEEP) {
					FindInFiles(szFinded,Pattern,Flags,pCallBack,pCustom);
				}
			}
		} else {
			if (((wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0 || (Flags & FIF_INCHID)) 
				&& IsMatch(szFinded, Pattern)) {
				pCallBack(szFinded,wfd.dwFileAttributes,pCustom);
			}
		}
		if (bContFIF==FALSE)
			break;
		nResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
}
