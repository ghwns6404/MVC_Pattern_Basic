#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MetaEnum");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

// 16비트 메타 파일을 32비트 메타 파일로 변환하여 메타 파일 핸들을 리턴한다. 
// 에러 발생시 NULL을 리턴한다.
HENHMETAFILE ConvertWinToEnh(LPTSTR wmf)
{
	HENHMETAFILE hEnh;
	HMETAFILE wfile;
	DWORD dwSize;
	LPBYTE pBits;
	METAFILEPICT mp;
	HDC hdc;

	// 16비트 메타 파일을 읽고 메타 파일 크기만큼 메모리를 할당한다.
	wfile=GetMetaFile(wmf);
	if (wmf==NULL) 
		return NULL;
	dwSize=GetMetaFileBitsEx(wfile, 0,NULL);
	if (dwSize==0) {
		DeleteMetaFile(wfile);
		return NULL;
	}
	pBits=(LPBYTE)malloc(dwSize);
	
	// 메타 파일의 내용을 버퍼로 읽어들인다.
	GetMetaFileBitsEx(wfile, dwSize, pBits);
	mp.mm=MM_ANISOTROPIC;
	mp.xExt=1000;
	mp.yExt=1000;
	mp.hMF=NULL;

	// 32비트 메타 파일을 만든다.
	hdc=GetDC(NULL);
	hEnh=SetWinMetaFileBits(dwSize, pBits, hdc, &mp);
	ReleaseDC(NULL, hdc);
	DeleteMetaFile(wfile);
	free(pBits);
	return hEnh;
}


#pragma pack(push)
#pragma pack(2)
typedef struct
{
	DWORD		dwKey;
	WORD		hmf;
	SMALL_RECT	bbox;
	WORD		wInch;
	DWORD		dwReserved;
	WORD		wCheckSum;
} APMHEADER, *PAPMHEADER;
#pragma pack(pop)

