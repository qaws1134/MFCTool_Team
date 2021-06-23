// AnimationTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"


// CAnimationTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAnimationTool, CDialog)

CAnimationTool::CAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANIMATIONTOOL, pParent)
	, m_wstrObject_Key(_T("")), m_wstrState_Key(_T(""))
	,m_fPlay_Speed(0.f),m_iMax_Index(0)
	, m_iInput_Index(0)
	, m_iDrawID(0), m_iAnimationCount(0)
	, m_iPlay_Speed(0)
{

}

CAnimationTool::~CAnimationTool()
{

	for (pair<CString, ANIMATION*> rPair : m_mapAnima)
		Safe_Delete(rPair.second);
	m_mapAnima.clear();

}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_wstrObject_Key);
	DDX_Text(pDX, IDC_EDIT2, m_wstrState_Key);
	DDX_Text(pDX, IDC_EDIT3, m_fPlay_Speed);
	DDX_Text(pDX, IDC_EDIT4, m_iMax_Index);
	DDX_Text(pDX, IDC_EDIT5, m_iInput_Index);

	DDX_Control(pDX, IDC_START_IMAGE, m_Start_Image);
	DDX_Control(pDX, IDC_END_IMAGE, m_End_Image);
	DDX_Control(pDX, IDC_INDEX_TO_SELECTED_IMAGE, m_Index_To_Selected_Image);
	DDX_Control(pDX, IDC_LIST_TO_SELECTED_IMAGE, m_List_To_Selected_Image);

	DDX_Control(pDX, IDC_LIST2, m_Image_ListBox);
	DDX_Control(pDX, IDC_LIST1, m_Animation_ListBox);

	DDX_Control(pDX, IDC_CHECK2, m_IsSingle);
	DDX_Control(pDX, IDC_CHECK1, m_Loop);
	DDX_Text(pDX, IDC_EDIT6, m_iPlay_Speed);
}


BEGIN_MESSAGE_MAP(CAnimationTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON5, &CAnimationTool::OnBnClickedIndex_Apply)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimationTool::OnBnClickedAddAnimation)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimationTool::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimationTool::OnBnClickedLoad)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAnimationTool::OnLbnSelchangeImageList)

	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationTool::OnLbnSelchangeAnimation_List)
	ON_WM_DROPFILES()

	ON_BN_CLICKED(IDC_BUTTON10_ANIM, &CAnimationTool::OnBnClickedAddInfo)
	ON_BN_CLICKED(IDC_BUTTON11, &CAnimationTool::OnBnClickedDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON12, &CAnimationTool::OnBnClickedDeleteAnimationList)
	ON_BN_CLICKED(IDC_BUTTON13, &CAnimationTool::OnBnClickedClearBox)
	ON_BN_CLICKED(IDC_BUTTON14, &CAnimationTool::OnBnClickedClear_Info)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON16, &CAnimationTool::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON15, &CAnimationTool::OnBnClickedStop)
END_MESSAGE_MAP()


// addinfo ��ư Ŭ��
void CAnimationTool::OnBnClickedAddInfo()
{
	UpdateData(TRUE);
	//==========animation ���� �Է�=======================
	ANIMATION* pAnima = new ANIMATION{};
	CString AnimationName = m_wstrObject_Key + m_wstrState_Key; //�ΰ�objkey�� statekey�� ���Ѱ��� Ű�� 

	pAnima->wstrObjectKey = m_wstrObject_Key;
	pAnima->wstrStateKey = m_wstrState_Key;
	pAnima->fPlay_Speed = m_fPlay_Speed;
	pAnima->iMax_Index = m_iMax_Index;

	if (m_Loop.GetCheck())
		pAnima->bLoop = true;
	else
		pAnima->bLoop = false;

	if (m_IsSingle.GetCheck())
		pAnima->bIsSingle = true;
	else
		pAnima->bIsSingle = false;
	//=====================================================

	m_mapAnima.emplace(AnimationName, pAnima);
	ERR_MSG(L"���� �Է� ����");
	UpdateData(FALSE);
}


void CAnimationTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE);
	int iShotGunCount = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};

	m_Drop = hDropInfo;


	for (int i = 0; i < iShotGunCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);

		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);

		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		m_Image_ListBox.AddString(szFileName);
	}
	m_Image_ListBox.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);



	CString wstrSingleFilePath = CFileInfo::ConvertRelativePath(szFileFullPath);
	
	//===============��Ƽ �ؽ��� ���� �̸� ����====================
	CString wstrFileNameAndEx = PathFindFileName(wstrSingleFilePath);
	CString wstrMultiFilePath = wstrSingleFilePath;
	
	TCHAR szFileName[MAX_PATH]{};
	lstrcpy(szFileName, wstrFileNameAndEx.GetString());
	int iPathLength = wstrMultiFilePath.GetLength() - lstrlen(szFileName);
	wstrMultiFilePath.Delete(iPathLength, lstrlen(szFileName));
	PathRemoveExtension(szFileName);

	int i = lstrlen(szFileName)-1;
	for (; i>=0; i--)
	{
		if (!isdigit(szFileName[i]))
		{
			break;
		}
		szFileName[i] = NULL;
	}
	lstrcat(szFileName, L"%d.png");

	wstrMultiFilePath.Insert(iPathLength, szFileName);
	
	//===========================================
	//�̹��� insert
	if (m_IsSingle.GetCheck())
	{
		if (m_wstrObject_Key != L"")
		{
			m_wstrFilePath = wstrSingleFilePath;
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, wstrSingleFilePath.GetString(), m_wstrObject_Key.GetString())))
			{
				ERR_MSG(L"�̱� �ؽ��� ����");
				return;
			}
		}
	}
	else
	{
		if (m_wstrObject_Key != L""&& m_wstrState_Key != L"")
		{
			m_wstrFilePath = wstrMultiFilePath;
			if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, wstrMultiFilePath.GetString(), m_wstrObject_Key.GetString(), m_wstrState_Key.GetString(), iShotGunCount)))
			{
				ERR_MSG(L"��Ƽ �ؽ��� ����");
				return;
			}
		}

	}
}


//������� �̹��� ����Ʈ ���� �� Ȯ��
void CAnimationTool::OnLbnSelchangeImageList()
{
	int iListCursor = m_Image_ListBox.GetCurSel();
	CString wstrFileName;
	m_Image_ListBox.GetText(iListCursor, wstrFileName);

	int i = wstrFileName.GetLength()-1;
	for (; i >= 0; --i)
	{
		if (!isdigit(wstrFileName[i]))
			break;
	}
	wstrFileName.Delete(0, i+1);
	m_iDrawID = _ttoi(wstrFileName.GetString());
	SetImageView(m_wstrObject_Key.GetString(), m_iDrawID, m_List_To_Selected_Image, m_wstrState_Key.GetString());
}


//����ƽ �ڽ��� �̹��� ����� ���� �Լ�
void CAnimationTool::SetImageView(CString Objectkey, int Index,const CStatic& PictureBox, CString StateKey)
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
		->Get_TexInfo(Objectkey.GetString(), StateKey.GetString(), Index);
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
	CGraphic_Device::Get_Instance()->Render_End(PictureBox.m_hWnd);
}

//������ ���½�Ŵ
void CAnimationTool::Reset_Info()
{

	m_wstrObject_Key = L"";
	m_wstrState_Key = L"";
	m_fPlay_Speed = 0.f;
	m_iMax_Index = 0;
	m_IsSingle.SetCheck(FALSE);
	m_Loop.SetCheck(FALSE);

}

//�̹��� ����Ʈ�ڽ��� ���½�Ŵ
void CAnimationTool::Reset_ImageList()
{
	if (m_Image_ListBox.GetCount() != 0)
		m_Image_ListBox.ResetContent();
}

