// ToMfcDoc.cpp : implementation of the CToMfcDoc class
//

#include "stdafx.h"
#include "ToMfc.h"

#include "ToMfcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToMfcDoc

IMPLEMENT_DYNCREATE(CToMfcDoc, CDocument)

BEGIN_MESSAGE_MAP(CToMfcDoc, CDocument)
	//{{AFX_MSG_MAP(CToMfcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToMfcDoc construction/destruction

CToMfcDoc::CToMfcDoc()
{
	// TODO: add one-time construction code here

}

CToMfcDoc::~CToMfcDoc()
{
}

BOOL CToMfcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CToMfcDoc serialization

void CToMfcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CToMfcDoc diagnostics

#ifdef _DEBUG
void CToMfcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CToMfcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToMfcDoc commands
