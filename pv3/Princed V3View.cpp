// Princed V3View.cpp : implementation of the CPrincedV3View class
//

#include "stdafx.h"
#include "Princed V3.h"

#include "MainFrm.h"
#include "Princed V3Doc.h"
#include "Princed V3View.h"
#include "conf.cpp"

#include "process.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3View

IMPLEMENT_DYNCREATE(CPrincedV3View, CScrollView)

BEGIN_MESSAGE_MAP(CPrincedV3View, CScrollView)
	//{{AFX_MSG_MAP(CPrincedV3View)
	ON_COMMAND(ID_DEBUG_EXECUTEPRINCEOFPERSIA, OnDebugExecuteprinceofpersia)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_VIEW_ZOOM_200, ID_VIEW_ZOOM_25, OnZoomFactor)
	ON_COMMAND_RANGE(ID_VIEW_ASDUNGEON, ID_VIEW_ASPALACE, OnViewMode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_ASDUNGEON, ID_VIEW_ASPALACE, OnUpdateViewMode)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_ZOOM_200, ID_VIEW_ZOOM_25, OnUpdateZoomFactor)

	//the following number range is reserved for map tool command id.
	ON_COMMAND_RANGE(32850, 32899, OnMapTool)
	ON_UPDATE_COMMAND_UI_RANGE(32850, 32899, OnUpdateMapTool)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3View construction/destruction

int CPrincedV3View::m_nKeys; BYTE CPrincedV3View::m_byKey[256];
CPoint CPrincedV3View::m_ptPrev;
UINT CPrincedV3View::m_nCurTool=ID_MAPTOOLS_ARRASTRAR; //Hand tool is the default
const int CPrincedV3View::m_nZfact[]={
	50, 75, 100, 150, 200, 300
};

CPrincedV3View::CPrincedV3View(): m_bPalace(FALSE), m_pbmOld(NULL), m_nZoom(100),
	m_bSmoothKey(TRUE), m_dwLastMomn(0)
{
	memset(m_byKey, 0, sizeof(m_byKey));
}

CPrincedV3View::~CPrincedV3View()
{
}

BOOL CPrincedV3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3View drawing

void CPrincedV3View::PutSprite(CDC *pDC, int dstx, int dsty, CBitmap *pSrc, CBitmap *pMask,
	int srcx, int cx, int cy)
{
	int cxp=cx>0 ? cx: -cx; //always positive. in case or mirrored bitblt
	m_dcTemp.SelectObject(pMask);
	pDC->StretchBlt(dstx, dsty, cxp*m_nZoom/100, cy*m_nZoom/100, &m_dcTemp, srcx, 0, cx, cy,
		SRCAND);
	m_dcTemp.SelectObject(pSrc);
	pDC->StretchBlt(dstx, dsty, cxp*m_nZoom/100, cy*m_nZoom/100, &m_dcTemp, srcx, 0, cx, cy,
		SRCPAINT);
}

