// Miniview.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Miniview.h"
#include "MainFrm.h"
#include "MFCToolView.h"
#include "Terrain.h"

// CMiniview

IMPLEMENT_DYNCREATE(CMiniview, CView)

CMiniview::CMiniview()
{

}

CMiniview::~CMiniview()
{
}

BEGIN_MESSAGE_MAP(CMiniview, CView)
END_MESSAGE_MAP()


// CMiniview �׸����Դϴ�.

void CMiniview::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	CGraphic_Device::Get_Instance()->Render_Begin(); 
	// SplitterWnd -> Getpane
	// MainApp -> MainFrame -> MainSplitterWnd ���ü� �ְ� -> MFCToolView -> Terrain 
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd()); 
	CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));
	pView->m_pTerrain->MiniRender_Terrain(); 
	CGraphic_Device::Get_Instance()->Render_End(m_hWnd); 
}


// CMiniview �����Դϴ�.

#ifdef _DEBUG
void CMiniview::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMiniview::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMiniview �޽��� ó�����Դϴ�.
