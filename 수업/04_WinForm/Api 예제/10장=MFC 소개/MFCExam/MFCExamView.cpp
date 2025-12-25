// MFCExamView.cpp : implementation of the CMFCExamView class
//

#include "stdafx.h"
#include "MFCExam.h"

#include "MFCExamDoc.h"
#include "MFCExamView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCExamView

IMPLEMENT_DYNCREATE(CMFCExamView, CView)

BEGIN_MESSAGE_MAP(CMFCExamView, CView)
	//{{AFX_MSG_MAP(CMFCExamView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCExamView construction/destruction

CMFCExamView::CMFCExamView()
{
	// TODO: add construction code here

}

CMFCExamView::~CMFCExamView()
{
}

BOOL CMFCExamView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFCExamView drawing

void CMFCExamView::OnDraw(CDC* pDC)
{
	CMFCExamDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDC->TextOut(10,10,TEXT("MFC로 만든 예제입니다"));
}

/////////////////////////////////////////////////////////////////////////////
// CMFCExamView printing

BOOL CMFCExamView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCExamView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCExamView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMFCExamView diagnostics

#ifdef _DEBUG
void CMFCExamView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCExamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCExamDoc* CMFCExamView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCExamDoc)));
	return (CMFCExamDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCExamView message handlers