void CPrincedV3View::DrawObject(CDC *pDC, int x, int y, CPrincedV3Doc::WALL wa, CPrincedV3Doc::ATTRIB at)
{
	int ind=-1, atn; CBitmap *pbmBase, *pbmBaseMask;
	pbmBase=m_bPalace ? &m_bmPala: &m_bmDung; atn=static_cast<int>(at);
	pbmBaseMask=m_bPalace ? &m_bmPalaMask: &m_bmDungMask;

	//background processing
	if(m_bPalace) {
		switch(atn) { //only on a floor
		case 0:
			switch(wa) {
			case CPrincedV3Doc::wNull:
				break;
			default: //only on a floor
				ind=8; break;
			}
			break;
		case 1:
			switch(wa) { //only when blank space
			case CPrincedV3Doc::wNull:
				ind=8; break;
			}
		case 2: ind=9; break; case 3: ind=7; break;
		}
	}
	else {
		switch(wa) {
		case CPrincedV3Doc::wNull: case CPrincedV3Doc::wFloor:
		case CPrincedV3Doc::wLooseFloor: case CPrincedV3Doc::wBrokenFloor:
			if(atn>0 && atn<=3) ind=atn-1; //dungeon
		}
	}
	if(ind!=-1)
		PutSprite(pDC, x+m_nBlockX, y+m_nFloorHeight, &m_bmBack, &m_bmBackMask, ind*32,
			32, 63);

	ind=-1;
	switch(wa) {
	//empty-background walls
	case CPrincedV3Doc::wNull: break;
	case CPrincedV3Doc::wFloor: ind=3; break;
	case CPrincedV3Doc::wLooseFloor: ind=9; break;
	case CPrincedV3Doc::wBrokenFloor: ind=5; break;

	case CPrincedV3Doc::wOneColumn: ind=6; break;
	case CPrincedV3Doc::wFloorTorch: ind=4; break;
	case CPrincedV3Doc::wBrokenFloorTorch: ind=25; break;
	case CPrincedV3Doc::wBigColumnUp: ind=18; break;
	case CPrincedV3Doc::wBigColumnDown: ind=19; break;

	case CPrincedV3Doc::wPotion:
		switch(at) {
		case CPrincedV3Doc::potRedSmall: ind=7; break;
		case CPrincedV3Doc::potRedBig: ind=22; break;
		case CPrincedV3Doc::potGreenBigGravity:
		case CPrincedV3Doc::potGreenBigInverse: ind=21; break;
		case CPrincedV3Doc::potBlueSmall: ind=20; break;
		default: ind=3; break; //unknown potion
		}
		break;

	case CPrincedV3Doc::wSkeleton: ind=8; break;
	case CPrincedV3Doc::wSword: ind=16; break;
	case CPrincedV3Doc::wSpike: ind=12+atn; break; //12+(0~3)
	case CPrincedV3Doc::wChopper: ind=17; break;
	case CPrincedV3Doc::wDoorCloser: ind=10; break;
	case CPrincedV3Doc::wDoorOpener: ind=11; break;
	case CPrincedV3Doc::wDoor: ind=atn; break; //1,2

	case CPrincedV3Doc::wExitGateLeft:
		PutSprite(pDC, x, y-m_nFloorHeight-m_nFloorDep, pbmBase, pbmBaseMask, 23*58, 58, 76);
		ind=3; break;
	case CPrincedV3Doc::wExitGateRight:
		PutSprite(pDC, x, y-m_nFloorHeight-m_nFloorDep, pbmBase, pbmBaseMask, 24*58, 58, 76);
		ind=3; break;
	case CPrincedV3Doc::wStarWindowLeft:
		PutSprite(pDC, x, y-m_nFloorHeight-m_nFloorDep, pbmBase, pbmBaseMask, 30*58, 58, 76);
		ind=3; break;
	case CPrincedV3Doc::wStarWindowRight:
		PutSprite(pDC, x, y-m_nFloorHeight-m_nFloorDep, pbmBase, pbmBaseMask, 31*58, 58, 76);
		ind=3; break;
	case CPrincedV3Doc::wArchColumn: ind= m_bPalace ? 27: 3; break;
	case CPrincedV3Doc::wWallBlock: ind=0; break;
	case CPrincedV3Doc::wOutWallDown: ind=32; break;
	case CPrincedV3Doc::wOutWallRight: ind=33; break;

	case CPrincedV3Doc::wCarpetWall:
		PutSprite(pDC, x, y, pbmBase, pbmBaseMask, 3*58, 58, 76);
	case CPrincedV3Doc::wCarpetWallSpace:
		//special exception case for 0x00 flag: carpet wall with arch!!
		if(atn==0) PutSprite(pDC, x, y, pbmBase, pbmBaseMask, 29*58, 64, 76);
		else ind=(atn==1 ? 26: atn==2 ? 28: 0); break; //26+(0~2)
	}

	if(m_bPalace) {
		//golden palace objects
		switch(wa) {
		case CPrincedV3Doc::wArchDown:
			PutSprite(pDC, x, y+m_nFloorHeight, &m_bmBack, &m_bmBackMask, 3*32, 32, 63);
			break;
		case CPrincedV3Doc::wArchUpSmall:
			PutSprite(pDC, x, y+m_nFloorHeight, &m_bmBack, &m_bmBackMask, 4*32, 32, 63);
			break;
		case CPrincedV3Doc::wArchUpBigLeft:
			PutSprite(pDC, x, y+m_nFloorHeight, &m_bmBack, &m_bmBackMask, 5*32, 32, 63);
			break;
		case CPrincedV3Doc::wArchUpBigRight:
			PutSprite(pDC, x, y+m_nFloorHeight, &m_bmBack, &m_bmBackMask, 6*32, 32, 63);
			break;
		}
	}

	if(ind!=-1) PutSprite(pDC, x, y, pbmBase, pbmBaseMask, ind*58, 58, 76);
}

