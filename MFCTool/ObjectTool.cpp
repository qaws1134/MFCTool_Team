// ObjectTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Form.h"


// CObjectTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
	, m_cstrName(_T(""))
	, m_iShotGunCount(0)
	, m_fShotGunAngle(0.f)
	, m_fMaxHp(0.f)
	, m_fObjAtk(0.f)
	, m_fObjAtkRatio(0.f)
	, m_fObjMoveSpeed(0.f)
	, m_bIsPlaying_DeathAnimation(false)
	, m_iDeathAnimationIndex(0)
{

}

CObjectTool::~CObjectTool()
{
	for (auto& rPair : m_mapObject)
		Safe_Delete(rPair.second);
	m_mapObject.clear();
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP_SG, m_GroupBox);
	DDX_Control(pDX, IDC_COMBO_BULLET_TYPE, m_BulletTypeSelectControl);
	DDX_Text(pDX, IDC_EDIT_OBJNAME, m_cstrName);
	DDX_Control(pDX, IDC_CHECK1_BULLET, m_CheckBoxDestructable);
	DDX_Text(pDX, IDC_EDIT_COUNT_SG, m_iShotGunCount);
	DDX_Text(pDX, IDC_EDIT_ANGLE_SG, m_fShotGunAngle);
	DDX_Control(pDX, IDC_LIST2_OBJ, m_ListBox_ObjectList);
	DDX_Control(pDX, IDC_LIST3_OBJ, m_ListBox_AnimList);
	DDX_Control(pDX, IDC_ANIM_PIC, m_PictureAnimation);
	DDX_Text(pDX, IDC_EDIT_OBJHP, m_fMaxHp);
	DDX_Text(pDX, IDC_EDIT_OBJATK, m_fObjAtk);
	DDX_Text(pDX, IDC_EDIT_OBJATKSPD, m_fObjAtkRatio);
	DDX_Text(pDX, IDC_EDIT_OBJMOVSPD, m_fObjMoveSpeed);
	DDX_Control(pDX, IDC_COMBO_OBJID, m_ComboOBJID);
	DDX_Control(pDX, IDC_BUTTON_OBJ_PLAYSTOP, m_ButtonAnimationPlay);
}

void CObjectTool::Update_Group_Bullet()
{
	int iIndex = m_ComboOBJID.GetCurSel();

	if (iIndex == OBJECTINFO::ENEMY_BULLET || iIndex == OBJECTINFO::PLAYER_BULLET)
	{
		GetDlgItem(IDC_GROUP_BULLET)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_COMBO_BULLET_TYPE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_CHECK1_BULLET)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_BULLET_TYPE)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_DESTRUCTABLE)->ShowWindow(SW_NORMAL);
	}
	else
	{
		GetDlgItem(IDC_GROUP_BULLET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BULLET_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK1_BULLET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_BULLET_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_DESTRUCTABLE)->ShowWindow(SW_HIDE);
	}
	Update_Group_ShotGun();
}

void CObjectTool::Update_Group_ShotGun()
{
	int iIndex = m_BulletTypeSelectControl.GetCurSel();

	if (iIndex == CB_ERR)
	{
		return;
	}
	CString strType;
	CString strShotgun = _T("2��ź");
	m_BulletTypeSelectControl.GetLBText(iIndex, strType);
	if (strType == strShotgun)
	{
		m_GroupBox.ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_ANGLE_SG)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_TEXT_COUNT_SG)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT_ANGLE_SG)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_EDIT_COUNT_SG)->ShowWindow(SW_NORMAL);
	}
	else
	{
		m_GroupBox.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_ANGLE_SG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_COUNT_SG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ANGLE_SG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_COUNT_SG)->ShowWindow(SW_HIDE);
	}
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_BULLET_TYPE, &CObjectTool::OnCbnSelchangeBulletType)
	ON_BN_CLICKED(IDC_BUTTON_OBJADD, &CObjectTool::OnBnClickedAdd)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST2_OBJ, &CObjectTool::OnLbnSelchangeObjectList)
	ON_BN_CLICKED(IDC_BUTTON_OBJDEL, &CObjectTool::OnBnClickedButtonObjectDelete)
	ON_BN_CLICKED(IDC_BUTTON_OBJSAVE, &CObjectTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOAD, &CObjectTool::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OBJLOADANIM, &CObjectTool::OnBnClickedButtonLoadAnimation)
	ON_LBN_SELCHANGE(IDC_LIST3_OBJ, &CObjectTool::OnLbnSelchangeAnimation)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_PLAYSTOP, &CObjectTool::OnBnClickedButtonObjPlaystop)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_OBJID, &CObjectTool::OnCbnSelchangeComboObjid)
