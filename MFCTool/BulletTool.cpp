// BulletTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "BulletTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"


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
	for (auto& rPair : m_mapBullet)
		Safe_Delete(rPair.second);
	m_mapBullet.clear();
}

void CBulletTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP, m_GroupBox);
	DDX_Control(pDX, IDC_COMBO1, m_BulletTypeSelectControl);
	DDX_Text(pDX, IDC_EDIT1_BULLET, m_cstrName);
	DDX_Text(pDX, IDC_EDIT2_BULLET, m_iAtk);
	DDX_Text(pDX, IDC_EDIT3_BULLET, m_fSpeed);
	DDX_Control(pDX, IDC_CHECK1_BULLET, m_CheckBoxDestructable);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_iShotGunCount);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_fShotGunAngle);
	DDX_Control(pDX, IDC_LIST1_BULLET, m_ListBox_BulletImage);
	DDX_Control(pDX, IDC_LIST2_BULLET, m_ListBox_BulletList);
	DDX_Control(pDX, IDC_LIST3_BULLET, m_ListBox_AnimList);
	DDX_Control(pDX, IDC_BT_PIC, m_BulletPicture);
	DDX_Control(pDX, IDC_ANIM_PIC, m_PictureAnimation);
}


BEGIN_MESSAGE_MAP(CBulletTool, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CBulletTool::OnCbnSelchangeBulletType)
	ON_BN_CLICKED(IDC_BUTTON1_BULLET, &CBulletTool::OnBnClickedAdd)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1_BULLET, &CBulletTool::OnLbnSelchangeBulletImage)
	ON_LBN_SELCHANGE(IDC_LIST2_BULLET, &CBulletTool::OnLbnSelchangeBulletList)
	ON_BN_CLICKED(IDC_BUTTON12_BULLET, &CBulletTool::OnBnClickedButtonBulletDelete)
	ON_BN_CLICKED(IDC_BUTTON10_BULLET, &CBulletTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON11_BULLET, &CBulletTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON9_BULLET, &CBulletTool::OnBnClickedButtonLoadAnimation)
	ON_LBN_SELCHANGE(IDC_LIST3_BULLET, &CBulletTool::OnLbnSelchangeAnimation)
	ON_BN_CLICKED(IDC_BUTTON2_BULLET, &CBulletTool::OnBnClickedButtonDeleteImage)
END_MESSAGE_MAP()


// CBulletTool 메시지 처리기입니다.


void CBulletTool::OnCbnSelchangeBulletType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_BulletTypeSelectControl.GetCurSel();
	CString strType;
	CString strShotgun = _T("2산탄");
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


//Bullet Add
void CBulletTool::OnBnClickedAdd()
{
	UpdateData(TRUE);
	if (m_cstrName.IsEmpty())
	{
		ERR_MSG(L"이름을 채워넣으세요");
		return;
	}

	int iTypeIndex = m_BulletTypeSelectControl.GetCurSel();

	if (iTypeIndex == CB_ERR )
	{
		ERR_MSG(L"총알 타입을 선택해주세요");
		return;
	}
	
	int iImageIndex = m_ListBox_BulletImage.GetCurSel();
		
	if (iImageIndex == LB_ERR)
	{
		ERR_MSG(L"총알 이미지를 선택해주세요");
		return;
	}
	
	CString cstrImageObjectKey;
	m_ListBox_BulletImage.GetText(iImageIndex, cstrImageObjectKey);

	BULLETDATA*	pBulletData = new BULLETDATA;
	pBulletData->bDestructable = m_CheckBoxDestructable.GetCheck();
	pBulletData->cstrName = m_cstrName;
	pBulletData->iAtk = m_iAtk;
	pBulletData->fSpeed = m_fSpeed;
	pBulletData->eType = (BULLETDATA::TYPE)iTypeIndex;
	pBulletData->cstrBulletImage_ObjectKey = cstrImageObjectKey;
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
	auto& iter = m_mapBullet.find(m_cstrName);
	if (iter != m_mapBullet.end())
	{
		Safe_Delete(iter->second);
		m_mapBullet.erase(iter);
	}
	else
	{
		m_ListBox_BulletList.AddString(m_cstrName);
	}
		
	m_mapBullet.emplace(m_cstrName, pBulletData); // Add는 수정도 가능

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
		int iResult = m_ListBox_BulletImage.FindStringExact(-1, szFileName);
		if (iResult != LB_ERR) // 중복추가 방지
		{
			continue;
		}
		else
		{
			CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, wstrRelativePath.GetString(), szFileName);
		}
			
		m_ListBox_BulletImage.AddString(szFileName);
	}
	m_ListBox_BulletImage.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CBulletTool::OnLbnSelchangeBulletImage()
{
	UpdateData(TRUE);
	int iIndex = m_ListBox_BulletImage.GetCurSel();

	CString cstrName;
	m_ListBox_BulletImage.GetText(iIndex, cstrName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(cstrName.GetString());
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / TILECX, WINCX / TILECX, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_BulletPicture.m_hWnd);
	
}


