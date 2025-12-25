// GrpBtn3TestView.cpp : implementation of the CGrpBtn3TestView class
//

#include "stdafx.h"
#include "GrpBtn3Test.h"

#include "GrpBtn3TestDoc.h"
#include "GrpBtn3TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestView

IMPLEMENT_DYNCREATE(CGrpBtn3TestView, CView)

BEGIN_MESSAGE_MAP(CGrpBtn3TestView, CView)
	//{{AFX_MSG_MAP(CGrpBtn3TestView)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestView construction/destruction

CGrpBtn3TestView::CGrpBtn3TestView()
{
	// TODO: add construction code here

}

CGrpBtn3TestView::~CGrpBtn3TestView()
{
}

BOOL CGrpBtn3TestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestView drawing

void CGrpBtn3TestView::OnDraw(CDC* pDC)
{
	CGrpBtn3TestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	TCHAR *Mes="E:사용금지, S:보이기, M:이동, C:체크";
	pDC->TextOut(10,100,Mes);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestView printing

BOOL CGrpBtn3TestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGrpBtn3TestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGrpBtn3TestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestView diagnostics

#ifdef _DEBUG
void CGrpBtn3TestView::AssertValid() const
{
	CView::AssertValid();
}

void CGrpBtn3TestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGrpBtn3TestDoc* CGrpBtn3TestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrpBtn3TestDoc)));
	return (CGrpBtn3TestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestView message handlers

void CGrpBtn3TestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	m_Btn1.Create(NULL,NULL,WS_CHILD | WS_VISIBLE | GBS_PUSH, CRect(10,10,26,26),this,1);
	m_Btn1.SetImage(IDB_BTN1NORMAL,IDB_BTN1HOT,IDB_BTN1DOWN,IDB_BTN1DISABLE);
	m_Btn2.Create(NULL,NULL,WS_CHILD | WS_VISIBLE | GBS_PUSH, CRect(30,10,62,42),this,2);
	m_Btn2.SetImage(IDB_BTN2NORMAL,IDB_BTN2HOT,IDB_BTN2DOWN,IDB_BTN2DISABLE);
	m_Btn3.Create(NULL,NULL,WS_CHILD | WS_VISIBLE | GBS_CHECK, CRect(80,10,104,34),this,3);
	m_Btn3.SetImage(IDB_BTN3NORMAL,IDB_BTN3NORMAL,IDB_BTN3DOWN,IDB_BTN3NORMAL);
}

BOOL CGrpBtn3TestView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (LOWORD(wParam)) {
	case 1:
		MessageBeep(0);
		break;
	case 2:
		MessageBox("두 번째 버튼을 눌렀습니다.");
		break;
	case 3:
		if (m_Btn3.GetState() == GB_DOWN) {
			AfxGetMainWnd()->SetWindowText("체크 되었음");
		} else {
			AfxGetMainWnd()->SetWindowText("체크 안되었음");
		}
		break;
	}
	
	return CView::OnCommand(wParam, lParam);
}

void CGrpBtn3TestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CRect rt;
	switch (nChar) {
	case 'E':
		m_Btn2.EnableWindow(!m_Btn2.IsWindowEnabled());
		break;
	case 'S':
		m_Btn2.ShowWindow(m_Btn2.IsWindowVisible() ? SW_HIDE:SW_SHOW);
		break;
	case 'M':
		m_Btn2.GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_Btn2.SetWindowPos(NULL,rt.left,rt.top+10,0,0,SWP_NOSIZE | SWP_NOZORDER);
		break;
	case 'C':
		m_Btn3.SetState(m_Btn3.GetState()==GB_DOWN ? GB_NORMAL:GB_DOWN);
		break;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