//array allocation helper
static int *new_zero_int(int size)
{
	int *p; p=new int[size]; memset(p, 0, size*sizeof(int)); return p;
}

template<typename T>
class CQueue {
protected:
    struct NODE {
		T data; NODE *extra;
    };
    NODE *m_pHead, *m_pTail; int m_nElem;
public:
    //Construction/destruction
    CQueue(): m_pHead(NULL), m_nElem(0) {}
    int GetCount() const { return m_nElem; } //number of elements
    virtual ~CQueue();
    virtual void Add(const T& elem);
    virtual bool Delete(T& elem); //returns the pointer to the element being deleted
};

template<typename T>
void CQueue<T>::Add(const T& elem)
{
    if(m_pHead) { m_pTail->extra=new NODE; m_pTail=m_pTail->extra; }
    else { m_pTail=m_pHead=new NODE; }
    m_pTail->extra=NULL;
    m_pTail->data=elem;
}

template<typename T>
bool CQueue<T>::Delete(T& elem)
{
	if(m_pHead) { //If there's anything to draw out
		NODE *tmp=m_pHead; elem=m_pHead->data; m_pHead=m_pHead->extra;
		delete tmp; return true;
	}
	else return false; //nothing
}

template<typename T>
CQueue<T>::~CQueue()
{
	NODE *tmp, *tmp2;
	for(tmp=m_pHead;tmp; tmp=tmp2) { tmp2=tmp->extra; delete tmp; }
}

