// BulletTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "BulletTool.h"
#include "afxdialogex.h"


// CBulletTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBulletTool, CDialog)

CBulletTool::CBulletTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BULLETTOOL, pParent)
	, m_cstrName(_T(""))
	, m_iAtk(0)
	, m_fSpeed(0)
	, m_iShotGunCount(0)
	, m_fShotGunAngle(0)
{

}

CBulletTool::~CBulletTool()
{
}

void CBulletTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP, m_GroupBox);
	DDX_Control(pDX, IDC_COMBO1, m_BulletTypeSelectControl);
	DDX_Text(pDX, IDC_EDIT1, m_cstrName);
	DDX_Text(pDX, IDC_EDIT2, m_iAtk);
	DDX_Text(pDX, IDC_EDIT3, m_fSpeed);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBoxDestructable);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_iShotGunCount);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_fShotGunAngle);
	DDX_Control(pDX, IDC_LIST1, m_ListBox_BulletImage);
	DDX_Control(pDX, IDC_LIST2, m_ListBox_BulletList);
	DDX_Control(pDX, IDC_LIST3, m_ListBox_AnimList);
}


BEGIN_MESSAGE_MAP(CBulletTool, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CBulletTool::OnCbnSelchangeBulletType)
	ON_BN_CLICKED(IDC_BUTTON1, &CBulletTool::OnBnClickedAdd)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CBulletTool 메시지 처리기입니다.


void CBulletTool::OnCbnSelchangeBulletType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_BulletTypeSelectControl.GetCurSel();
	CString strType;
	CString strShotgun = _T("산탄");
	m_BulletTypeSelectControl.GetLBText(iIndex, strType);
	if (strType == strShotgun)
	{
		m_GroupBox.ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_ANGLE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_COUNT)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT_ANGLE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT_COUNT)->ShowWindow(SW_NORMAL);
	}
	else
	{
		m_GroupBox.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ANGLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_COUNT)->ShowWindow(SW_HIDE);
	}
}


void CBulletTool::OnBnClickedAdd()
{
	if (m_cstrName.IsEmpty())
	{
		ERR_MSG(L"이름을 채워넣으세요");
		return;
	}

	int iIndex = m_BulletTypeSelectControl.GetCurSel();

	if (iIndex == CB_ERR )
	{
		ERR_MSG(L"총알 타입을 선택해주세요");
		return;
	}
	
	
		
	
	ERR_MSG(L"이미지 만들고 다시 오세요");
	return;

	BULLETDATA*	pBulletData = new BULLETDATA;
	pBulletData->bDestructable = m_CheckBoxDestructable.GetCheck();
	pBulletData->cstrName = m_cstrName;
	pBulletData->eType = (BULLETDATA::TYPE)iIndex;
	if (pBulletData->eType == BULLETDATA::TYPE::SHOTGUN)
	{
		pBulletData->iCount = m_iShotGunCount;
		pBulletData->fAngle = m_fShotGunAngle;
	}
	else
	{
		pBulletData->iCount = 0;
		pBulletData->fAngle = 0.f;
	}
}


void CBulletTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);
	int iCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};

	for (int i = 0; i < iCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);
		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		if (m_ListBox_BulletImage.FindStringExact(-1, szFileName) == LB_ERR)
			continue;
		m_ListBox_BulletImage.AddString(szFileName);
	}
	m_ListBox_BulletImage.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}