//�ִϸ��̼� �������� ���ļ� �߰�
//���� ���� map�� ���� �� listbox�� �߰� 
void CAnimationTool::OnBnClickedAddAnimation()
{
	UpdateData(TRUE);

	auto& iter_find = m_mapAnima.find(m_wstrObject_Key + m_wstrState_Key);
	//�ʿ� Ű�� ���� �� ���� 
	if (iter_find == m_mapAnima.end())
	{
		ERR_MSG(L"�ʿ� ��ġ�ϴ� Ű�� ����");
		return;
	}
	//���� ����Ʈ�� ������ �ȵǾ������� ����
	if (m_wstrFilePath.IsEmpty())
	{
		ERR_MSG(L"���� ��� ����Ǿ� ���� �ʴ�");
		return;
	}
	//���� ���� ����
	iter_find->second->wstrFilePath = m_wstrFilePath;

	//����Ʈ Ŭ���� �� �ֱ�
	//�߰� �� �̹��� ����Ʈ �ڽ� ���� 
	for (int i = 0; i <m_Image_ListBox.GetCount();i++)
		m_Image_ListBox.ResetContent();


	m_Animation_ListBox.AddString(m_wstrObject_Key+"->"+m_wstrState_Key);

	//start_Image�� End_Index �̹��� ��� 
	Reset_Info();


	UpdateData(FALSE);
}

//�ε����� �ִ� �׸��� Ȯ����
void CAnimationTool::OnBnClickedIndex_Apply()
{
	UpdateData(TRUE);
	int iIndex = m_Animation_ListBox.GetCurSel();

	if (iIndex == LB_ERR)
		return;

	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" ���� ����
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	auto& iter_find = m_mapAnima.find(wstrFindName);

	SetImageView(iter_find->second->wstrObjectKey, m_iInput_Index, m_Index_To_Selected_Image, iter_find->second->wstrStateKey);
	UpdateData(FALSE);

}


void CAnimationTool::OnBnClickedDeleteAll()
{
	UpdateData(TRUE);

	//������ �̹��� �ڽ� ���� ���� 
	auto& iter_find = m_mapAnima.find(m_wstrObject_Key+m_wstrState_Key);
	CString wstrKey = iter_find->second->wstrObjectKey + "->" + iter_find->second->wstrStateKey;
	for (int i = 0; i < m_Animation_ListBox.GetCount(); i++)
	{
		CString wstrBoxKey;
		m_Animation_ListBox.GetText(i, wstrBoxKey);
		if (wstrKey == wstrBoxKey)
		{
			ERR_MSG(L"�̹� ���� ����Ʈ�� ���⼭ ����� �ȵȴ�");
			return;
		}
	}

	if (iter_find == m_mapAnima.end())
	{
		ERR_MSG(L"��ġ�ϴ� ������ ����");
		return;
	}
	Reset_ImageList();
	Safe_Delete(iter_find->second);
	m_mapAnima.erase(iter_find);

	Reset_Info();
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE,// FALSE�� �ٸ��̸����� ����. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;
	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		//�ϳ��� ���� �ؾ���. ���� ���鼭 . 
		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;
		// �� ���� �־�. �ʳ� ������ . 
		for (auto& rPair : m_mapAnima)
		{

			dwStringSize = (rPair.second->wstrObjectKey.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrObjectKey.GetString(), dwStringSize, &dwbyte, nullptr);

			dwStringSize = (rPair.second->wstrStateKey.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrStateKey.GetString(), dwStringSize, &dwbyte, nullptr);

			dwStringSize = (rPair.second->wstrFilePath.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrFilePath.GetString(), dwStringSize, &dwbyte, nullptr);


			WriteFile(hFile, &rPair.second->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->iMax_Index, sizeof(int), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->bLoop, sizeof(bool), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->bIsSingle, sizeof(bool), &dwbyte, nullptr);
		}
		CloseHandle(hFile);
	}

}


