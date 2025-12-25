// MfcKey.h : main header file for the MFCKEY application
//

#if !defined(AFX_MFCKEY_H__7BF50000_EB5D_42A2_BFB0_D826CA3F09AD__INCLUDED_)
#define AFX_MFCKEY_H__7BF50000_EB5D_42A2_BFB0_D826CA3F09AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcKeyApp:
// See MfcKey.cpp for the implementation of this class
//

class CMfcKeyApp : public CWinApp
{
public:
	CMfcKeyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcKeyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMfcKeyApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCKEY_H__7BF50000_EB5D_42A2_BFB0_D826CA3F09AD__INCLUDED_)
