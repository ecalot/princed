// Princed V3Doc.cpp : implementation of the CPrincedV3Doc class
//

#include "stdafx.h"
#include "Princed V3.h"

#include "maps.h"
#include "Princed V3Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3Doc

IMPLEMENT_DYNCREATE(CPrincedV3Doc, CDocument)

BEGIN_MESSAGE_MAP(CPrincedV3Doc, CDocument)
	//{{AFX_MSG_MAP(CPrincedV3Doc)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3Doc construction/destruction

//Width and height of POP1's screen
const int CPrincedV3Doc::SCRX=10, CPrincedV3Doc::SCRY=3;
const int CPrincedV3Doc::MAXSCRN=24;
CString CPrincedV3Doc::m_strWallName[32];

CPrincedV3Doc::CPrincedV3Doc(): m_Map("")
{
	m_Map.lev=(tLevel *)malloc(sizeof(tLevel));

	if(m_strWallName[0].IsEmpty()) { //only once when necessary
		int i; TCHAR buff[512];
		::LoadString(AfxGetApp()->m_hInstance, IDS_WALL_NAMES, buff, 512);
		for(i=0, m_strWallName[0]=_tcstok(buff, _T("\n")); i<31;
			i++, m_strWallName[i]=_tcstok(NULL, _T("\n")));
	}
}

CPrincedV3Doc::~CPrincedV3Doc()
{
}

BOOL CPrincedV3Doc::OnNewDocument()
{
	if(!CDocument::OnNewDocument()) return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3Doc diagnostics

#ifdef _DEBUG
void CPrincedV3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPrincedV3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3Doc commands

BOOL CPrincedV3Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(!CDocument::OnOpenDocument(lpszPathName)) return FALSE;
	CFile f(lpszPathName, CFile::modeRead); DWORD dw=f.GetLength(); f.Close();

	//according to the file size
	switch(dw) {
	case sizeof(tLevel): //if PED
		m_nLevel=-1; m_Map.mLoadPED(lpszPathName, m_Map.lev); break;
	case MAPS_BLOCK_SIZEOF_FILE: //if DAT, load whole level
		m_nLevel=1;
		m_Map.mLoadDAT(lpszPathName, m_Map.lev, m_nLevel);
		break;
	default:
		return FALSE; //neither of the supported types
	}
	return TRUE;
}

SCRNNUM CPrincedV3Doc::PositionFromScreen(int a, int b) const
{
	char c, x,y, err; x=(char)(a/SCRX)+1, y=(char)(b/SCRY)+1;
	c=m_Map.mGetScreen(m_Map.lev, y, x, &err); return err || c<0 ? 0: c;
}

CPrincedV3Doc::WALL CPrincedV3Doc::GetWallAt(int x, int y, SCRNNUM s) const
{
	switch(s) {
	case -1: //x, y is the absolute position: screen number should be implied from it
		s=PositionFromScreen(x, y);
		if(s) return static_cast<WALL>(m_Map.mGetWall(m_Map.lev, (char)s, y%SCRY, x%SCRX)&(~0x20));
		else return wNull;
	case 0: //invalid screen
		return wNull;
	default: //specified screen
		ASSERT(x>=0 && x<SCRX && y>=0 && y<SCRY);
		return static_cast<WALL>(m_Map.mGetWall(m_Map.lev, (char)s, y, x)&(~0x20));
	}
}

CPrincedV3Doc::ATTRIB CPrincedV3Doc::GetBackAt(int x, int y, SCRNNUM s) const
{
	switch(s) {
	case -1: //x, y is the absolute position: screen number should be implied from it
		s=PositionFromScreen(x, y);
		if(s) return static_cast<ATTRIB>(m_Map.mGetBack(m_Map.lev, (char)s, y%SCRY, x%SCRX));
		else return null;
	case 0: //invalid screen
		return null;
	default:
		ASSERT(x>=0 && x<SCRX && y>=0 && y<SCRY);
		return static_cast<ATTRIB>(m_Map.mGetBack(m_Map.lev, (char)s, y, x));
	}
}

