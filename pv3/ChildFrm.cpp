// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Princed V3.h"
#include "Princed V3Doc.h"
#include "Princed V3View.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style=WS_OVERLAPPED|WS_CAPTION|FWS_ADDTOTITLE|WS_THICKFRAME|WS_MAXIMIZE|
		WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CHILD|WS_VISIBLE;
	return CMDIChildWnd::PreCreateWindow(cs);
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

LRESULT CChildFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message) {
	case WM_SETTEXT:
		{
			CString t; TCHAR pt[10]; int x;
			CPrincedV3Doc *pDoc=(DYNAMIC_DOWNCAST(CPrincedV3View, GetActiveView()))->GetDocument();
			x=pDoc->GetLevel();
			if(x!=-1) {
				wsprintf(pt, _T(" #%d"), pDoc->GetLevel());
				t=reinterpret_cast<LPCTSTR>(lParam); t+=pt;
				return CMDIChildWnd::DefWindowProc(message, wParam,
					reinterpret_cast<LPARAM>((LPCTSTR)t));
			}
		}
	}
	
	return CMDIChildWnd::DefWindowProc(message, wParam, lParam);
}
