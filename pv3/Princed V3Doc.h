// Princed V3Doc.h : interface of the CPrincedV3Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINCEDV3DOC_H__B98D158D_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_)
#define AFX_PRINCEDV3DOC_H__B98D158D_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "maps.h"

typedef int SCRNNUM;

class CPrincedV3Doc : public CDocument
{
protected: // create from serialization only
	CPrincedV3Doc();
	DECLARE_DYNCREATE(CPrincedV3Doc)

// Attributes
public:
	const static int SCRX, SCRY; //screen size 10x3
	const static int MAXSCRN; //max number of screens 24
	enum WALL {
		wNull=0x00,
		wFloor=0x01,
		wLooseFloor=0x0b,
		wOneColumn=0x03,
		wBrokenFloor=0x0e,
		wFloorTorch=0x13,
		wBrokenFloorTorch=0x1e,
		wBigColumnUp=0x09,
		wBigColumnDown=0x08,
		wPotion=0x0a,
		wSkeleton=0x15,
		wSword=0x16,
		wSpike=0x02,
		wChopper=0x12,
		wDoorOpener=0x0f,
		wDoorCloser=0x06,
		wDoor=0x04,
		wExitGateLeft=0x10,
		wExitGateRight=0x11,
		wWallBlock=0x14,
		wCarpetWallSpace=0x0c, //it also exists in dungeon :)
		
		//Some out handling objects
		wOutWallDown=0xFF1,
		wOutWallRight=0xFF2,
		wOutWallDownRight=0xFF3,

		//The following are palace-specific
		wCarpetWall=0x07,
		wArchDown=0x1a,
		wArchUpSmall=0x1b,
		wArchUpBigLeft=0x1c,
		wArchUpBigRight=0x1d,
		wArchColumn=0x19,
		wStarWindowLeft=0x17,
		wStarWindowRight=0x18
	};
	enum ATTRIB {
		null=0x00,
		wallBrick=1, wallBrick2=2, wallWindow=3,
		potRedSmall=1, potRedBig=2, potGreenBigGravity=3, potGreenBigInverse=4, potBlueSmall=5,
		spikState1=0, spikState2=1, spikState3=2, spikState4=3,
		doorOpen=1, doorClosed=2
	};
	enum G_ORIENTATION {
		gLeft=0, gRight=1
	};
	enum G_COLOR {
	};
	enum G_SKILL {
	};
	struct GUARD {
		int x, y; //screen location
		G_COLOR color;
		G_SKILL skill;
		G_ORIENTATION org;
	};

// Operations
public:
	void LoadLevel(int n);
	int GetLevel() const { return m_nLevel; }

	//cMap helper functions
	//x, y: block(wall)-unit coordinate
	//a, b: screen-unit coordinate
	//s: screen number
	WALL GetWallAt(int x, int y, SCRNNUM s=-1) const;
	ATTRIB GetBackAt(int x, int y, SCRNNUM s=-1) const;
	SCRNNUM PositionFromScreen(int x, int y) const;
	CString GetWallName(WALL nam, ATTRIB at=null) const;
	BOOL GetGuard(SCRNNUM s, GUARD *pInf=NULL) const;
	BOOL IsPalaceLevel() const;
	CSize GetMapSize() const;
	CPoint GetStartScreen() const;
	void PlaceObject(int x, int y, WALL wa, ATTRIB at=null, SCRNNUM s=-1);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrincedV3Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	friend class CPrincedV3View;
	virtual ~CPrincedV3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	cMaps m_Map; CString m_strFile;
	int m_nLevel; //-1 indicates it's PED file, not dat
	static CString m_strWallName[];

// Generated message map functions
protected:
	//{{AFX_MSG(CPrincedV3Doc)
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINCEDV3DOC_H__B98D158D_6AA8_11D7_8A8E_00104B7CC0FF__INCLUDED_)