void CAnimationTool::OnBnClickedLoad()
{
	CFileDialog Dlg(TRUE,// TRUE�� ����. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);

	TCHAR szFilePath[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);
	lstrcat(szFilePath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szFilePath;

	if (IDOK == Dlg.DoModal())
	{
		CString strFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;
		for (auto& rPair : m_mapAnima)
			Safe_Delete(rPair.second);
		m_mapAnima.clear();
		m_Animation_ListBox.ResetContent(); 

		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;
		ANIMATION* pAnimaInfo = 0;
		TCHAR* pBuf = nullptr;

		while (true)
		{
			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			if (0 == dwbyte)
				break;
			pBuf = new TCHAR[dwStringSize];
			pAnimaInfo = new ANIMATION;
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pAnimaInfo->wstrObjectKey = pBuf;
			Safe_Delete(pBuf);
				

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pAnimaInfo->wstrStateKey = pBuf;
			Safe_Delete(pBuf);

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pAnimaInfo->wstrFilePath = pBuf;
			Safe_Delete(pBuf);


			ReadFile(hFile, &pAnimaInfo->fPlay_Speed, sizeof(float), &dwbyte, nullptr);
			ReadFile(hFile, &pAnimaInfo->iMax_Index, sizeof(int), &dwbyte, nullptr);
			ReadFile(hFile, &pAnimaInfo->bLoop, sizeof(bool), &dwbyte, nullptr);
			ReadFile(hFile, &pAnimaInfo->bIsSingle, sizeof(bool), &dwbyte, nullptr);

			m_mapAnima.emplace(pAnimaInfo->wstrObjectKey+pAnimaInfo->wstrStateKey, pAnimaInfo);
			m_Animation_ListBox.AddString(pAnimaInfo->wstrObjectKey +"->"+pAnimaInfo->wstrStateKey);


			//==============�̱� �ؽ��� ���� �̸� ����===============
			CString wstrSingleFilePath = pAnimaInfo->wstrFilePath;

			//===============��Ƽ �ؽ��� ���� �̸� ����==============
			CString wstrFileNameAndEx = PathFindFileName(wstrSingleFilePath);
			CString wstrMultiFilePath = wstrSingleFilePath;

			TCHAR szFileName[MAX_PATH]{};
			lstrcpy(szFileName, wstrFileNameAndEx.GetString());
			int iPathLength = wstrMultiFilePath.GetLength() - lstrlen(szFileName);
			wstrMultiFilePath.Delete(iPathLength, lstrlen(szFileName));
			PathRemoveExtension(szFileName);

			lstrcat(szFileName, L".png");

			wstrMultiFilePath.Insert(iPathLength, szFileName);


			//===============�̹��� Insert==============
			if (pAnimaInfo->bIsSingle)
			{
				if (pAnimaInfo->wstrObjectKey.GetString() != L"")
				{
					if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, wstrSingleFilePath.GetString(), pAnimaInfo->wstrObjectKey.GetString())))
					{
						ERR_MSG(L"�̱� �ؽ��� ����");
						return;
					}
				}
			}
			else
			{
				if (pAnimaInfo->wstrObjectKey.GetString() != L""&& pAnimaInfo->wstrStateKey.GetString() != L"")
				{
					if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::MULTI_TEX, wstrMultiFilePath.GetString(), pAnimaInfo->wstrObjectKey.GetString(), pAnimaInfo->wstrStateKey.GetString(), pAnimaInfo->iMax_Index)))
					{
						ERR_MSG(L"��Ƽ �ؽ��� ����");
						return;
					}
				}
			}
		}
		CloseHandle(hFile);
	}
}


