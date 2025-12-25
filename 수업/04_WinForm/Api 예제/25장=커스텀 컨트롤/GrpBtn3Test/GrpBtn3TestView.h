// GrpBtn3TestView.h : interface of the CGrpBtn3TestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRPBTN3TESTVIEW_H__A404C293_2859_4E12_8986_B4300A81EF35__INCLUDED_)
#define AFX_GRPBTN3TESTVIEW_H__A404C293_2859_4E12_8986_B4300A81EF35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GrpBtn3.h"

class CGrpBtn3TestView : public CView
{
protected: // create from serialization only
	CGrpBtn3TestView();
	DECLARE_DYNCREATE(CGrpBtn3TestView)

// Attributes
public:
	CGrpBtn3TestDoc* GetDocument();
	CGrpBtn3 m_Btn1;
	CGrpBtn3 m_Btn2;
	CGrpBtn3 m_Btn3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrpBtn3TestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGrpBtn3TestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGrpBtn3TestView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GrpBtn3TestView.cpp
inline CGrpBtn3TestDoc* CGrpBtn3TestView::GetDocument()
   { return (CGrpBtn3TestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRPBTN3TESTVIEW_H__A404C293_2859_4E12_8986_B4300A81EF35__INCLUDED_)
