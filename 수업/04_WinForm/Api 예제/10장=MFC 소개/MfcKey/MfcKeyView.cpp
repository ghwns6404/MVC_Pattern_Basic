// MfcKeyView.cpp : implementation of the CMfcKeyView class
//

#include "stdafx.h"
#include "MfcKey.h"

#include "MfcKeyDoc.h"
#include "MfcKeyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyView

IMPLEMENT_DYNCREATE(CMfcKeyView, CView)

BEGIN_MESSAGE_MAP(CMfcKeyView, CView)
	//{{AFX_MSG_MAP(CMfcKeyView)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyView construction/destruction

CMfcKeyView::CMfcKeyView()
{
	// TODO: add construction code here

}

CMfcKeyView::~CMfcKeyView()
{
}

BOOL CMfcKeyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyView drawing

void CMfcKeyView::OnDraw(CDC* pDC)
{
	CMfcKeyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->TextOut(100,100,pDoc->str);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyView printing

BOOL CMfcKeyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMfcKeyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMfcKeyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyView diagnostics

#ifdef _DEBUG
void CMfcKeyView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcKeyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcKeyDoc* CMfcKeyView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcKeyDoc)));
	return (CMfcKeyDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyView message handlers

void CMfcKeyView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CMfcKeyDoc* pDoc = GetDocument();
	pDoc->str += (TCHAR)nChar;
	Invalidate();
	
	CView::OnChar(nChar, nRepCnt, nFlags);
}