// 플레이스블 메타 파일을 32비트 메타 파일로 변경해 준다. 
// 에러 발생시 NULL을 리턴한다.
HENHMETAFILE ConvertPlaToEnh(LPTSTR szFileName)
{
	HENHMETAFILE	hEnh;
	DWORD			dwSize;
	LPBYTE			pBits;
	METAFILEPICT	mp;
	HDC				hdc;
	HANDLE			hFile;

	// 32비트 메타 파일이 아니면 플레이스블 메타 파일로 읽는다.
	// 파일 크기만큼 메모리를 할당하고 메타 파일을 읽어들인다.
	hFile = CreateFile( szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;
	dwSize = GetFileSize( hFile, NULL );
	pBits = (LPBYTE)malloc( dwSize );
	ReadFile( hFile, pBits, dwSize, &dwSize, NULL );
	CloseHandle( hFile );
	// 플레이스블 메타 파일이 맞는지 확인한다.
	if( ((PAPMHEADER)pBits)->dwKey != 0x9ac6cdd7l )	{
		free( pBits );
		return NULL;
	}

	// 구조체를 채운다.
	mp.mm = MM_ANISOTROPIC;
	mp.xExt = ((PAPMHEADER)pBits)->bbox.Right - ((PAPMHEADER)pBits)->bbox.Left;
	mp.xExt = ( mp.xExt * 2540l ) / (DWORD)(((PAPMHEADER)pBits)->wInch);
	mp.yExt = ((PAPMHEADER)pBits)->bbox.Bottom - ((PAPMHEADER)pBits)->bbox.Top;
	mp.yExt = ( mp.yExt * 2540l ) / (DWORD)(((PAPMHEADER)pBits)->wInch);
	mp.hMF = NULL;
	// 메타 파일을 만든다.
	hdc = GetDC( NULL );
	hEnh = SetWinMetaFileBits( dwSize, &(pBits[sizeof(APMHEADER)]), hdc, &mp );
	ReleaseDC( NULL, hdc );
	free( pBits );
	return hEnh;
}

// 32비트 메타 파일의 핸들을 리턴한다. 16비트 메타 파일이나 플레이스블 메타 파일
// 일 경우 32비트 메타 파일로 변환해준다.
HENHMETAFILE ReadMeta(LPTSTR FileName)
{
	HENHMETAFILE hEnh;
	// 32비트 메타 파일의 핸들을 구해 리턴한다.
	hEnh=GetEnhMetaFile(FileName);
	if (hEnh != NULL)
		return hEnh;
	// 32비트 메타 파일이 아닐 경우 16비트 포멧으로 읽어보고 32비트 전환한다.
	hEnh=ConvertWinToEnh(FileName);
	if (hEnh != NULL)
		return hEnh;
	// 16비트 메타 파일도 아닐 경우 플레이스블 메타 파일을 32비트로 전환한다.
	hEnh=ConvertPlaToEnh(FileName);
	if (hEnh != NULL)
		return hEnh;
	// 세 경우 다 해당하지 않을 경우 NULL을 리턴한다.
	return NULL;
}

HWND hList;
int delay;
int count;
TCHAR *arMetaRecord[]={"","Header", "PolyBezier","Polygon","PolyLine","PolyBezierTo",			// 0
	"PolyLineTo","PolyPolyLine","PolyPolygon","SetWindowExtEx","SetWindowOrgEx",				// 6
	"SetViewPortExtEx","SetViewportOrgEx","SetBrushOrgEx","EOF","SetPixelV",					// 11
	"SetMapperFlags","SetMapMode","SetBkMode","SetPolyFillMode","SetRop2",						// 16
	"SetStretchBltMode","SetTextAlign","SetColorAdjustment","SetTextColor","SetBkColor",		// 21
	"OffsetClipRgn","MoveToEx","SetMetaRgn","ExcldueClipRect","IntersectClipRect",				// 26
	"ScaleViewportExtEx","ScaleWindowExtEx","SaveDC","ResotreDC","SetWorldTransform",			// 31
	"ModifyWorldTransForm","SelectObject","CreatePen","CreateBrushIndirect","DeleteObject",		// 36
	"AngleArc","Ellipse","Rectangle","RoundRect","Arc",											// 41
	"Chord","Pie","SelectPalette","CreatePalette","SetPaletteEntries",							// 46
	"ResizePalette","RealizePalette","ExtFloodFill","LineTo","ArcTo",							// 51
	"PolyDraw","SetArcDirection","SetMiterLimit","BeginPath","EndPath",							// 56
	"CloseFigure","FillPath","StrokeAndFillPath","StrokePath","FlattenPath",					// 61
	"WidenPath","SelectClipPath","AboartPath","EMPTY","GdiComment",								// 66
	"FillRgn","FrameRgn","InvertRgn","PaintRgn","ExtSelectClipRgn",								// 71
	"BitBlt","StretchBlt","MaskBlt","PlgBlt","SetDibitsToDevice",								// 76
	"StretchDIBits","ExtCreateFontIndirectW","ExtTextOutA","ExtTextOutW","PolyBezier16",		// 81
	"Polygon16","PolyLine16","PolyBezierTo16","PolyLineTo16","PolyPolyLine16",					// 86
	"PolyPolygon16","PolyDraw16","CreateMonoBrush","CreateDINPatternBrushPt","ExtCreatePen",	// 91
	"PolyTextOutA","PolyTextOutW","SetICMMode","CreateColorSpace","SetColorSpace",				// 96
	"DeleteColorSpace","GLSRecord","GLSBoundedRecord","PixelFormat","DrawEscape",				// 101
	"ExtEscape","StartDoc","SmallTextOut","ForceUfiMapping","NamedEscape",						// 106
	"ColorCorrectPalette","SetICMProfileA","SetICMProfileW","AlphaBlend","AlphaDibBlend",		// 111
	"TransparentBlt","TransparentDIB","GradientFill","SetLinkedUfis","SetTextjustification"		// 116
};

int CALLBACK EnhMetaFileProc(HDC hDC, HANDLETABLE *lpHTable, CONST ENHMETARECORD *lpEMFR, int nObj, LPARAM lpData)
{
	TCHAR str[255], sParm[128];
	POINT pt;
	int idx, i;

	// 메타 레코드 출력
	wsprintf(str,"%d-%s(%d), 인수=",
		count,arMetaRecord[lpEMFR->iType], lpEMFR->nSize);

	// 인수를 조사하되 최대 6개까지만 조사한다.
	// nSize가 DWORD 단위이므로 4로 나누고 iType,nSize의 길이도 포함되므로 2를 뺐다.
	for (i=0;i<int(lpEMFR->nSize)/4-2;i++) {
		if (i == 6)
			break;
		wsprintf(sParm,"%d,",lpEMFR->dParm[i]);
		strcat(str,sParm);
	}
	str[lstrlen(str)-1]=0;

	// 리스트 박스에 메타 레코드 출력
	idx=SendMessage(hList,LB_ADDSTRING,0,(LPARAM)str);
	SendMessage(hList,LB_SETCURSEL,idx,0);
	UpdateWindow(hList);
	count++;

	// 메타 레코드 재생
	PlayEnhMetaFileRecord(hDC,lpHTable,lpEMFR,nObj);
	Sleep(delay);

	// 커서가 0,0으로 이동하면 중지한다.
	GetCursorPos(&pt);
	if (pt.x==0 && pt.y==0) {
		if (MessageBox(hWndMain,"메타 파일 열거를 중지하시겠습니까?",
			"질문",MB_YESNO) == IDYES) {
			return 0;
		} else {
			return 1;
		}
	} else {
		return 1;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	OPENFILENAME ofn;
	TCHAR szFileName[MAX_PATH];
	HENHMETAFILE hEnh;
	RECT rt;

	switch (iMessage) {
	case WM_CREATE:
		hList=CreateWindow("listbox",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			LBS_NOTIFY | WS_VSCROLL,10,50,400,600,hWnd,(HMENU)0,g_hInst,NULL);
		CreateWindow("button","파일 열기",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20,10,100,25,hWnd,(HMENU)1,g_hInst,NULL);
		CreateWindow("static","지연시간",WS_CHILD | WS_VISIBLE,
			140,13,100,25,hWnd,(HMENU)-1,g_hInst,NULL);
		CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
			220,10,100,25,hWnd,(HMENU)2,g_hInst,NULL);
		SetDlgItemInt(hWnd,2,1,FALSE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1:
			hdc=GetDC(hWnd);
			// 출력하고자 하는 메타 파일의 핸들을 구한다.
			szFileName[0]=0;
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize=sizeof(OPENFILENAME);
			ofn.hwndOwner=hWnd;
			ofn.lpstrFilter="Meta File\0*.?MF\0";
			ofn.nFilterIndex=1;
			ofn.lpstrFile=szFileName;
			ofn.nMaxFile=MAX_PATH;

			if (GetOpenFileName(&ofn) != 0) {
				// 32비트 메타 파일의 핸들을 구해 재생한다.
				hEnh=ReadMeta(szFileName);
				if (hEnh==NULL)
					return 0;
				GetClientRect(hWnd, &rt);
				rt.left += 420;
				SendMessage(hList,LB_RESETCONTENT,0,0);
				count=1;
				delay=GetDlgItemInt(hWnd,2,NULL,FALSE);
				InvalidateRect(hWnd,NULL,TRUE);
				UpdateWindow(hWnd);
				//PlayEnhMetaFile(hdc, hEnh, &rt);
				EnumEnhMetaFile(hdc,hEnh,EnhMetaFileProc,NULL,&rt);
				DeleteEnhMetaFile(hEnh);
			}
			ReleaseDC(hWnd, hdc);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