void CPrincedV3Doc::LoadLevel(int n)
{
	//to go to another level, you must save the current document~
	if(m_nLevel!=-1 && SaveModified()) {
		m_Map.mLoadDAT(GetPathName(), m_Map.lev, n); m_nLevel=n; SetModifiedFlag(FALSE);
	}
}

CString CPrincedV3Doc::GetWallName(CPrincedV3Doc::WALL nam, CPrincedV3Doc::ATTRIB at) const
{
	int wn=static_cast<int>(nam);
	if(wn>=0 && wn<32) return m_strWallName[wn]; else return CString("");
}

BOOL CPrincedV3Doc::GetGuard(SCRNNUM s, GUARD *pInf) const
{
	char x, y, skill, color, dire, era; if(s<=0) return FALSE;
	m_Map.mGetGuard(m_Map.lev, s, &y, &x, &skill, &color, &dire, &era);
	if(pInf) {
		pInf->color=static_cast<G_COLOR>(color);
		pInf->skill=static_cast<G_SKILL>(skill);
		pInf->org=static_cast<G_ORIENTATION>(dire);
		pInf->x=x; pInf->y=y;
	}
	return era;
}

//If there're any objects which belong to a palace (arch, carpet, for example)
BOOL CPrincedV3Doc::IsPalaceLevel() const
{
	int i,j,k;
	for(k=1;k<=MAXSCRN;k++)
		for(j=0;j<SCRY;j++) for(i=0;i<SCRX;i++)
			switch(m_Map.mGetWall(m_Map.lev, k, j, i)) {
			case wCarpetWall:
			case wArchDown: case wArchUpSmall: case wArchUpBigLeft:
			case wArchUpBigRight: case wArchColumn:
			case wStarWindowLeft: case wStarWindowRight:
				return TRUE;
			case wSkeleton:
				return FALSE;
			case wCarpetWallSpace: //only if the adjacent cell is not the wall
				if((m_Map.mGetWall(m_Map.lev, k, j, i+1)&(~0x20))!=wWallBlock) return TRUE;
			}
	return FALSE;
}

void CPrincedV3Doc::PlaceObject(int x, int y, WALL wa, ATTRIB at, SCRNNUM s)
{
	switch(s) {
	case -1:
		s=PositionFromScreen(x, y); if(s) x%=SCRX, y%=SCRY; else return;
		break;
	case 0: return; //invalid screen
	default: ASSERT(x>=0 && x<SCRX && y>=0 && y<SCRY);
	}
	m_Map.mSetWall(m_Map.lev, (char)s, (char)y, (char)x, wa);
	m_Map.mSetBack(m_Map.lev, (char)s, (char)y, (char)x, at);
}

CSize CPrincedV3Doc::GetMapSize() const
{
	char i,j; CSize pt; m_Map.mGetSize(m_Map.lev, &i, &j); pt.cx=j-1; pt.cy=i-1;
	return pt;
}

CPoint CPrincedV3Doc::GetStartScreen() const
{
	char i, j; m_Map.mGetMainScreen(m_Map.lev, &i, &j); return CPoint(j,i);
}

  
void CPrincedV3Doc::OnEditUndo() 
{
	//will be implemented in the future.
}

void CPrincedV3Doc::OnEditRedo() 
{
	//will be implemented in the future.
}

//lev 멤버의 형태 고치기
//맵 크기를 결정할 때 GetSize를 사용하기
//툴바에 콤보 박스 구현하기

BOOL CPrincedV3Doc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if(m_nLevel==-1) {
		if(CDocument::OnSaveDocument(lpszPathName)) {
			m_Map.mSavePED(lpszPathName, m_Map.lev); return TRUE;
		}
	}
	else {
		//When saving DAT file, you mustn't call the base class, because it will completely
		//overwrite the file, while we're going to modify only a part of the file.

		//TODO: then, what about when handling SAVE AS??
		m_Map.mSaveDAT(lpszPathName, m_Map.lev);
		SetModifiedFlag(FALSE);
	}
	return FALSE;
}
