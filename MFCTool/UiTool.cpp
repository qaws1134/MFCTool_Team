// UiTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UiTool.h"
#include "afxdialogex.h"
#include "MFCToolView.h"

// CUiTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUiTool, CDialog)

CUiTool::CUiTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UITOOL, pParent), m_strID(_T(""))
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotZ(0)
	, m_fScaleX(0)
	, m_fScaleY(0)
	, m_bMatLine(false)

	, m_wstrObjID(_T(""))
{
	


}

CUiTool::~CUiTool()
{
	for (pair<CString, PLACEMENT*> rPair : m_mapPlacementInfo)
		Safe_Delete(rPair.second);
	m_mapPlacementInfo.clear();
}

void CUiTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1_PLACE, m_ComboID);
	DDX_Control(pDX, IDC_LIST1_PLACE, m_Image_ListBox);
	DDX_Control(pDX, IDC_PICTURE_PLACE, m_Picture);
	DDX_Control(pDX, IDC_LIST2_PLACE, m_Prefab_ListBox);
	DDX_Control(pDX, IDC_LIST4_PLACE, m_Result_ListBox);
	DDX_Text(pDX, IDC_EDIT7_PLACE, m_fPosX);
	DDX_Text(pDX, IDC_EDIT10_PLACE, m_fPosY);
	DDX_Text(pDX, IDC_EDIT13_PLACE, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT8_PLACE, m_fRotX);
	DDX_Text(pDX, IDC_EDIT11_PLACE, m_fRotY);
	DDX_Text(pDX, IDC_EDIT14_PLACE, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT9_PLACE, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT12_PLACE, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT1_PLACE, m_wstrObjID);
}


BEGIN_MESSAGE_MAP(CUiTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON22_PLACE, &CUiTool::OnBnClickedPrefabLoad)
	ON_BN_CLICKED(IDC_BUTTON8_PLACE, &CUiTool::OnBnClickedTranslation)
	ON_BN_CLICKED(IDC_BUTTON9_PLACE, &CUiTool::OnBnClickedRotation)
	ON_BN_CLICKED(IDC_BUTTON17_PLACE, &CUiTool::OnBnClickedScale)
	ON_BN_CLICKED(IDC_BUTTON18_PLACE, &CUiTool::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON19_PLACE, &CUiTool::OnBnClickedClear)
	ON_BN_CLICKED(IDC_BUTTON20_PLACE, &CUiTool::OnBnClickedResultSave)
	ON_BN_CLICKED(IDC_BUTTON21_PLACE, &CUiTool::OnBnClickedResultLoad)
	ON_BN_CLICKED(IDC_BUTTON1_PLACE, &CUiTool::OnBnClickedApply)
	ON_LBN_SELCHANGE(IDC_LIST1_PLACE, &CUiTool::OnLbnSelchangeImageListBox)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST4_PLACE, &CUiTool::OnLbnSelchangeResultList)
END_MESSAGE_MAP()


// CUiTool 메시지 처리기입니다.




void CUiTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);
	int iDropIndex = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);
	TCHAR szFileFullPath[MAX_PATH]{};

	for (int i = 0; i < iDropIndex; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);

		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);

		TCHAR szFileName[MAX_PATH] = {};	
		lstrcpy(szFileName, wstrFileNameAndEx.GetString());
		PathRemoveExtension(szFileName);
		m_Image_ListBox.AddString(szFileName);
		if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, wstrRelativePath.GetString(), szFileName)))
		{
			ERR_MSG(L"싱글 텍스쳐 실패");
			return;
		}
		m_mapFileInfo.emplace(szFileName, wstrRelativePath);
	}
	m_Image_ListBox.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CUiTool::OnLbnSelchangeImageListBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//인덱스에 있는 파일 텍스쳐에 추가 후 출력
	int iListCursor = m_Image_ListBox.GetCurSel();
	CString wstrFileName;
	m_Image_ListBox.GetText(iListCursor, wstrFileName);
	SetImageView(wstrFileName.GetString(), m_Picture);
}