END_MESSAGE_MAP()


// CObjectTool �޽��� ó�����Դϴ�.


void CObjectTool::OnCbnSelchangeBulletType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Update_Group_ShotGun();
}


//Bullet Add
void CObjectTool::OnBnClickedAdd()
{
	UpdateData(TRUE);
	if (m_cstrName.IsEmpty())
	{
		ERR_MSG(L"�̸��� ä����������");
		return;
	}

	int iObjID = m_ComboOBJID.GetCurSel();
	if (iObjID == CB_ERR)
	{
		ERR_MSG(L"������Ʈ Ÿ���� �������ּ���");
		return;
	}

	int iDeathAnimationIndex = m_ListBox_AnimList.GetCurSel();
	if (iDeathAnimationIndex == LB_ERR)
	{
		ERR_MSG(L"��� �ִϸ��̼��� �������ּ���");
		return;
	}

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	CString cstrName;
	m_ListBox_AnimList.GetText(iDeathAnimationIndex, cstrName);
	int i = 0;
	// "->" ���� ����
	for (; i < cstrName.GetLength(); ++i)
	{
		if (cstrName[i] == L'>')
			break;
	}
	cstrName.Delete(i - 1, 2);
	auto& iter_find = map.find(cstrName);
	if (iter_find == map.end())
	{
		ERR_MSG(L"�ִϸ��̼��� ã�� ���߽��ϴ�.");
		return;
	}

	int iBulletTypeIndex = m_BulletTypeSelectControl.GetCurSel();

	if (iBulletTypeIndex == CB_ERR )
	{
		iBulletTypeIndex = 0;
	}

	OBJECTINFO*	pObjectData = new OBJECTINFO;
	pObjectData->cstrName = m_cstrName;
	pObjectData->fMaxHp = m_fMaxHp;
	pObjectData->fAtk = m_fObjAtk;
	pObjectData->fAtkRatio = m_fObjAtkRatio;
	pObjectData->fMoveSpeed = m_fObjMoveSpeed;
	pObjectData->eObjId = (OBJECTINFO::OBJID)iObjID;
	pObjectData->cstrDeathAnimImage_ObjectKey = iter_find->second->wstrObjectKey;
	pObjectData->cstrDeathAnimImage_StateKey = iter_find->second->wstrStateKey;
	

	//Bullet Data	
	pObjectData->bDestructable = (bool)m_CheckBoxDestructable.GetCheck();
	pObjectData->eBulletType = (OBJECTINFO::BULLET_TYPE)iBulletTypeIndex;

	//ShotGun Data
	if (pObjectData->eBulletType == OBJECTINFO::BULLET_TYPE::SHOTGUN)
	{
		pObjectData->iShotGunCount = m_iShotGunCount;
		pObjectData->fShotGunAngle = m_fShotGunAngle;
	}
	else
	{
		pObjectData->iShotGunCount = 0;
		pObjectData->fShotGunAngle = 0.f;
	}
	auto& iter = m_mapObject.find(m_cstrName);
	if (iter != m_mapObject.end())
	{
		Safe_Delete(iter->second);
		m_mapObject.erase(iter);
	}
	else
	{
		m_ListBox_ObjectList.AddString(m_cstrName);
	}
		
	m_mapObject.emplace(m_cstrName, pObjectData); // Add�� ������ ����

}


void CObjectTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);
	int iShotGunCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};

	for (int i = 0; i < iShotGunCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);
		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
	}
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}

