// Princed V3.h : main header file for the PRINCED V3 application
//

#if !defined(AFX_PRINCEDV3_H__B98D1585_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_)
#define AFX_PRINCEDV3_H__B98D1585_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3App:
// See Princed V3.cpp for the implementation of this class
//

class CPrincedV3App : public CWinApp
{
public:
	CPrincedV3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrincedV3App)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPrincedV3App)
	afx_msg void OnAppAbout();
	afx_msg void OnFileExporttoimage();
	afx_msg void OnHelpPrincedhomepage();
	afx_msg void OnAddonsEditsavedgame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void msgBox(char *text);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINCEDV3_H__B98D1585_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_)