//스테틱 박스에 이미지 출력을 위한 함수
void CUiTool::SetImageView(CString Objectkey, const CStatic& PictureBox)
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
		->Get_TexInfo(Objectkey.GetString());
	if (nullptr == pTexInfo)
		return;
	float m_ScaleX= 1.f;
	float m_ScaleY= 1.f;

	if (pTexInfo->tImageInfo.Width > WINCX || pTexInfo->tImageInfo.Height > WINCY)
	{
		m_ScaleX = 0.5f;
		m_ScaleY = 0.5f;
	}
	else
	{
		m_ScaleX = float(WINCX/ pTexInfo->tImageInfo.Width);
		m_ScaleY = float( WINCY/ pTexInfo->tImageInfo.Height);
	}


	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, m_ScaleX, m_ScaleY, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Render_End(PictureBox.m_hWnd);
}


// 툴뷰에 띄어줄 이미지의 메트릭스를 정의 
void CUiTool::Render_UI()
{
	UpdateData(TRUE);
	int iMax_Idx = m_Result_ListBox.GetCount();
	if (0 == iMax_Idx)	
		return;

	for(int i= 0; i <iMax_Idx; i++)
	{
		CString wstrName;
		m_Result_ListBox.GetText(i, wstrName);
		auto& iter = m_mapPlacementInfo.find(wstrName);
		if (iter == m_mapPlacementInfo.end())
		{
			ERR_MSG(L"맵에 해당 키가 없다");
			return;
		}

		if (!iter->second->m_bRender)
			return;
		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
			->Get_TexInfo(iter->second->wstrObjectKey.GetString());

		D3DXMATRIX matScale, matTrans, matRotX, matRotY, matRotZ, matWorld;

		//초기화
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matRotX);
		D3DXMatrixIdentity(&matRotY);
		D3DXMatrixIdentity(&matRotZ);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		D3DXMatrixScaling(&matScale, iter->second->m_tMatInfo.mat[MATID::SCALE].x, iter->second->m_tMatInfo.mat[MATID::SCALE].y, 0.f);

		if (0 != iter->second->m_tMatInfo.mat[MATID::ROT].x)
			D3DXMatrixRotationX(&matRotX, D3DXToRadian(iter->second->m_tMatInfo.mat[MATID::ROT].x));
		else if (0 != iter->second->m_tMatInfo.mat[MATID::ROT].y)
			D3DXMatrixRotationY(&matRotY, D3DXToRadian(iter->second->m_tMatInfo.mat[MATID::ROT].y));
		else if (0 != iter->second->m_tMatInfo.mat[MATID::ROT].z)
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(iter->second->m_tMatInfo.mat[MATID::ROT].z));

		D3DXMatrixTranslation(&matTrans, iter->second->m_tMatInfo.mat[MATID::TRANS].x - m_pView->GetScrollPos(SB_HORZ), iter->second->m_tMatInfo.mat[MATID::TRANS].y - m_pView->GetScrollPos(SB_VERT), iter->second->m_tMatInfo.mat[MATID::TRANS].z);
		matWorld = matScale *matRotX*matRotY*matRotZ* matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3{ fCenterX,fCenterY,0.f }, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	UpdateData(FALSE);
}

void CUiTool::PickingPos(D3DXVECTOR3 vMouse)
{
	int iIdx = m_Result_ListBox.GetCurSel();
	if (iIdx != LB_ERR)	//선택된게 있으면 
	{
		CString wstrName;
		m_Result_ListBox.GetText(iIdx, wstrName);
		auto& iter = m_mapPlacementInfo.find(wstrName);
		if (iter == m_mapPlacementInfo.end())
		{
			ERR_MSG(L"맵에 해당 키가 없다");
			return;
		}
		iter->second->m_tMatInfo.mat[MATID::TRANS]= vMouse;
		//해당 이미지는 그려라
		iter->second->m_bRender = true;	
	}
}




void CUiTool::OnBnClickedApply()
{
	UpdateData(TRUE);
	int iIdx = m_Result_ListBox.GetCurSel();
	CString wstrName;
	m_Result_ListBox.GetText(iIdx, wstrName);
	auto& iter = m_mapPlacementInfo.find(wstrName);

	if (iter == m_mapPlacementInfo.end())
	{
		ERR_MSG(L"맵에 해당 키가 없다");
		return;
	}
	iter->second->m_tMatInfo.mat[MATID::TRANS] = { m_fPosX ,m_fPosY,m_fPosZ };
	iter->second->m_tMatInfo.mat[MATID::ROT] = { m_fRotX,m_fRotY,m_fRotZ };
	iter->second->m_tMatInfo.mat[MATID::SCALE] = { m_fScaleX,m_fScaleY ,0.f };
	iter->second->m_bRender = true;
	m_pView->Invalidate(FALSE);
	UpdateData(FALSE);
}