void CObjectTool::OnLbnSelchangeObjectList()
{
	OnBnClickedButtonLoadAnimation();

	int iObjectIndex = m_ListBox_ObjectList.GetCurSel();

	CString cstrName;
	m_ListBox_ObjectList.GetText(iObjectIndex, cstrName);

	auto& iter = m_mapObject.find(cstrName);
	if (iter == m_mapObject.end())
		return;

	OBJECTINFO* pObjectData = iter->second;
		
	m_cstrName = pObjectData->cstrName;
	m_fMaxHp = pObjectData->fMaxHp;
	m_fObjAtk = pObjectData->fAtk;
	m_fObjAtkRatio = pObjectData->fAtkRatio;
	m_fObjMoveSpeed = pObjectData->fMoveSpeed;
	m_ComboOBJID.SetCurSel(pObjectData->eObjId);

	int findIndex = m_ListBox_AnimList.FindStringExact(-1, pObjectData->cstrDeathAnimImage_ObjectKey + L"->" + pObjectData->cstrDeathAnimImage_StateKey);
	if (findIndex != -1)
		m_ListBox_AnimList.SetCurSel(findIndex);
	
	

	m_BulletTypeSelectControl.SetCurSel(pObjectData->eBulletType);
	m_CheckBoxDestructable.SetCheck(pObjectData->bDestructable);

	Update_Group_Bullet();
	Update_Group_ShotGun();
	if (pObjectData->eBulletType == OBJECTINFO::BULLET_TYPE::SHOTGUN)
	{
		m_iShotGunCount = pObjectData->iShotGunCount;
		m_fShotGunAngle = pObjectData->fShotGunAngle;
	}
	else
	{
		m_iShotGunCount = 0;
		m_fShotGunAngle = 0.f;
	}
	OnLbnSelchangeAnimation();
	UpdateData(FALSE);
}


void CObjectTool::OnBnClickedButtonObjectDelete()
{
	int iIndex = m_ListBox_ObjectList.GetCurSel();
	
	if (iIndex == LB_ERR)
	{
		ERR_MSG(L"������ ����� ������ �ּ���");
		return;
	}

	CString cstrName;
	m_ListBox_ObjectList.GetText(iIndex, cstrName);
	
	auto& iter = m_mapObject.find(cstrName);
	if (iter == m_mapObject.end())
	{
		ERR_MSG(L"ListBox���� ���������� Map���� �������� �ʴ� Ű���Դϴ�");
		return;
	}
	
	Safe_Delete(iter->second);
	m_mapObject.erase(iter);

	m_ListBox_ObjectList.DeleteString(iIndex);
}


void CObjectTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE,// FALSE�� �ٸ��̸����� ����. 
		L"dat",
		L"ObjectData.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data");
	//PathCombine(szFilePath, szFilePath, L"Data"); 
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		//CString strPath = Dlg.GetFileName(); // ������ �̸���!
		//strPath = Dlg.GetPathName();// ������ ��� + �̸����� 
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwbyte = 0;
		DWORD strLen;
		// �� ���� �־�. �ʳ� ������ . // �޷� ��������
		for (auto& rPair : m_mapObject)
		{
			OBJECTINFO* pObject = rPair.second;
			strLen = pObject->cstrName.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrName.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrObjectImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrObjectImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrDeathAnimImage_ObjectKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrDeathAnimImage_ObjectKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			strLen = pObject->cstrDeathAnimImage_StateKey.GetLength() + 1;
			WriteFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, pObject->cstrDeathAnimImage_StateKey.GetString(), sizeof(TCHAR) * strLen, &dwbyte, nullptr);

			WriteFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->fShotGunAngle, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &pObject->iShotGunCount, sizeof(int), &dwbyte, nullptr);
		}
		CloseHandle(hFile);
	}
}


