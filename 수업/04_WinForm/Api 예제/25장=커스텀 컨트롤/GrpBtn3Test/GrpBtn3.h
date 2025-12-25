#if !defined(AFX_GRPBTN3_H__7165D65A_2078_412B_A61A_2220E2D8F850__INCLUDED_)
#define AFX_GRPBTN3_H__7165D65A_2078_412B_A61A_2220E2D8F850__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrpBtn3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3 window

#define GBS_PUSH 0
#define GBS_CHECK 1
#define GBN_CLICKED 0

enum eState { GB_NORMAL, GB_HOT, GB_DOWN, GB_DISABLE };
class CGrpBtn3 : public CWnd
{
// Construction
public:
	CGrpBtn3();
	CBitmap Bitmap[4];
	eState State;
	eState CheckState;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrpBtn3)
	//}}AFX_VIRTUAL

// Implementation
public:
	eState GetState();
	void SetState(eState State);
	void ChangeState(eState State);
	BOOL IsPtOnMe(CPoint point);
	void DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBit);
	void DeleteAllBitmap();
	void SetImage(WORD Normal,WORD Hot,WORD Down,WORD Disable);
	virtual ~CGrpBtn3();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrpBtn3)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRPBTN3_H__7165D65A_2078_412B_A61A_2220E2D8F850__INCLUDED_)