void CUiTool::OnBnClickedAdd()
{
	UpdateData(TRUE);
	PLACEMENT* pPlacementInfo = new PLACEMENT{};
	//콤보 박스 선택
 	int iIdx = m_ComboID.GetCurSel();
	CString strID;
	m_ComboID.GetLBText(iIdx, strID);
	m_strID = strID;
	
	//이미지 리스트 선택
	int iImageIdx = m_Image_ListBox.GetCurSel();
	CString Image_Key;
	m_Image_ListBox.GetText(iImageIdx, Image_Key);

	//오브젝트 이름 가져오기
	CString ObjKey = m_wstrObjID.GetString();


	if (LB_ERR == iIdx || LB_ERR == iImageIdx)
	{
		ERR_MSG(L"리스트 박스 선택을 안했다");
		return;
	}




	auto& iter_find = m_mapFileInfo.find(Image_Key);

	pPlacementInfo->wstrObjectKey = iter_find->first;
	pPlacementInfo->wstrFilePath = iter_find->second;



	if (strID == L"BACKGROUND")
		pPlacementInfo->eRenderID = RENDERID::BACKGROUND;
	else if(strID == L"UI")
		pPlacementInfo->eRenderID = RENDERID::UI;
	else if (strID == L"OBJECT")
	{
		pPlacementInfo->eRenderID = RENDERID::OBJECT;

		//프리팹 리스트박스 선택
		int iPrefabIdx = m_Prefab_ListBox.GetCurSel();
		if (LB_ERR == iPrefabIdx)
		{
			ERR_MSG(L"프리팹 선택 안함");
			return;
		}
		CString PrefabName;
		m_Prefab_ListBox.GetText(iPrefabIdx, PrefabName);
		//이름 가져와서 키값으로 정보 저장 
		pPlacementInfo->tPrafabInfo = m_tPrefabInfo;
	}

	m_mapPlacementInfo.emplace(ObjKey, pPlacementInfo);
	m_Result_ListBox.AddString(ObjKey);
	UpdateData(FALSE);
}


void CUiTool::OnBnClickedClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CUiTool::OnBnClickedTranslation()
{
	if (m_bMatLine)
	{
		m_bMatLine = false;
		return;
	}
	m_bMatLine = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	D3DXVECTOR2 vLinePosX[3] = { { 0.f, 0.f },{ 200.f, 0.f},{-10.f,-10.f} };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosX, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DXVECTOR2 vLinePosY[3] = { { 0.f, 0.f },{ 0.f, 200.f },{ -10.f,10.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosY, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

}


void CUiTool::OnBnClickedRotation()
{
	//if (m_bMatLine)
	//{
	//	m_bMatLine = false;
	//	return;
	//}
	//m_bMatLine = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	D3DXVECTOR2 vLinePosX[3] = { { 100.f, 100.f },{ 200.f, 100.f },{ -10.f,-10.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosX, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DXVECTOR2 vLinePosY[3] = { { 100.f, 100.f },{ 100.f, 200.f },{ -10.f,10.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosY, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DXVECTOR2 vLinePosZ[3] = { { 100.f, 100.f },{ 200.f, 200.f },{ -10.f,0.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosZ, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}


void CUiTool::OnBnClickedScale()
{
	if (m_bMatLine)
	{
		m_bMatLine = false;
		return;
	}
	m_bMatLine = true;
	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	D3DXVECTOR2 vLinePosX[3] = { { 0.f, 0.f },{ 200.f, 0.f },{ -10.f,-10.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosX, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DXVECTOR2 vLinePosY[3] = { { 0.f, 0.f },{ 0.f, 200.f },{ -10.f,10.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosY, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}



// 프리팹 로드 
void CUiTool::OnBnClickedPrefabLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 
void CUiTool::OnBnClickedResultSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CUiTool::OnBnClickedResultLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CUiTool::OnLbnSelchangeResultList()
{
	
}