void CBulletTool::OnLbnSelchangeBulletList()
{
	int iBulletIndex = m_ListBox_BulletList.GetCurSel();

	CString cstrName;
	m_ListBox_BulletList.GetText(iBulletIndex, cstrName);

	auto& iter = m_mapBullet.find(cstrName);
	if (iter == m_mapBullet.end())
		return;
	BULLETDATA* pBulletData = iter->second;
	
	
	m_cstrName = pBulletData->cstrName;
	m_iAtk = pBulletData->iAtk;
	m_fSpeed = pBulletData->fSpeed;
	m_CheckBoxDestructable.SetCheck(pBulletData->bDestructable);
	m_BulletTypeSelectControl.SetCurSel(pBulletData->eType);
	m_iShotGunCount = pBulletData->iCount;
	m_fShotGunAngle = pBulletData->fSpeed;

	int iPictureIndex = m_ListBox_BulletImage.FindString(-1, pBulletData->cstrBulletImage_ObjectKey);

	m_ListBox_BulletImage.SetCurSel(iPictureIndex);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(pBulletData->cstrBulletImage_ObjectKey.GetString());
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / TILECX, WINCX / TILECX, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_BulletPicture.m_hWnd);

	UpdateData(FALSE);
}


void CBulletTool::OnBnClickedButtonBulletDelete()
{
	int iIndex = m_ListBox_BulletList.GetCurSel();
	
	if (iIndex == LB_ERR)
	{
		ERR_MSG(L"삭제할 대상을 선택해 주세요");
		return;
	}

	CString cstrName;
	m_ListBox_BulletList.GetText(iIndex, cstrName);
	
	auto& iter = m_mapBullet.find(cstrName);
	if (iter == m_mapBullet.end())
	{
		ERR_MSG(L"ListBox에는 존재하지만 Map에는 존재하지 않는 키값입니다");
		return;
	}
	
	Safe_Delete(iter->second);
	m_mapBullet.erase(iter);

	m_ListBox_BulletList.DeleteString(iIndex);

}


void CBulletTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE,// FALSE가 다른이름으로 저장. 
		L"dat",
		L"BulletData.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data");
	//PathCombine(szFilePath, szFilePath, L"Data"); 
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		//CString strPath = Dlg.GetFileName(); // 파일의 이름만!
		//strPath = Dlg.GetPathName();// 파일의 경로 + 이름까지 
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwbyte = 0;
		DWORD strLen;
		// 난 쓸수 있어. 너넨 쓰지마 . // 메롱 쓸거지롱
		for (auto& rPair : m_mapBullet)
		{
			BULLETDATA* pBullet = rPair.second;
			strLen = pBullet->cstrName.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pBullet->cstrName.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pBullet->cstrBulletImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pBullet->cstrBulletImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pBullet->cstrAnimImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pBullet->cstrAnimImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pBullet->cstrAnimImage_StateKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pBullet->cstrAnimImage_StateKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			WriteFile(hFile, &pBullet->iAtk, sizeof(int), &dwbyte, nullptr);
			WriteFile(hFile, &pBullet->fSpeed, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pBullet->bDestructable, sizeof(bool), &dwbyte, nullptr);
			WriteFile(hFile, &pBullet->eType, sizeof(BYTE), &dwbyte, nullptr);
			WriteFile(hFile, &pBullet->fAngle, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pBullet->iCount, sizeof(int), &dwbyte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CBulletTool::OnBnClickedButtonLoad()
{
	CFileDialog Dlg(TRUE,// FALSE가 다른이름으로 저장. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data");
	//PathCombine(szFilePath, szFilePath, L"Data"); 
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		//CString strPath = Dlg.GetFileName(); // 파일의 이름만!
		//strPath = Dlg.GetPathName();// 파일의 경로 + 이름까지 
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		for (auto& rPair : m_mapBullet)
		{
			Safe_Delete(rPair.second);
		}
		m_mapBullet.clear();
		m_ListBox_BulletList.ResetContent();

		DWORD dwbyte = 0;
		DWORD strLen;
		BULLETDATA* pBullet;
		TCHAR *pBuff;
		// 난 쓸수 있어. 너넨 쓰지마 . // 메롱 쓸거지롱
		while (true)
		{
			
			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);

			if (dwbyte == 0)
				break;

			pBullet = new BULLETDATA;

			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pBullet->cstrName = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pBullet->cstrBulletImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);


			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pBullet->cstrAnimImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pBullet->cstrAnimImage_StateKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &pBullet->iAtk, sizeof(int), &dwbyte, nullptr);
			ReadFile(hFile, &pBullet->fSpeed, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pBullet->bDestructable, sizeof(bool), &dwbyte, nullptr);
			ReadFile(hFile, &pBullet->eType, sizeof(BYTE), &dwbyte, nullptr);
			ReadFile(hFile, &pBullet->fAngle, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pBullet->iCount, sizeof(int), &dwbyte, nullptr);

			m_mapBullet.emplace(pBullet->cstrName, pBullet);
			m_ListBox_BulletList.AddString(pBullet->cstrName);
		}
		CloseHandle(hFile);
	}
}


void CBulletTool::OnBnClickedButtonLoadAnimation()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	//pForm->m_tAnimationTool
	//Animation Map 로드코드 구간
}


void CBulletTool::OnLbnSelchangeAnimation()
{
	UpdateData(TRUE);
	int iIndex = m_ListBox_AnimList.GetCurSel();

	CString cstrName;
	m_ListBox_AnimList.GetText(iIndex, cstrName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(cstrName.GetString());
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / TILECX, WINCX / TILECX, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = pTexInfo->tImageInfo.Width >> 1;
	float fCenterY = pTexInfo->tImageInfo.Height >> 1;
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_PictureAnimation.m_hWnd);
}


void CBulletTool::OnBnClickedButtonDeleteImage()
{
	int iIndex = m_ListBox_BulletImage.GetCurSel();
	
	if (iIndex == LB_ERR)
		return;

	m_ListBox_BulletList.DeleteString(iIndex);
}
