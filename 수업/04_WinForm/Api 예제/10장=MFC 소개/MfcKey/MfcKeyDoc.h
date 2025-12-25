// MfcKeyDoc.h : interface of the CMfcKeyDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCKEYDOC_H__E732C81E_C81B_4A60_8B04_74E0DB25E3C5__INCLUDED_)
#define AFX_MFCKEYDOC_H__E732C81E_C81B_4A60_8B04_74E0DB25E3C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMfcKeyDoc : public CDocument
{
protected: // create from serialization only
	CMfcKeyDoc();
	DECLARE_DYNCREATE(CMfcKeyDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcKeyDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString str;
	virtual ~CMfcKeyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcKeyDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCKEYDOC_H__E732C81E_C81B_4A60_8B04_74E0DB25E3C5__INCLUDED_)
