// Form.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "Form.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON6, &CForm::OnBnClickedMapTool)
	ON_BN_CLICKED(IDC_BUTTON9, &CForm::OnBnClickedUiTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CForm::OnBnClickedAnimationTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CForm::OnBnClickedBulletTool)
END_MESSAGE_MAP()


// CForm �����Դϴ�.

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm �޽��� ó�����Դϴ�.

void CForm::OnInitialUpdate() // �̰͵� ���� ������ ��. 
{
	CFormView::OnInitialUpdate();
	if (nullptr == m_tMapTool.GetSafeHwnd())
		m_tMapTool.Create(IDD_MAPTOOL);
	if (nullptr == m_tAnimationTool.GetSafeHwnd())
		m_tAnimationTool.Create(IDD_ANIMATIONTOOL);
	if (nullptr == m_tObjectTool.GetSafeHwnd())
		m_tObjectTool.Create(IDD_OBJECTTOOL);
	if (nullptr == m_tUiTool.GetSafeHwnd())
		m_tUiTool.Create(IDD_UITOOL);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}

// MapTool 
void CForm::OnBnClickedMapTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_tMapTool.ShowWindow(SW_SHOW);
}


void CForm::OnBnClickedUiTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_tUiTool.ShowWindow(SW_SHOW);
}


void CForm::OnBnClickedAnimationTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_tAnimationTool.ShowWindow(SW_SHOW);
}


void CForm::OnBnClickedBulletTool()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_tObjectTool.ShowWindow(SW_SHOW);
}
