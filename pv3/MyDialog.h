#if !defined(AFX_MYDIALOG_H__95397FE7_7038_4655_95B1_9BBAA2390A38__INCLUDED_)
#define AFX_MYDIALOG_H__95397FE7_7038_4655_95B1_9BBAA2390A38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDialog dialog

class CMyDialog : public CDialog
{
// Construction
public:
	CMyDialog(LPCTSTR lpszTemp, CWnd* pParentWnd = NULL): CDialog(lpszTemp, pParentWnd)
	{
	}
	CMyDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL): CDialog(nIDTemplate, pParentWnd)
	{
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDialog)
	public:
	virtual int DoModal();
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL MyCreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate,
		CWnd* pParentWnd, HINSTANCE hInst);

	// Generated message map functions
	//{{AFX_MSG(CMyDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIALOG_H__95397FE7_7038_4655_95B1_9BBAA2390A38__INCLUDED_)
