// MyDialog.cpp : implementation file
// System font-censitive dialog class

#include "stdafx.h"
#include "MyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialog dialog


/////////////////////////////////////////////////////////////////////////////
// CMyDialog message handlers

#include <afxpriv.h>
#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)
int CMyDialog::DoModal() 
{
	// can be constructed with a resource template or InitModalIndirect
	ASSERT(m_lpszTemplateName != NULL || m_hDialogTemplate != NULL ||
		m_lpDialogTemplate != NULL);

	// load resource as necessary
	LPCDLGTEMPLATE lpDialogTemplate = m_lpDialogTemplate;
	HGLOBAL hDialogTemplate = m_hDialogTemplate;
	HINSTANCE hInst = AfxGetResourceHandle();
	if (m_lpszTemplateName != NULL)
	{
		hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
		HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
		hDialogTemplate = LoadResource(hInst, hResource);
	}
	if (hDialogTemplate != NULL)
		lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);

	// return -1 in case of failure to load the dialog template resource
	if (lpDialogTemplate == NULL)
		return -1;

	// disable parent (before creating dialog)
	HWND hWndParent = PreModal();
	AfxUnhookWindowCreate();
	BOOL bEnableParent = FALSE;
	if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}

	TRY
	{
		// create modeless dialog
		AfxHookWindowCreate(this);
		if (MyCreateDlgIndirect(lpDialogTemplate,
						CWnd::FromHandle(hWndParent), hInst))
		{
			if (m_nFlags & WF_CONTINUEMODAL)
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				if (GetStyle() & DS_NOIDLEMSG)
					dwFlags |= MLF_NOIDLEMSG;
				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
			}

			// hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		}
	}
	CATCH_ALL(e)
	{
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL

	if (bEnableParent)
		::EnableWindow(hWndParent, TRUE);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	// destroy modal window
	DestroyWindow();
	PostModal();

	// unlock/free resources as necessary
	if (m_lpszTemplateName != NULL || m_hDialogTemplate != NULL)
		UnlockResource(hDialogTemplate);
	if (m_lpszTemplateName != NULL)
		FreeResource(hDialogTemplate);

	return m_nModalResult;
}

#include <AFXIMPL.H>
#include <OCCIMPL.H>
BOOL CMyDialog::MyCreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate,
	CWnd* pParentWnd, HINSTANCE hInst)
{
	ASSERT(lpDialogTemplate != NULL);
	if (pParentWnd != NULL)
		ASSERT_VALID(pParentWnd);

	if (hInst == NULL)
		hInst = AfxGetInstanceHandle();

#ifndef _AFX_NO_OCC_SUPPORT
	_AFX_OCC_DIALOG_INFO occDialogInfo;
	COccManager* pOccManager = afxOccManager;
#endif

	HGLOBAL hTemplate = NULL;

	HWND hWnd = NULL;
#ifdef _DEBUG
	DWORD dwError = 0;
#endif

	TRY
	{
		VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
		AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

#ifndef _AFX_NO_OCC_SUPPORT
		// separately create OLE controls in the dialog template
		if (pOccManager != NULL)
		{
			if (!SetOccDialogInfo(&occDialogInfo))
				return FALSE;

			lpDialogTemplate = pOccManager->PreCreateDialog(&occDialogInfo,
				lpDialogTemplate);
		}

		if (lpDialogTemplate == NULL)
			return FALSE;
#endif //!_AFX_NO_OCC_SUPPORT

		// If no font specified, set the system font.
		CString strFace;
		WORD wSize = 0;
		BOOL bSetSysFont = !CDialogTemplate::GetFont(lpDialogTemplate, strFace,
			wSize);

		// On DBCS systems, also change "MS Sans Serif" or "Helv" to system font.
		if ((!bSetSysFont) && GetSystemMetrics(SM_DBCSENABLED))
		{
			bSetSysFont = (strFace == _T("MS Shell Dlg") ||
				strFace == _T("MS Sans Serif") || strFace == _T("Helv"));
			if (bSetSysFont && (wSize == 8))
				wSize = 0;
		}

		if (bSetSysFont)
		{
			LOGFONT lg; int x; HDC hDC=::GetDC(NULL);
			//::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(lg), &lg);
			::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lg), &lg, SPIF_UPDATEINIFILE);
			CDialogTemplate dlgTemp(lpDialogTemplate);
			x=MulDiv(abs(lg.lfHeight), 72, ::GetDeviceCaps(hDC, LOGPIXELSY));
			dlgTemp.SetFont(lg.lfFaceName, x);
			hTemplate = dlgTemp.Detach(); ::ReleaseDC(NULL, hDC);
		}

		if (hTemplate != NULL)
			lpDialogTemplate = (DLGTEMPLATE*)GlobalLock(hTemplate);

		// setup for modal loop and creation
		m_nModalResult = -1;
		m_nFlags |= WF_CONTINUEMODAL;

		// create modeless dialog
		AfxHookWindowCreate(this);
		hWnd = ::CreateDialogIndirect(hInst, lpDialogTemplate,
			pParentWnd->GetSafeHwnd(), AfxDlgProc);
#ifdef _DEBUG
		dwError = ::GetLastError();
#endif
	}
	CATCH_ALL(e)
	{
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL

#ifndef _AFX_NO_OCC_SUPPORT
	if (pOccManager != NULL)
	{
		pOccManager->PostCreateDialog(&occDialogInfo);
		if (hWnd != NULL)
			SetOccDialogInfo(NULL);
	}
#endif //!_AFX_NO_OCC_SUPPORT

	if (!AfxUnhookWindowCreate())
		PostNcDestroy();        // cleanup if Create fails too soon

	// handle EndDialog calls during OnInitDialog
	if (hWnd != NULL && !(m_nFlags & WF_CONTINUEMODAL))
	{
		::DestroyWindow(hWnd);
		hWnd = NULL;
	}

	if (hTemplate != NULL)
	{
		GlobalUnlock(hTemplate);
		GlobalFree(hTemplate);
	}

	// help with error diagnosis (only if WM_INITDIALOG didn't EndDialog())
	if (hWnd == NULL)
	{
#ifdef _DEBUG
#ifndef _AFX_NO_OCC_SUPPORT
		if (m_nFlags & WF_CONTINUEMODAL)
		{
			if (afxOccManager == NULL)
			{
				TRACE0(">>> If this dialog has OLE controls:\n");
				TRACE0(">>> AfxEnableControlContainer has not been called yet.\n");
				TRACE0(">>> You should call it in your app's InitInstance function.\n");
			}
			else if (dwError != 0)
			{
				TRACE1("Warning: Dialog creation failed!  GetLastError returns 0x%8.8X\n", dwError);
			}
		}
#endif //!_AFX_NO_OCC_SUPPORT
#endif //_DEBUG
		return FALSE;
	}

	ASSERT(hWnd == m_hWnd);
	return TRUE;
}
