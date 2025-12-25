// MfcInfoDlgView.h : interface of the CMfcInfoDlgView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCINFODLGVIEW_H__98D72537_1A84_435F_97F1_E14611310700__INCLUDED_)
#define AFX_MFCINFODLGVIEW_H__98D72537_1A84_435F_97F1_E14611310700__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMfcInfoDlgView : public CView
{
protected: // create from serialization only
	CMfcInfoDlgView();
	DECLARE_DYNCREATE(CMfcInfoDlgView)

// Attributes
public:
	CMfcInfoDlgDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcInfoDlgView)
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
	virtual ~CMfcInfoDlgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcInfoDlgView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MfcInfoDlgView.cpp
inline CMfcInfoDlgDoc* CMfcInfoDlgView::GetDocument()
   { return (CMfcInfoDlgDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCINFODLGVIEW_H__98D72537_1A84_435F_97F1_E14611310700__INCLUDED_)