void CPrincedV3View::OnDraw(CDC* pDC)
{
	SCRNNUM s; CPrincedV3Doc* pDoc = GetDocument(); ASSERT_VALID(pDoc);
	int i,j,k, x,y, it, jt; CRect rect; CPoint sp=GetScrollPosition();
	CPrincedV3Doc::GUARD guar;

	//guard settings
	int *gu; CQueue<CPrincedV3Doc::GUARD> gq; gu=new_zero_int(CPrincedV3Doc::MAXSCRN+1);

	if(pDC->IsKindOf(RUNTIME_CLASS(CPaintDC))) //If by WM_PAINT,
		//we can optimize the area to be drawn by accessing its member
		rect=static_cast<CPaintDC *>(pDC)->m_ps.rcPaint;
	else GetClientRect(rect); //draw the entire screen
	
	rect+=sp; //this is an invalid area
	//to prevent garbages when scrolling to left/top, we must draw additional block (-1)
	//but also check for negative values.
	y=rect.bottom/m_nBlockY+1; x=rect.right/m_nBlockX+1; k=rect.top/m_nBlockY; k=max(k,0);
	for(i=rect.left/m_nBlockX-1, i=max(i,0);i<x;i++)
		for(j=y;j>=k;j--) {
			s=pDoc->PositionFromScreen(i, j);
			it=i%CPrincedV3Doc::SCRX; jt=j%CPrincedV3Doc::SCRY;

			//Virtual border objects
			if (!s) {
				SCRNNUM s2;
				//If it is a floor
				if ((jt==(CPrincedV3Doc::SCRY-1))&&(pDoc->PositionFromScreen(i, j+1)))
					DrawObject(pDC, i*m_nBlockX, j*m_nBlockY-m_nFloorHeight, CPrincedV3Doc::wFloor, CPrincedV3Doc::null);
				//If it is a wall
				else if ((it==(CPrincedV3Doc::SCRX-1))&&(pDoc->PositionFromScreen(i+1, j)))
					DrawObject(pDC, i*m_nBlockX, j*m_nBlockY-m_nFloorHeight, CPrincedV3Doc::wWallBlock, CPrincedV3Doc::null);
				//If it is a lower nothing
				else if ((jt==0)&&(s2=pDoc->PositionFromScreen(i, j-1))&&(pDoc->GetWallAt(it, CPrincedV3Doc::SCRY-1, s2)==CPrincedV3Doc::wNull))
					DrawObject(pDC, i*m_nBlockX, j*m_nBlockY-m_nFloorHeight, CPrincedV3Doc::wOutWallDown, CPrincedV3Doc::null);
				//If it is a right nothing
				else if ((it==0)&&(s2=pDoc->PositionFromScreen(i-1, j))&&(pDoc->GetWallAt(CPrincedV3Doc::SCRX-1, jt, s2)!=CPrincedV3Doc::wWallBlock))
					DrawObject(pDC, i*m_nBlockX, j*m_nBlockY-m_nFloorHeight, CPrincedV3Doc::wOutWallRight, CPrincedV3Doc::null);
				//If it is a right-lower nothing
				else if ((jt==0)&&(it==0)&&(s2=pDoc->PositionFromScreen(i-1, j-1))&&(pDoc->GetWallAt(CPrincedV3Doc::SCRX-1, CPrincedV3Doc::SCRY-1, s2)==CPrincedV3Doc::wNull))
					DrawObject(pDC, i*m_nBlockX, j*m_nBlockY-m_nFloorHeight, CPrincedV3Doc::wOutWallDownRight, CPrincedV3Doc::null);
				else 
					DrawObject(pDC, i*m_nBlockX, j*m_nBlockY-m_nFloorHeight, CPrincedV3Doc::wWallBlock, CPrincedV3Doc::null);
			}

			//Real objects
			DrawObject(pDC, i*m_nBlockX, j*m_nBlockY-m_nFloorHeight,
				pDoc->GetWallAt(it, jt, s), pDoc->GetBackAt(it, jt, s));

			if(!gu[s]) { //Prepare for handling guard (only one guard per screen)
				gu[s]=1; //only once
				if(pDoc->GetGuard(s, &guar)) { //This screen has a guard
					it=i-it; jt=j-jt; //12,13,14 -> 10, 3,4,5 -> 3
					guar.x+=it, guar.y+=jt; gq.Add(guar); //for convinience
				}
			}
		}
	
	//Now, display guards at once //TODO: Redraw the front layer
	while(gq.Delete(guar)) {					
		if(guar.color) k=guar.color; //normal guards
		else k=8; //fat guy/Jaffar

		//according to his direction
		switch(guar.org) {
		case CPrincedV3Doc::gRight:
			PutSprite(pDC, guar.x*m_nBlockX, guar.y*m_nBlockY-m_nFloorHeight,
				&m_bmGuard, &m_bmGuardMask, k*58, 58, 76);
			break;
		case CPrincedV3Doc::gLeft: //m_nFloorDep is uncertain
			PutSprite(pDC, guar.x*m_nBlockX+m_nFloorDep*2, guar.y*m_nBlockY-
				m_nFloorHeight, &m_bmGuard, &m_bmGuardMask, (k+1)*58, -58, 76);
		}
		guar.x=(guar.x/10)*10; guar.y=(guar.y/3)*3;
	}

	//at last, draw the border line
	CPen rp, *op; rp.CreatePen(PS_SOLID, 1, m_bPalace ? RGB(0,255,255): RGB(255,0,0));
	op=pDC->SelectObject(&rp);
	it=CPrincedV3Doc::SCRX*m_nBlockX; jt=CPrincedV3Doc::SCRY*m_nBlockY;
	for(j=(rect.top/jt)*jt;j<rect.bottom;j+=jt) {
		pDC->MoveTo(rect.left, j); pDC->LineTo(rect.right, j);
	}
	for(i=(rect.left/it)*it;i<rect.right;i+=it) {
		pDC->MoveTo(i, rect.top); pDC->LineTo(i, rect.bottom);
	}

	pDC->SelectObject(op); delete []gu;
#if 0
	CString st;
	st.Format("Rendering level %d, pos %d %d, (screen %d)  \n", m_nLev, i,j,c);
	pDC->TextOut(0, rect.bottom-16, st);

	//locate the guard
	{
			//handle the front layer (when the guard overlaps the column and something)
			for(err=b;err<b+2 && err<10;err++) { //have to consider the right side too :)
				ind=-1;
				switch(mpc.mGetWall(mpc.lev, c, a,err)&(~0x20)) {
				case 0x03: ind=26; break; //floor with 1-story column
				case 0x09: ind=27; break; //2-story floor (up part)
				case 0x08: ind=28; break; //2-story floor (down part)
					//TODO: more.. (spikes, choppers, doors ...)
				}
				if(ind!=-1) {
					tempd.SelectObject(&bmMask);
					pDC->BitBlt(err*32, a*63-13, 58, 76, &tempd, ind*58, 0, SRCAND);
					tempd.SelectObject(&bmElem);
					pDC->BitBlt(err*32, a*63-13, 58, 76, &tempd, ind*58, 0, SRCPAINT);
				}
			}
		}
	}
	tempd.SelectObject(olb);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3View printing

