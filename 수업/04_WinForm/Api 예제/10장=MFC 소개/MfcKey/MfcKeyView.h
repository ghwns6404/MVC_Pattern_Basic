// MfcKeyView.h : interface of the CMfcKeyView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCKEYVIEW_H__8A3EA474_A284_408F_AA8D_20A847F9A493__INCLUDED_)
#define AFX_MFCKEYVIEW_H__8A3EA474_A284_408F_AA8D_20A847F9A493__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMfcKeyView : public CView
{
protected: // create from serialization only
	CMfcKeyView();
	DECLARE_DYNCREATE(CMfcKeyView)

// Attributes
public:
	CMfcKeyDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcKeyView)
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
	virtual ~CMfcKeyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcKeyView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MfcKeyView.cpp
inline CMfcKeyDoc* CMfcKeyView::GetDocument()
   { return (CMfcKeyDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCKEYVIEW_H__8A3EA474_A284_408F_AA8D_20A847F9A493__INCLUDED_)
