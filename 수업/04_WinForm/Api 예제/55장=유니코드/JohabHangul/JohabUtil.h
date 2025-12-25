#define CP_JOHAB 1361
#define CP_WANSUNG 949

void WansungToJohab(const char *in,char *out);
wchar_t GetChosung(wchar_t Han,int Cho);
wchar_t GetJungsung(wchar_t Han,int Jung);
wchar_t GetJongsung(wchar_t Han,int Jong);
void JohabToWansung(const char *in, char *out);
void BreakHangul(const char *in, char *out);
int GetHanCount(wchar_t Han);
int GetHanStringCount(char *szHan);
