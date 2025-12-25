// ToMfcView.cpp : implementation of the CToMfcView class
//

#include "stdafx.h"
#include "ToMfc.h"

#include "ToMfcDoc.h"
#include "ToMfcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToMfcView

IMPLEMENT_DYNCREATE(CToMfcView, CView)

BEGIN_MESSAGE_MAP(CToMfcView, CView)
	//{{AFX_MSG_MAP(CToMfcView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToMfcView construction/destruction

CToMfcView::CToMfcView()
{
	// TODO: add construction code here

}

CToMfcView::~CToMfcView()
{
}

BOOL CToMfcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CToMfcView drawing

extern "C" __declspec(dllimport) int AddInteger(int,int);

void CToMfcView::OnDraw(CDC* pDC)
{
	CToMfcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CString str;
	str.Format("1+2 = %d",AddInteger(1,2));
	pDC->TextOut(100,100,str);
}

/////////////////////////////////////////////////////////////////////////////
// CToMfcView printing

BOOL CToMfcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CToMfcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CToMfcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CToMfcView diagnostics

#ifdef _DEBUG
void CToMfcView::AssertValid() const
{
	CView::AssertValid();
}

void CToMfcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToMfcDoc* CToMfcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToMfcDoc)));
	return (CToMfcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CToMfcView message handlers

