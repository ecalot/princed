// cSelectLevelWindow.cpp : implementation file
//

#include "stdafx.h"
#include "Princed V3.h"
#include "cSelectLevelWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cSelectLevelWindow dialog


cSelectLevelWindow::cSelectLevelWindow(CWnd* pParent /*=NULL*/)
	: CDialog(cSelectLevelWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(cSelectLevelWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cSelectLevelWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cSelectLevelWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cSelectLevelWindow, CDialog)
	//{{AFX_MSG_MAP(cSelectLevelWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cSelectLevelWindow message handlers

BOOL cSelectLevelWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
