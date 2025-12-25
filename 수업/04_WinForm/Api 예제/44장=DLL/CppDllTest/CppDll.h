#ifdef DLLEXPORT
#define CINTDLL __declspec(dllexport)
#else
#define CINTDLL __declspec(dllimport)
#endif

class CINTDLL CInt
{
private:
	int i;

public:
	CInt(int ai) : i(ai) { }
	void Inc();
	void Dec();
	int GetValue() const;
	void SetValue(int ai);
};