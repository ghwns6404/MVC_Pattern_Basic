// MFCExamDoc.h : interface of the CMFCExamDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCEXAMDOC_H__9624A7CF_38AB_4B5C_8CE5_DC74A327661D__INCLUDED_)
#define AFX_MFCEXAMDOC_H__9624A7CF_38AB_4B5C_8CE5_DC74A327661D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMFCExamDoc : public CDocument
{
protected: // create from serialization only
	CMFCExamDoc();
	DECLARE_DYNCREATE(CMFCExamDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCExamDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFCExamDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFCExamDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCEXAMDOC_H__9624A7CF_38AB_4B5C_8CE5_DC74A327661D__INCLUDED_)
