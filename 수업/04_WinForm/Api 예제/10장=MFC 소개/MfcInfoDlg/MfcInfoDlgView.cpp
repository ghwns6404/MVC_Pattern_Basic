// MfcInfoDlgView.cpp : implementation of the CMfcInfoDlgView class
//

#include "stdafx.h"
#include "MfcInfoDlg.h"

#include "MfcInfoDlgDoc.h"
#include "MfcInfoDlgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgView

IMPLEMENT_DYNCREATE(CMfcInfoDlgView, CView)

BEGIN_MESSAGE_MAP(CMfcInfoDlgView, CView)
	//{{AFX_MSG_MAP(CMfcInfoDlgView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgView construction/destruction

CMfcInfoDlgView::CMfcInfoDlgView()
{
	// TODO: add construction code here

}

CMfcInfoDlgView::~CMfcInfoDlgView()
{
}

BOOL CMfcInfoDlgView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgView drawing

void CMfcInfoDlgView::OnDraw(CDC* pDC)
{
	CMfcInfoDlgDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->TextOut(pDoc->m_x, pDoc->m_y, pDoc->m_str);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgView printing

BOOL CMfcInfoDlgView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMfcInfoDlgView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMfcInfoDlgView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgView diagnostics

#ifdef _DEBUG
void CMfcInfoDlgView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcInfoDlgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcInfoDlgDoc* CMfcInfoDlgView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcInfoDlgDoc)));
	return (CMfcInfoDlgDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgView message handlers

#include "InfoDlg.h"
void CMfcInfoDlgView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMfcInfoDlgDoc* pDoc = GetDocument();
	CInfoDlg dlg;
	dlg.m_x=pDoc->m_x;
	dlg.m_y=pDoc->m_y;
	dlg.m_str=pDoc->m_str;
	if (dlg.DoModal()==IDOK) {
		pDoc->m_x=dlg.m_x;
		pDoc->m_y=dlg.m_y;
		pDoc->m_str=dlg.m_str;
		Invalidate();
	}
	
	CView::OnLButtonDown(nFlags, point);
}
