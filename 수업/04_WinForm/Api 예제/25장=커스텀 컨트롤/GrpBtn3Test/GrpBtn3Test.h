// GrpBtn3Test.h : main header file for the GRPBTN3TEST application
//

#if !defined(AFX_GRPBTN3TEST_H__F751694F_FBF8_479A_8B7A_7687CB7AFB02__INCLUDED_)
#define AFX_GRPBTN3TEST_H__F751694F_FBF8_479A_8B7A_7687CB7AFB02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3TestApp:
// See GrpBtn3Test.cpp for the implementation of this class
//

class CGrpBtn3TestApp : public CWinApp
{
public:
	CGrpBtn3TestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrpBtn3TestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGrpBtn3TestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRPBTN3TEST_H__F751694F_FBF8_479A_8B7A_7687CB7AFB02__INCLUDED_)
