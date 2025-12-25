// GrpBtn3.cpp : implementation file
//

#include "stdafx.h"
#include "GrpBtn3Test.h"
#include "GrpBtn3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3

CGrpBtn3::CGrpBtn3()
{
	State=GB_NORMAL;
}

CGrpBtn3::~CGrpBtn3()
{
	DeleteAllBitmap();
}


BEGIN_MESSAGE_MAP(CGrpBtn3, CWnd)
	//{{AFX_MSG_MAP(CGrpBtn3)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrpBtn3 message handlers

void CGrpBtn3::SetImage(WORD Normal, WORD Hot, WORD Down, WORD Disable)
{
	DeleteAllBitmap();
	Bitmap[0].LoadBitmap(Normal);
	Bitmap[1].LoadBitmap(Hot);
	Bitmap[2].LoadBitmap(Down);
	Bitmap[3].LoadBitmap(Disable);
	Invalidate(FALSE);
}

void CGrpBtn3::DeleteAllBitmap()
{
	int i;

	for (i=0;i<4;i++) {
		if (Bitmap[i].m_hObject) {
			Bitmap[i].DeleteObject();
		}
	}
}

void CGrpBtn3::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if ((GetWindowLong(m_hWnd,GWL_STYLE) & GBS_CHECK)==0) {
		ChangeState(GB_DOWN);
	} else {
		CheckState=State;
		ChangeState(CheckState==GB_NORMAL ? GB_DOWN:GB_NORMAL);
	}
	SetCapture();
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CGrpBtn3::OnMouseMove(UINT nFlags, CPoint point) 
{
	HWND hParent;

	if (GetCapture()==this) {
		if ((GetWindowLong(m_hWnd,GWL_STYLE) & GBS_CHECK) == 0) {
			if (IsPtOnMe(point)) {
				ChangeState(GB_DOWN);
			} else {
				ChangeState(GB_NORMAL);
			}
		} else {
			if (IsPtOnMe(point)) {
				ChangeState(CheckState==GB_NORMAL ? GB_DOWN:GB_NORMAL);
			} else {
				ChangeState(CheckState);
			}
		}
	} else {
		for (hParent=m_hWnd;::GetParent(hParent);hParent=::GetParent(hParent)) {;}
		if (::GetForegroundWindow() != hParent) return;
		if ((GetWindowLong(m_hWnd,GWL_STYLE) & GBS_CHECK)==0) {
			SetTimer(0,50,NULL);
			ChangeState(GB_HOT);
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGrpBtn3::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (GetCapture()==this) {
		ReleaseCapture();
		if ((GetWindowLong(m_hWnd,GWL_STYLE) & GBS_CHECK)==0) {
			ChangeState(GB_NORMAL);
		}
		if (IsPtOnMe(point)) {
			GetParent()->SendMessage(WM_COMMAND,MAKEWPARAM(
				GetWindowLong(m_hWnd,GWL_ID),GBN_CLICKED),(LPARAM)m_hWnd);
		}
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CGrpBtn3::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawBitmap(&dc,0,0,&Bitmap[State]);
}

void CGrpBtn3::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	CRect crt;

	GetCursorPos(&pt);
	GetWindowRect(&crt);
	if (crt.PtInRect(pt)==FALSE) {
		KillTimer(0);
		ChangeState(GB_NORMAL);
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CGrpBtn3::DrawBitmap(CDC *pDC, int x, int y, CBitmap *pBit)
{
	BITMAP bitinfo;
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap;
	pOldBitmap=MemDC.SelectObject(pBit);
	pBit->GetBitmap(&bitinfo);
	pDC->BitBlt(x,y,bitinfo.bmWidth,bitinfo.bmHeight,&MemDC,0,0,SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
}

BOOL CGrpBtn3::IsPtOnMe(CPoint point)
{
	CRect wrt;

	GetWindowRect(&wrt);
	ClientToScreen(&point);

	return wrt.PtInRect(point);
}

void CGrpBtn3::ChangeState(eState State)
{
	if (this->State == State) return;

	this->State = State;
	Invalidate(FALSE);
}

eState CGrpBtn3::GetState()
{
	return State;
}

void CGrpBtn3::SetState(eState State)
{
	if ((GetWindowLong(m_hWnd,GWL_STYLE) & GBS_CHECK)!=0) {
		ChangeState(State);
	}
}

void CGrpBtn3::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	if (bEnable) {
		State=GB_NORMAL;
	} else {
		State=GB_DISABLE;
		KillTimer(0);
	}
	Invalidate(FALSE);
}