BOOL CPrincedV3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPrincedV3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPrincedV3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3View diagnostics

#ifdef _DEBUG
void CPrincedV3View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPrincedV3View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CPrincedV3Doc* CPrincedV3View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPrincedV3Doc)));
	return (CPrincedV3Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrincedV3View message handlers

void CPrincedV3View::OnDebugExecuteprinceofpersia() {
//	SetRegKeyValue(
	//cProcess normalExec("C:\\WINDOWS\\Escritorio\\kkx\\princed\\prince.exe");
	cConfig c;
	c.setSetting("test","probando");
	c.setSetting("tamaño",(char)43);

	Beep(2600,100);

}

void CPrincedV3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CPrincedV3Doc *pDoc=GetDocument();
	if(!m_byKey[nChar]) m_byKey[nChar]=1; m_nKeys++;
	switch(nChar) {
	case VK_PRIOR:
		pDoc->LoadLevel(pDoc->GetLevel()-1); m_bPalace=pDoc->IsPalaceLevel(); CenterView();
		Invalidate(); break;
	case VK_NEXT:
		pDoc->LoadLevel(pDoc->GetLevel()+1); m_bPalace=pDoc->IsPalaceLevel(); CenterView();
		Invalidate(); break;
	case VK_UP:
		if(m_bSmoothKey) break;
		ScrollToPosition(GetScrollPosition()+CPoint(0, -m_nBlockY)); break;
	case VK_DOWN:
		if(m_bSmoothKey) break;
		ScrollToPosition(GetScrollPosition()+CPoint(0, m_nBlockY)); break;
	case VK_LEFT:
		if(m_bSmoothKey) break;
		ScrollToPosition(GetScrollPosition()+CPoint(-m_nBlockX, 0)); break;
	case VK_RIGHT:
		if(m_bSmoothKey) break;
		ScrollToPosition(GetScrollPosition()+CPoint(m_nBlockX, 0)); break;
	case VK_SPACE: m_bPalace^=1; Invalidate(); break;
	}
	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPrincedV3View::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(GetCapture()==this) { //dragging
		HandleEdit(point, nFlags);
		switch(m_nCurTool) {
		case ID_MAPTOOLS_ARRASTRAR: //Hand tool. drag the view.
			ScrollToPosition(GetScrollPosition()+(m_ptPrev-point)); m_ptPrev=point;
			break;
		}
	}
	else {
		//Hit-test the position
		CMainFrame *pfr=DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		CPrincedV3Doc* pDoc=GetDocument(); CPoint ps; CString t;
		point+=GetScrollPosition(); ps.x=point.x/m_nBlockX, ps.y=point.y/m_nBlockY;
		t.Format("(%d, %d) - %s 0x%x! ", ps.x, ps.y,
			pDoc->GetWallName(pDoc->GetWallAt(ps.x, ps.y)), pDoc->GetBackAt(ps.x, ps.y));
		if(pfr) pfr->SetStatusText(t);
	}
}