//�ϼ��� �ִϸ��̼� ����Ʈ�ڽ� Ȯ��
void CAnimationTool::OnLbnSelchangeAnimation_List()
{
	UpdateData(TRUE);
	Reset_Info();
	Reset_ImageList();

	int iIndex = m_Animation_ListBox.GetCurSel();
	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);

	// Ű���� -> ���� ���� 
	int i = 0;
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}

	wstrFindName.Delete(i-1, 2);
	auto& iter_find = m_mapAnima.find(wstrFindName);

	m_wstrObject_Key = iter_find->second->wstrObjectKey;
	m_wstrState_Key = iter_find->second->wstrStateKey;
	m_fPlay_Speed = iter_find->second->fPlay_Speed;
	m_iMax_Index = iter_find->second->iMax_Index;

	
	if (iter_find->second->bLoop)
		m_Loop.SetCheck(true);
	else
		m_Loop.SetCheck(false);
	if (iter_find->second->bIsSingle)
		m_IsSingle.SetCheck(true);
	else
		m_IsSingle.SetCheck(false);


	//�̹��� ����Ʈ�ڽ��� ����� ���� ���� ��� 
	int iShotGunCount = iter_find->second->iMax_Index;
	TCHAR szFileFullPath[MAX_PATH]{};

	for (int i = 0; i < iShotGunCount; ++i)
	{
		TCHAR szFilePath[MAX_PATH] = L"";
		swprintf_s(szFilePath, iter_find->second->wstrFilePath, i);
		
		CString wstrFileNameAndEx = PathFindFileName(szFilePath);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		
		m_Image_ListBox.AddString(szFileName);
	}

	m_Image_ListBox.SetHorizontalExtent(500);

	//start �̹���, end �̹��� ��� 
	SetImageView(iter_find->second->wstrObjectKey.GetString(), 0, m_Start_Image, iter_find->second->wstrStateKey.GetString());
	SetImageView(iter_find->second->wstrObjectKey.GetString(), iter_find->second->iMax_Index - 1, m_End_Image, iter_find->second->wstrStateKey.GetString());

	UpdateData(FALSE);
}

void CAnimationTool::OnBnClickedDeleteAnimationList()
{
	UpdateData(TRUE);

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iIndex = m_Animation_ListBox.GetCurSel();
	if (iIndex == LB_ERR)
	{
		ERR_MSG(L"����Ʈ ���� ������");
		return;
	}
	CString wstrFindName;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);
	int i = 0;
	// "->" ���� ����
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);

	auto& iter_find = m_mapAnima.find(wstrFindName);

	Safe_Delete(iter_find->second);
	m_mapAnima.erase(iter_find);

	Reset_Info();
	Reset_ImageList();

	m_Animation_ListBox.DeleteString(iIndex);

	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedClearBox()
{
	UpdateData(TRUE);
	Reset_ImageList();
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedClear_Info()
{
	UpdateData(TRUE);
	Reset_Info();
	UpdateData(FALSE);
}


void CAnimationTool::OnTimer(UINT_PTR nIDEvent)
{

	CString wstrFindName;
	int i = 0;
	int iIndex = m_Animation_ListBox.GetCurSel();

	if (iIndex == LB_ERR)
		return;
	m_Animation_ListBox.GetText(iIndex, wstrFindName);
	// "->" ���� ����
	for (; i < wstrFindName.GetLength(); ++i)
	{
		if (wstrFindName[i] == L'>')
			break;
	}
	wstrFindName.Delete(i - 1, 2);
	auto& iter_find = m_mapAnima.find(wstrFindName);

	if ((nIDEvent)<100)
	{
		SetImageView(iter_find->second->wstrObjectKey, m_iAnimationCount, m_Index_To_Selected_Image, iter_find->second->wstrStateKey);
		if (++m_iAnimationCount >= iter_find->second->iMax_Index)
			m_iAnimationCount = 0;
	}

	CDialog::OnTimer(nIDEvent);
}


void CAnimationTool::OnBnClickedPlay()
{
	UpdateData(TRUE);
	SetTimer(m_iTimer++, m_iPlay_Speed, NULL);
	UpdateData(FALSE);
}


void CAnimationTool::OnBnClickedStop()
{
	KillTimer(m_iKillTimer++);
}
