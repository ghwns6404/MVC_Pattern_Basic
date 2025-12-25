// ToMfcView.h : interface of the CToMfcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMFCVIEW_H__C2FBA2ED_C5FB_11D2_8FC8_006097CDD64B__INCLUDED_)
#define AFX_TOMFCVIEW_H__C2FBA2ED_C5FB_11D2_8FC8_006097CDD64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToMfcView : public CView
{
protected: // create from serialization only
	CToMfcView();
	DECLARE_DYNCREATE(CToMfcView)

// Attributes
public:
	CToMfcDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToMfcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToMfcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToMfcView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ToMfcView.cpp
inline CToMfcDoc* CToMfcView::GetDocument()
   { return (CToMfcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOMFCVIEW_H__C2FBA2ED_C5FB_11D2_8FC8_006097CDD64B__INCLUDED_)