void CPrincedV3View::OnInitialUpdate() 
{
	SetZoom(m_nZoom); CScrollView::OnInitialUpdate();

	m_bPalace=GetDocument()->IsPalaceLevel();
	if(m_bmBack.m_hObject==NULL) { //initialize graphic elements
		m_bmBack.LoadBitmap(IDB_BACKS); m_bmBackMask.LoadBitmap(IDB_BACKMASK);
		m_bmDung.LoadBitmap(IDB_ELEMS); m_bmDungMask.LoadBitmap(IDB_ELEMMASK);
		m_bmPala.LoadBitmap(IDB_ELEMP); m_bmPalaMask.LoadBitmap(IDB_ELEMPMASK);
		m_bmGuard.LoadBitmap(IDB_GUARD); m_bmGuardMask.LoadBitmap(IDB_GUARDMASK);

		CClientDC dc(this);
		m_dcTemp.CreateCompatibleDC(&dc); m_pbmOld=m_dcTemp.GetCurrentBitmap();
	}
}

void CPrincedV3View::OnDestroy() 
{
	CScrollView::OnDestroy(); m_dcTemp.SelectObject(m_pbmOld);	
}

BOOL CPrincedV3View::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect; GetClientRect(rect); //fill black~
	pDC->FillRect(rect, CBrush::FromHandle((HBRUSH)::GetStockObject(BLACK_BRUSH)));
	return TRUE;
}

void CPrincedV3View::OnZoomFactor(UINT nID)
{
	switch(nID) {
	case ID_VIEW_ZOOM_200: SetZoom(200); break; case ID_VIEW_ZOOM_100: SetZoom(100); break;
	case ID_VIEW_ZOOM_50: SetZoom(50); break; case ID_VIEW_ZOOM_25: SetZoom(25); break;
	}
	Invalidate();
}

//Set the zoom factor, change related variables, and reset the view, placing the CPoint
//orientation to the center.
#define ZOOM(x) x=x*m_nZoom/100
void CPrincedV3View::SetZoom(int fact, CPoint point)
{
	CSize sz; int nPrevz=m_nZoom; CRect rect; GetClientRect(rect); m_nZoom=fact;
	m_nBlockX=32*m_nZoom/100; m_nBlockY=63*m_nZoom/100; m_nFloorHeight=13*m_nZoom/100;
	m_nFloorDep=3*m_nZoom/100;

	sz=GetDocument()->GetMapSize();
	SetScrollSizes(MM_TEXT, CSize(m_nBlockX*CPrincedV3Doc::SCRX*15,
		m_nBlockY*CPrincedV3Doc::SCRY*12),
		CSize(CPrincedV3Doc::SCRX*m_nBlockX, m_nBlockY*CPrincedV3Doc::SCRY),
		CSize(m_nBlockX, m_nBlockY));

	point+=GetScrollPosition(); //now, center this point.
	point.x=point.x*m_nZoom/nPrevz; point.y=point.y*m_nZoom/nPrevz;
	ScrollToPosition(point+CPoint(-rect.Width()/2, -rect.Height()/2 ));
	if(m_nZoom!=nPrevz) Invalidate(); //no optimized drawing
}
#undef ZOOM

void CPrincedV3View::OnUpdateZoomFactor(CCmdUI *pCmdUI)
{
	switch(pCmdUI->m_nID) {
	case ID_VIEW_ZOOM_200: pCmdUI->SetRadio(m_nZoom==200); break;
	case ID_VIEW_ZOOM_100: pCmdUI->SetRadio(m_nZoom==100); break;
	case ID_VIEW_ZOOM_50: pCmdUI->SetRadio(m_nZoom==50); break;
	case ID_VIEW_ZOOM_25: pCmdUI->SetRadio(m_nZoom==25); break;
	}
}

void CPrincedV3View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_byKey[nChar]) m_byKey[nChar]=0, m_nKeys--;
}

