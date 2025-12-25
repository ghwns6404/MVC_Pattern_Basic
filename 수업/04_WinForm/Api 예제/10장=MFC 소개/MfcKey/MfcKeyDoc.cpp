// MfcKeyDoc.cpp : implementation of the CMfcKeyDoc class
//

#include "stdafx.h"
#include "MfcKey.h"

#include "MfcKeyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyDoc

IMPLEMENT_DYNCREATE(CMfcKeyDoc, CDocument)

BEGIN_MESSAGE_MAP(CMfcKeyDoc, CDocument)
	//{{AFX_MSG_MAP(CMfcKeyDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyDoc construction/destruction

CMfcKeyDoc::CMfcKeyDoc()
{
	// TODO: add one-time construction code here

}

CMfcKeyDoc::~CMfcKeyDoc()
{
}

BOOL CMfcKeyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMfcKeyDoc serialization

void CMfcKeyDoc::Serialize(CArchive& ar)
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
// CMfcKeyDoc diagnostics

#ifdef _DEBUG
void CMfcKeyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMfcKeyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyDoc commands
