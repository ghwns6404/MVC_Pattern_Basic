// MfcInfoDlgDoc.h : interface of the CMfcInfoDlgDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCINFODLGDOC_H__0682AD33_12EB_4AD3_8BE2_33E899DE1ED3__INCLUDED_)
#define AFX_MFCINFODLGDOC_H__0682AD33_12EB_4AD3_8BE2_33E899DE1ED3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMfcInfoDlgDoc : public CDocument
{
protected: // create from serialization only
	CMfcInfoDlgDoc();
	DECLARE_DYNCREATE(CMfcInfoDlgDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcInfoDlgDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_str;
	int m_y;
	int m_x;
	virtual ~CMfcInfoDlgDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcInfoDlgDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCINFODLGDOC_H__0682AD33_12EB_4AD3_8BE2_33E899DE1ED3__INCLUDED_)
