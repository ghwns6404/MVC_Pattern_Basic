// ToMfcDoc.h : interface of the CToMfcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMFCDOC_H__C2FBA2EB_C5FB_11D2_8FC8_006097CDD64B__INCLUDED_)
#define AFX_TOMFCDOC_H__C2FBA2EB_C5FB_11D2_8FC8_006097CDD64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToMfcDoc : public CDocument
{
protected: // create from serialization only
	CToMfcDoc();
	DECLARE_DYNCREATE(CToMfcDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToMfcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToMfcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CToMfcDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOMFCDOC_H__C2FBA2EB_C5FB_11D2_8FC8_006097CDD64B__INCLUDED_)
