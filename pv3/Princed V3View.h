// Princed V3View.h : interface of the CPrincedV3View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINCEDV3VIEW_H__B98D158F_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_)
#define AFX_PRINCEDV3VIEW_H__B98D158F_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPrincedV3View : public CScrollView
{
protected: // create from serialization only
	CPrincedV3View();
	DECLARE_DYNCREATE(CPrincedV3View)

// Attributes
public:
	CPrincedV3Doc* GetDocument();

// Operations
public:
	void DrawObject(CDC *pDC, int x, int y, CPrincedV3Doc::WALL wa, CPrincedV3Doc::ATTRIB at=
		CPrincedV3Doc::null);
	void CenterView();
	BOOL HandleKey(); //Idle-time processing for smooth key scroll
	void HandleEdit(CPoint pt, UINT nFlags=0);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrincedV3View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrincedV3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//Map tool
	static UINT m_nCurTool; //currently selected tool
	static CPoint m_ptPrev; //previous mouse cursor position (only one mouse, so static :-)

	//Graphic elements
	CBitmap m_bmDung, m_bmDungMask, m_bmPala, m_bmPalaMask;
	CBitmap m_bmGuard, m_bmGuardMask, m_bmBack, m_bmBackMask;
	BOOL m_bPalace; //yellow palace mode?

	//Metric factor
	int m_nBlockX, m_nBlockY, m_nFloorHeight, m_nFloorDep; //32, 63, 13, 3
	int m_nZoom;
	void SetZoom(int fact, CPoint orig=CPoint(0,0));
	const static int m_nZfact[];

	//Virtual DC
	CDC m_dcTemp; CBitmap *m_pbmOld;
	void PutSprite(CDC *pDC, int dstx, int dsty, CBitmap *pSrc, CBitmap *pMask,
		int srcx, int cx, int cy); //helper function

	//Keyboard behavior
	static BYTE m_byKey[256]; BOOL m_bSmoothKey; static int m_nKeys; DWORD m_dwLastMomn;

// Generated message map functions
protected:

	afx_msg void OnZoomFactor(UINT nID);
	afx_msg void OnUpdateZoomFactor(CCmdUI *pCmdUI);
	afx_msg void OnMapTool(UINT nID);
	afx_msg void OnUpdateMapTool(CCmdUI *pCmdUI);
	afx_msg void OnViewMode(UINT nID);
	afx_msg void OnUpdateViewMode(CCmdUI *pCmdUI);
	//{{AFX_MSG(CPrincedV3View)
	afx_msg void OnDebugExecuteprinceofpersia();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Princed V3View.cpp
inline CPrincedV3Doc* CPrincedV3View::GetDocument()
   { return (CPrincedV3Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINCEDV3VIEW_H__B98D158F_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_)
