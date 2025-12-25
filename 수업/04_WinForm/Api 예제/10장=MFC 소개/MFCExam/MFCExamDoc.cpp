// MFCExamDoc.cpp : implementation of the CMFCExamDoc class
//

#include "stdafx.h"
#include "MFCExam.h"

#include "MFCExamDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCExamDoc

IMPLEMENT_DYNCREATE(CMFCExamDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCExamDoc, CDocument)
	//{{AFX_MSG_MAP(CMFCExamDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCExamDoc construction/destruction

CMFCExamDoc::CMFCExamDoc()
{
	// TODO: add one-time construction code here

}

CMFCExamDoc::~CMFCExamDoc()
{
}

BOOL CMFCExamDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMFCExamDoc serialization

void CMFCExamDoc::Serialize(CArchive& ar)
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
// CMFCExamDoc diagnostics

#ifdef _DEBUG
void CMFCExamDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCExamDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCExamDoc commands