BOOL CPrincedV3View::HandleKey()
{
	if(m_nKeys==0 || !m_bSmoothKey) { m_dwLastMomn=0; return FALSE; }
	if(m_dwLastMomn==0) { m_dwLastMomn=::GetTickCount(); return TRUE; }
	DWORD ex=::GetTickCount(); int delta; CPoint newp=GetScrollPosition();
	delta=(ex-m_dwLastMomn)*m_nZoom/200; //constant delta for both fast/slow computers
	if(m_byKey[VK_LEFT]) newp+=CPoint(-delta,0); if(m_byKey[VK_RIGHT]) newp+=CPoint(delta,0);
	if(m_byKey[VK_UP]) newp+=CPoint(0,-delta); if(m_byKey[VK_DOWN]) newp+=CPoint(0,delta);
	if(newp!=GetScrollPosition()) { /*::Sleep(1);*/  ScrollToPosition(newp); }
	m_dwLastMomn=ex;
	return TRUE;
}

void CPrincedV3View::CenterView()
{
	CRect rct; int srcsx, srcsy; CPoint pp=GetDocument()->GetStartScreen();
	GetClientRect(rct); srcsx=m_nBlockX*CPrincedV3Doc::SCRX;
	srcsy=m_nBlockY*CPrincedV3Doc::SCRY; pp.x=(pp.x-1)*srcsx; pp.y=(pp.y-1)*srcsy;
	pp.x-=(rct.Width()-srcsx)/2; pp.y-=(rct.Height()-srcsy)/2;
	ScrollToPosition(pp);
}

void CPrincedV3View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int i; m_ptPrev=point; HandleEdit(point, nFlags);
	switch(m_nCurTool) {
	case ID_MAPTOOLS_ZOOM: //zoom in
		for(i=0;i+1<sizeof(m_nZfact)/sizeof(m_nZfact[0]);i++) if(m_nZfact[i]>m_nZoom) break;
		SetZoom(m_nZfact[i], point); break;
	default: SetCapture();
	}
}

void CPrincedV3View::OnMapTool(UINT nID)
{
	m_nCurTool=nID;
}

void CPrincedV3View::OnUpdateMapTool(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!(pCmdUI->m_nID==ID_MAPTOOLS_WALKABLE_BALDOZACONESQUELETO && m_bPalace));
	pCmdUI->SetRadio(m_nCurTool==pCmdUI->m_nID);
}

BOOL CPrincedV3View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UINT ind=0;
	if(nHitTest==HTCLIENT) //not on a scroll bar or anything else! :-P
		switch(m_nCurTool) { //different cursor for different map tools
		case ID_MAPTOOLS_DROPPER: ind=IDC_DROPPER; break;
		case ID_MAPTOOLS_ZOOM: ind=IDC_ZOOMTOOL; break;
		case ID_MAPTOOLS_ARRASTRAR: ind=IDC_HANDPALM; break;
		}

	if(ind!=0) { ::SetCursor(AfxGetApp()->LoadCursor(ind)); return FALSE; }
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CPrincedV3View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
}

void CPrincedV3View::OnRButtonDown(UINT nFlags, CPoint point) 
{
	int i;
	switch(m_nCurTool) {
	case ID_MAPTOOLS_ZOOM: //zoom out
		for(i=sizeof(m_nZfact)/sizeof(m_nZfact[0])-1;i>0;i--) if(m_nZfact[i]<m_nZoom) break;
		SetZoom(m_nZfact[i], point); break;
	}
}

