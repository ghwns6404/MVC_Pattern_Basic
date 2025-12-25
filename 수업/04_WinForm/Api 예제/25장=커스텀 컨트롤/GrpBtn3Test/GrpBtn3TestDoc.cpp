// GrpBtn3TestDoc.cpp : implementation of the CGrpBtn3TestDoc class
//

#include "stdafx.h"
#include "GrpBtn3Test.h"

#include "GrpBtn3TestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestDoc

IMPLEMENT_DYNCREATE(CGrpBtn3TestDoc, CDocument)

BEGIN_MESSAGE_MAP(CGrpBtn3TestDoc, CDocument)
	//{{AFX_MSG_MAP(CGrpBtn3TestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestDoc construction/destruction

CGrpBtn3TestDoc::CGrpBtn3TestDoc()
{
	// TODO: add one-time construction code here

}

CGrpBtn3TestDoc::~CGrpBtn3TestDoc()
{
}

BOOL CGrpBtn3TestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestDoc serialization

void CGrpBtn3TestDoc::Serialize(CArchive& ar)
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
// CGrpBtn3TestDoc diagnostics

#ifdef _DEBUG
void CGrpBtn3TestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGrpBtn3TestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestDoc commands