void CObjectTool::OnBnClickedButtonLoad()
{
	OnBnClickedButtonLoadAnimation();

	CFileDialog Dlg(TRUE,// FALSE�� �ٸ��̸����� ����. 
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
		//CString strPath = Dlg.GetFileName(); // ������ �̸���!
		//strPath = Dlg.GetPathName();// ������ ��� + �̸����� 
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		for (auto& rPair : m_mapObject)
		{
			Safe_Delete(rPair.second);
		}
		m_mapObject.clear();
		m_ListBox_ObjectList.ResetContent();

		DWORD dwbyte = 0;
		DWORD strLen;
		OBJECTINFO* pObject;
		TCHAR *pBuff;
		// �� ���� �־�. �ʳ� ������ . // �޷� ��������
		while (true)
		{
			
			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);

			if (dwbyte == 0)
				break;

			pObject = new OBJECTINFO;

			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrName = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrObjectImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);


			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrDeathAnimImage_ObjectKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &strLen, sizeof(DWORD), &dwbyte, nullptr);
			pBuff = new TCHAR[strLen]{};
			ReadFile(hFile, pBuff, sizeof(TCHAR) * strLen, &dwbyte, nullptr);
			pObject->cstrDeathAnimImage_StateKey = pBuff;
			Safe_Delete(pBuff);

			ReadFile(hFile, &pObject->fMaxHp, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fAtk, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fAtkRatio, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fMoveSpeed, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->eObjId, sizeof(BYTE), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->bDestructable, sizeof(bool), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->eBulletType, sizeof(BYTE), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->fShotGunAngle, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pObject->iShotGunCount, sizeof(int), &dwbyte, nullptr);

			m_mapObject.emplace(pObject->cstrName, pObject);
			m_ListBox_ObjectList.AddString(pObject->cstrName);
		}
		CloseHandle(hFile);
	}
}


void CObjectTool::OnBnClickedButtonLoadAnimation()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString,ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;
	
	for (auto& rPair : map)
	{
		CString cstrTemp = rPair.second->wstrObjectKey + L"->" + rPair.second->wstrStateKey;
		if(m_ListBox_AnimList.FindStringExact(-1,cstrTemp.GetString()) == -1)
			m_ListBox_AnimList.AddString(cstrTemp);
	}

	//Animation Map �ε��ڵ� ����
}


void CObjectTool::OnLbnSelchangeAnimation()
{
	int iIndex = m_ListBox_AnimList.GetCurSel();

	CString wstrFindName;
	m_ListBox_AnimList.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" ���� ����
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	auto& iter_find = map.find(wstrFindName);

	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(iter_find->second->wstrObjectKey.GetString(), iter_find->second->wstrStateKey.GetString(), m_iDeathAnimationIndex);
	if (nullptr == pTexInfo)
		return;
	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, WINCX / TILECX, WINCX / TILECX, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_PictureAnimation.m_hWnd);
}

void CObjectTool::OnBnClickedButtonObjPlaystop()
{
	int iIndex = m_ListBox_AnimList.GetCurSel();

	if (iIndex == LB_ERR)
		return;

	if (!m_bIsPlaying_DeathAnimation)
	{
		m_bIsPlaying_DeathAnimation = true;
		SetTimer(1000, 0 ,NULL);
		m_ButtonAnimationPlay.SetWindowText(L"Stop");
	}
	else
	{
		m_bIsPlaying_DeathAnimation = false;
		KillTimer(1000);
		m_ButtonAnimationPlay.SetWindowText(L"Play");
	}
}


void CObjectTool::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent != 1000)
		return;

	int iIndex = m_ListBox_AnimList.GetCurSel();

	CString wstrFindName;
	m_ListBox_AnimList.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" ���� ����
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map< CString, ANIMATION*>& map = pForm->m_tAnimationTool.m_mapAnima;

	auto& iter_find = map.find(wstrFindName);

	iter_find->second->fPlay_Speed;
	++m_iDeathAnimationIndex;
	if (m_iDeathAnimationIndex >= iter_find->second->iMax_Index)
		m_iDeathAnimationIndex = 0;

	OnLbnSelchangeAnimation();
	SetTimer(1000, iter_find->second->fPlay_Speed * 1000.f, NULL);
	CDialog::OnTimer(nIDEvent);
}


void CObjectTool::OnCbnSelchangeComboObjid()
{
	Update_Group_Bullet();
}
