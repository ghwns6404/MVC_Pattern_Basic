// MfcInfoDlg.h : main header file for the MFCINFODLG application
//

#if !defined(AFX_MFCINFODLG_H__68331299_EB81_4580_A390_79E11ADCABD3__INCLUDED_)
#define AFX_MFCINFODLG_H__68331299_EB81_4580_A390_79E11ADCABD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfcInfoDlgApp:
// See MfcInfoDlg.cpp for the implementation of this class
//

class CMfcInfoDlgApp : public CWinApp
{
public:
	CMfcInfoDlgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcInfoDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMfcInfoDlgApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCINFODLG_H__68331299_EB81_4580_A390_79E11ADCABD3__INCLUDED_)