void CPrincedV3View::HandleEdit(CPoint point, UINT nFlags)
{
	BOOL b=TRUE; CPrincedV3Doc::WALL wa; CPrincedV3Doc::ATTRIB at=CPrincedV3Doc::null;
	int sx,sy; CPoint pt=point; CPrincedV3Doc *pDoc=GetDocument();
	point+=GetScrollPosition(); sx=point.x/m_nBlockX, sy=point.y/m_nBlockY;

	switch(m_nCurTool) {
	case ID_MAPTOOLS_CLEAR: wa=CPrincedV3Doc::wNull; break; //draw-something tool
	case ID_MAPTOOLS_OBJECTS_POSSION: wa=CPrincedV3Doc::wPotion; at=CPrincedV3Doc::potRedSmall; break;

	case ID_MAPTOOLS_WALKABLE_NORMAL: wa=CPrincedV3Doc::wFloor; break; //normal floor
	case ID_MAPTOOLS_WALKABLE_BALDOZAROTA:
		switch(pDoc->GetWallAt(sx, sy)) {
		case CPrincedV3Doc::wFloorTorch: case CPrincedV3Doc::wBrokenFloorTorch:
			wa=CPrincedV3Doc::wBrokenFloorTorch; break;
		default: wa=CPrincedV3Doc::wBrokenFloor;
		}
		break; //broken floor
	case ID_MAPTOOLS_WALKABLE_BALDOZAFLOJA: wa=CPrincedV3Doc::wLooseFloor; break; //loose floor
	case ID_MAPTOOLS_WALKABLE_BALDOZAAPRETABLE: wa=CPrincedV3Doc::wDoorOpener; break; //door opener
	case ID_MAPTOOLS_WALKABLE_DOORCLOSER: wa=CPrincedV3Doc::wDoorCloser; break; //door closer
	case ID_MAPTOOLS_WALKABLE_PINCHES: wa=CPrincedV3Doc::wSpike; break; //spikes
	case ID_MAPTOOLS_WALKABLE_BALDOZACONESQUELETO: wa=CPrincedV3Doc::wSkeleton; break; //skeleton

	case ID_MAPTOOLS_GATES_DOOR: wa=CPrincedV3Doc::wDoor; at=CPrincedV3Doc::doorClosed; break;
	case ID_MAPTOOLS_GATES_PUERTASIERRA: wa=CPrincedV3Doc::wChopper; break; //chopper
	case ID_MAPTOOLS_SWORD: wa=CPrincedV3Doc::wSword;break;
	//case ID_MAPTOOLS_GATES_LEVELGATE: wa=CPrincedV3Doc::; break; //level gate. (NOTE: 2 blocks)
	case ID_MAPTOOLS_WALL: wa=CPrincedV3Doc::wWallBlock; break;
	case ID_MAPTOOLS_BACKGROUND_COLUMN: wa=CPrincedV3Doc::wOneColumn; break;
	//case ID_MAPTOOLS_BACKGROUND_BIGCOLUMN: wa=CPrincedV3Doc::wBigColumnUp; break; //(NOTE: 2 blocks)
	case ID_MAPTOOLS_BACKGROUND_LIGHT:
		switch(pDoc->GetWallAt(sx, sy)) {
		case CPrincedV3Doc::wBrokenFloor: case CPrincedV3Doc::wBrokenFloorTorch:
			wa=CPrincedV3Doc::wBrokenFloorTorch; break;
		default: wa=CPrincedV3Doc::wFloorTorch;
		}
		break;

	//and many background objects

	default: b=FALSE; break;
	}
	if(b) {
		CRect rct;
		rct.left=pt.x-m_nBlockX; rct.right=pt.x+m_nBlockX*2; //with original mouse point
		rct.top=pt.y-m_nBlockY; rct.bottom=pt.y+m_nBlockY*2;
		if(pDoc->GetWallAt(sx, sy)!=wa || pDoc->GetBackAt(sx, sy)!=at) {
			pDoc->PlaceObject(sx, sy, wa, at); InvalidateRect(rct); //invalidate only when necessary
		}
		pDoc->SetModifiedFlag();
	}
}

//Setting the view mode -- either the palace or the dungeon
void CPrincedV3View::OnViewMode(UINT nID)
{
	m_bPalace= nID==ID_VIEW_ASPALACE; Invalidate();
}

void CPrincedV3View::OnUpdateViewMode(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio( (pCmdUI->m_nID==ID_VIEW_ASDUNGEON) ^ m_bPalace);
}

void CPrincedV3View::OnTimer(UINT nIDEvent) 
{
	//TODO: implement scroll when dragging mouse outside the view
	
	CScrollView::OnTimer(nIDEvent);
}
