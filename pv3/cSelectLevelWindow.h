#if !defined(AFX_CSELECTLEVELWINDOW_H__AF32C261_6F5D_11D7_8A8E_00104B7CC0FF__INCLUDED_)
#define AFX_CSELECTLEVELWINDOW_H__AF32C261_6F5D_11D7_8A8E_00104B7CC0FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cSelectLevelWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cSelectLevelWindow dialog

class cSelectLevelWindow : public CDialog
{
// Construction
public:
	cSelectLevelWindow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cSelectLevelWindow)
	enum { IDD = IDD_DIALOG_SELECT_LEVEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cSelectLevelWindow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cSelectLevelWindow)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSELECTLEVELWINDOW_H__AF32C261_6F5D_11D7_8A8E_00104B7CC0FF__INCLUDED_)
