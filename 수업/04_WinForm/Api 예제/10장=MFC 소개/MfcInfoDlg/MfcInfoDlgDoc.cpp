// MfcInfoDlgDoc.cpp : implementation of the CMfcInfoDlgDoc class
//

#include "stdafx.h"
#include "MfcInfoDlg.h"

#include "MfcInfoDlgDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgDoc

IMPLEMENT_DYNCREATE(CMfcInfoDlgDoc, CDocument)

BEGIN_MESSAGE_MAP(CMfcInfoDlgDoc, CDocument)
	//{{AFX_MSG_MAP(CMfcInfoDlgDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgDoc construction/destruction

CMfcInfoDlgDoc::CMfcInfoDlgDoc()
{
	m_x=100;
	m_y=100;
	m_str=TEXT("String");
}

CMfcInfoDlgDoc::~CMfcInfoDlgDoc()
{
}

BOOL CMfcInfoDlgDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgDoc serialization

void CMfcInfoDlgDoc::Serialize(CArchive& ar)
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
// CMfcInfoDlgDoc diagnostics

#ifdef _DEBUG
void CMfcInfoDlgDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMfcInfoDlgDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgDoc commands
