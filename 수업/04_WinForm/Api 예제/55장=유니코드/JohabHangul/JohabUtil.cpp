#include <windows.h>
#include "JohabUtil.h"

// 완성형 문자열의 한글 자모를 분해한다. 중간 계산을 위해 조합형으로 변환한다.
void BreakHangul(const char *in, char *out)
{
	int i,idx;
	wchar_t Han,Cho,Jung,Jong;
	char *pJohab;
	char *pTemp;

	pJohab=(char *)malloc(strlen(in)+1);
	pTemp=(char *)malloc(strlen(in)*3+2);
	WansungToJohab(in,pJohab);
	for (i=0,idx=0;i<(int)strlen(pJohab);i++) {
		if (IsDBCSLeadByte(in[i])) {
			Han=MAKEWORD(pJohab[i+1],pJohab[i]);
			Cho=GetChosung(Han,(Han & 0x7c00) >> 10);
			Jung=GetJungsung(Han,(Han & 0x03e0) >> 5);
			Jong=GetJongsung(Han,Han & 0x001f);
			
			if (Cho) {
				pTemp[idx++]=HIBYTE(Cho);
				pTemp[idx++]=LOBYTE(Cho);
			}
			if (Jung) {
				pTemp[idx++]=HIBYTE(Jung);
				pTemp[idx++]=LOBYTE(Jung);
			}
			if (Jong) {
				pTemp[idx++]=HIBYTE(Jong);
				pTemp[idx++]=LOBYTE(Jong);
			}
			i++;
		} else {
			pTemp[idx++]=in[i];
		}
	}
	pTemp[idx]=0;
	JohabToWansung(pTemp,out);
	free(pTemp);
	free(pJohab);
}

wchar_t GetChosung(wchar_t Han,int Cho)
{
	static wchar_t arCode[32]= {0,0,0x8841,0x8c41,0x9041,0x9441,0x9841,0x9c41,
		0xa041,0xa441,0xa841,0xac41,0xb041,0xb441,0xb841,0xbc41,0xc041,0xc441,
		0xc841,0xcc41,0xd041,
	};

	return arCode[Cho];
}

wchar_t GetJungsung(wchar_t Han,int Jung)
{
	static wchar_t arCode[32]= {0,0,0,0x8461,0x8481,0x84a1,0x84c1,0x84e1,
		0,0,0x8541,0x8561,0x8581,0x85a1,0x85c1,0x85e1,0,0,0x8641,0x8661,
		0x8681,0x86a1,0x86c1,0x86e1,0,0,0x8741,0x8761,0x8781,0x87a1,
	};

	return arCode[Jung];
}

wchar_t GetJongsung(wchar_t Han,int Jong)
{
	static wchar_t arCode[32]= {0,0,0x8442,0x8443,0x8444,0x8445,0x8446,
		0x8447,0x8448,0x8449,0x844a,0x844b,0x844c,0x844d,0x844e,0x844f,
		0x8450,0x8451,0,0x8453,0x8454,0x8455,0x8456,0x8457,0x8458,
		0x8459,0x845a,0x845b,0x845c,0x845d,
	};

	return arCode[Jong];
}

void WansungToJohab(const char *in,char *out)
{
	size_t len=strlen(in);
	wchar_t *pUni=(wchar_t *)malloc(len*2+2);

	MultiByteToWideChar(CP_WANSUNG,MB_ERR_INVALID_CHARS,in,-1,pUni,len*2);
	WideCharToMultiByte(CP_JOHAB,0,pUni,-1,out,len+1,0,0);
	free(pUni);
}

void JohabToWansung(const char *in, char *out)
{
	size_t len=strlen(in);
	wchar_t *pUni=(wchar_t *)malloc(len*2+2);

	MultiByteToWideChar(CP_JOHAB,MB_ERR_INVALID_CHARS,in,-1,pUni,len*2);
	WideCharToMultiByte(CP_WANSUNG,0,pUni,-1,out,len+1,0,0);
	free(pUni);
}

int GetHanCount(wchar_t Han)
{
	static unsigned char arNum[3][32]={
		{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,1,1,1,1,1,0,0,1,1,1,1,2,2,0,0,2,1,1,2,2,2,0,0,1,1,2,1,0,0},
		{0,0,1,1,2,1,2,2,1,1,2,2,2,2,2,2,2,1,0,1,2,1,1,1,1,1,1,1,1,1,0,0}
	};

	return arNum[0][(Han & 0x7c00) >> 10]+arNum[1][(Han & 0x03e0) >> 5]+
		arNum[2][Han & 0x001f];
}

int GetHanStringCount(char *szHan)
{
	int Num=0;
	int idx;
	char *pJohab;

	pJohab=(char *)malloc(strlen(szHan)+1);
	WansungToJohab(szHan,pJohab);
	for (idx=0;pJohab[idx];) {
		if (pJohab[idx] & 0x80) {
			Num+=GetHanCount(MAKEWORD(pJohab[idx+1],pJohab[idx]));
			idx+=2;
		} else {
			Num++;
			idx+=1;
		}
	}
	free(pJohab);
	return Num;
}
