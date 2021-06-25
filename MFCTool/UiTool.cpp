// UiTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "UiTool.h"
#include "afxdialogex.h"
#include "MFCToolView.h"
#include "MainFrm.h"
#include "Form.h"
// CUiTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUiTool, CDialog)

CUiTool::CUiTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UITOOL, pParent)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fRotZ(0)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_KeyIndex(0)
	, m_wstrObjID(_T(""))
	, m_bPicKingStart(false)
	, m_bPicking(false)
	, m_wstrMatMod(_T("Mouse"))
	, m_iNowResultIdx(0)
	, m_bArrowX(false)
	, m_bArrowY(false)
	, m_bCircle(false)
	, m_bMouseDown(false)
	, m_beIDObject(false)

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
	DDX_Text(pDX, IDC_EDIT10_PLACE, m_fPosY);;
	DDX_Text(pDX, IDC_EDIT14_PLACE, m_fRotZ);
	DDX_Text(pDX, IDC_EDIT9_PLACE, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT12_PLACE, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT1_PLACE, m_wstrObjID);
	DDX_Text(pDX, IDC_MAT_MOD_PLACE, m_wstrMatMod);
	DDX_Control(pDX, IDC_PICTURE_PLACE2, m_Picture_Prefab);
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
	ON_BN_CLICKED(IDC_BUTTON1, &CUiTool::OnBnClickedMouseTrans)
	ON_LBN_SELCHANGE(IDC_LIST2_PLACE, &CUiTool::OnLbnSelchangePrefabList)
END_MESSAGE_MAP()


// CUiTool �޽��� ó�����Դϴ�.




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
			ERR_MSG(L"�̱� �ؽ��� ����");
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//�ε����� �ִ� ���� �ؽ��Ŀ� �߰� �� ���
	int iListCursor = m_Image_ListBox.GetCurSel();
	CString wstrFileName;
	m_Image_ListBox.GetText(iListCursor, wstrFileName);
	SetImageView(wstrFileName.GetString(), m_Picture);

}

//����ƽ �ڽ��� �̹��� ����� ���� �Լ�
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


// ���信 ����� �̹����� ��Ʈ������ ���� 
void CUiTool::Render_UI()
{
	int iMax_Idx = m_Result_ListBox.GetCount();
	if (0 == iMax_Idx)
		return;

	for (int i = 0; i < iMax_Idx; i++)
	{
		CString wstrName;
		m_Result_ListBox.GetText(i, wstrName);
		auto& iter = m_mapPlacementInfo.find(wstrName);
		if (iter == m_mapPlacementInfo.end())
		{
			ERR_MSG(L"�ʿ� �ش� Ű�� ����");
			return;
		}

		if (!iter->second->m_bRender)
			continue;

		const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
			->Get_TexInfo(iter->second->wstrObjectKey.GetString());

		D3DXMATRIX matScale, matTrans, matRotZ, matWorld;

		//�ʱ�ȭ
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matRotZ);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		D3DXMatrixScaling(&matScale, iter->second->m_tMatInfo.mat[MATID::SCALE].x, iter->second->m_tMatInfo.mat[MATID::SCALE].y, 0.f);
		D3DXMatrixRotationZ(&matRotZ, -D3DXToRadian(iter->second->m_tMatInfo.mat[MATID::ROT].z));
		D3DXMatrixTranslation(&matTrans, iter->second->m_tMatInfo.mat[MATID::TRANS].x - m_pView->GetScrollPos(SB_HORZ), iter->second->m_tMatInfo.mat[MATID::TRANS].y - m_pView->GetScrollPos(SB_VERT), 0.f);
		matWorld = matScale *matRotZ* matTrans;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3{ fCenterX,fCenterY,0.f }, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


		if (i == m_iNowResultIdx)
		{
			if (m_bMatTrans|| m_bMatScale)
			{
				D3DXVECTOR3 vPos = iter->second->m_tMatInfo.mat[MATID::TRANS];
				vPos.x -= m_pView->GetScrollPos(SB_HORZ);
				vPos.y -= m_pView->GetScrollPos(SB_VERT);
				CGraphic_Device::Get_Instance()->Get_Sprite()->End();
				D3DXVECTOR2 vLinePosX[3] = { { vPos.x,vPos.y }
				,{ vPos.x + 100.f, vPos.y },{ vPos.x + 100.f - 10.f,vPos.y - 10.f } };
				CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosX, 3, D3DCOLOR_ARGB(255, 255, 0, 0));
				D3DXVECTOR2 vLinePosY[3] = { { vPos.x, vPos.y },{ vPos.x,vPos.y - 100.f },{ vPos.x - 10.f,vPos.y - 100 + 10.f } };
				CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosY, 3, D3DCOLOR_ARGB(255, 0, 255, 0));
				CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
			}
			if (m_bMatRot)
			{
				D3DXVECTOR3 vPos = iter->second->m_tMatInfo.mat[MATID::TRANS];
				vPos.x -= m_pView->GetScrollPos(SB_HORZ);
				vPos.y -= m_pView->GetScrollPos(SB_VERT);
				CGraphic_Device::Get_Instance()->Get_Sprite()->End();
				float fAngle = 360.f;
				D3DXVECTOR2	vCirclePos[31];
				for (int i = 0; i < 30; i++)
				{			
					vCirclePos[i].x = vPos.x + cosf(D3DXToRadian(( 360.f/ 30.f)*i))*100.f;
					vCirclePos[i].y = vPos.y - sinf(D3DXToRadian((360.f / 30.f )*i))*100.f;
				}
				vCirclePos[30] = vCirclePos[0];
				CGraphic_Device::Get_Instance()->Get_Line()->Draw(vCirclePos, 31, D3DCOLOR_ARGB(255, 0, 255, 0));
				CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
			}

		}
	}
}

bool CUiTool::ColArrow(D3DXVECTOR3 vMouse, RECT rc)
{
	//���ο�� �浹 üũ
	CString wstrResultName;
	int iresultIdx = m_Result_ListBox.GetCurSel();
	m_Result_ListBox.GetText(iresultIdx, wstrResultName);

	auto& iter_find = m_mapPlacementInfo.find(wstrResultName);

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
		->Get_TexInfo(iter_find->second->wstrObjectKey.GetString());
	

	float fArrowLeft	= iter_find->second->m_tMatInfo.mat[MATID::TRANS].x + (float)rc.left;
	float fArrowTop		= iter_find->second->m_tMatInfo.mat[MATID::TRANS].y + (float)rc.top;
	float fArrowRight	= iter_find->second->m_tMatInfo.mat[MATID::TRANS].x + (float)rc.right;
	float fArrowBottom	= iter_find->second->m_tMatInfo.mat[MATID::TRANS].y + (float)rc.bottom;
	//{-5.f,-5.f,100.f,5.f}; X
	//{-5.f,-100.f,5.f,5.f}; Y
 
	//float fArrowLeft = iter_fine->second->m_tMatInfo.mat[MATID::TRANS].x-5.f ;
	//float fArrowUp = iter_fine->second->m_tMatInfo.mat[MATID::TRANS].y - 5.f;
	//float fArrowRight = iter_fine->second->m_tMatInfo.mat[MATID::TRANS].x +100.f;
	//float fArrowBottom = iter_fine->second->m_tMatInfo.mat[MATID::TRANS].y +5.f;

	RECT MouseColRect = { (LONG)vMouse.x - 5,(LONG)vMouse.y - 5,(LONG)vMouse.x + 5,(LONG)vMouse.y + 5 };
	RECT ArrowXColRect = { (LONG)fArrowLeft,(LONG)fArrowTop,(LONG)fArrowRight,(LONG)fArrowBottom };
	RECT rect = {};
	if (IntersectRect(&rect, &MouseColRect, &ArrowXColRect))
		return true;
	return false;
}

bool CUiTool::ColCircle(D3DXVECTOR3 vMouse)
{
	//���ο�� �浹 üũ
	CString wstrResultName;
	int iresultIdx = m_Result_ListBox.GetCurSel();
	m_Result_ListBox.GetText(iresultIdx, wstrResultName);

	auto& iter_find = m_mapPlacementInfo.find(wstrResultName);

	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()
		->Get_TexInfo(iter_find->second->wstrObjectKey.GetString());

	D3DXVECTOR3 vDis = vMouse-iter_find->second->m_tMatInfo.mat[MATID::TRANS];
	float fDia = D3DXVec3Length(&vDis);

	float fDis = float((100 + 10));

	if (fDia <= fDis)
		return true;
	return false;
}

void CUiTool::Collision_Move(D3DXVECTOR3 vMouse)
{
	if (m_bMouseDown)
	{
		CString wstrResultName;
		int iresultIdx = m_Result_ListBox.GetCurSel();
		m_Result_ListBox.GetText(iresultIdx, wstrResultName);

		auto& iter_find = m_mapPlacementInfo.find(wstrResultName);
		if (m_bArrowX)
		{
			if (m_bMatTrans)
			{	
			
				iter_find->second->m_tMatInfo.mat[MATID::TRANS].x = m_vStartPos.x+(vMouse.x- m_vStartMouse.x);
				UpdateData(TRUE);
				m_fPosX = iter_find->second->m_tMatInfo.mat[MATID::TRANS].x;
				UpdateData(FALSE);
			}
			if (m_bMatScale)
			{
				iter_find->second->m_tMatInfo.mat[MATID::SCALE].x = ( vMouse.x - m_vStartMouse.x)/100;
				UpdateData(TRUE);
				m_fScaleX = iter_find->second->m_tMatInfo.mat[MATID::SCALE].x;
				UpdateData(FALSE);
			}
		}
		if (m_bArrowY)
		{
			if (m_bMatTrans)
			{
				iter_find->second->m_tMatInfo.mat[MATID::TRANS].y = m_vStartPos.y + (vMouse.y - m_vStartMouse.y);
				UpdateData(TRUE);
				m_fPosY = iter_find->second->m_tMatInfo.mat[MATID::TRANS].y;
				UpdateData(FALSE);
			}
			if (m_bMatScale)
			{
				iter_find->second->m_tMatInfo.mat[MATID::SCALE].y = (vMouse.y - m_vStartMouse.y)/100;
				UpdateData(TRUE);
				m_fScaleY = iter_find->second->m_tMatInfo.mat[MATID::SCALE].y;
				UpdateData(FALSE);
			}
		}
		if (m_bCircle)
		{
			D3DXVECTOR3 vDis =  iter_find->second->m_tMatInfo.mat[MATID::TRANS]- vMouse;
			D3DXVec3Normalize(&vDis, &vDis);
		
			float fCos = D3DXVec3Dot(&vDis, &m_vStartPos);
			//���� angle
			float fAngle = D3DXToDegree(acosf(fCos));
			if (iter_find->second->m_tMatInfo.mat[MATID::TRANS].y < vMouse.y)
				 fAngle *= -1.f; 
			iter_find->second->m_tMatInfo.mat[MATID::ROT].z = fAngle;

			UpdateData(TRUE);
			m_fRotZ = iter_find->second->m_tMatInfo.mat[MATID::ROT].z;
			UpdateData(FALSE);
		}
		m_pView->Invalidate(FALSE);
	}
}

void CUiTool::Collision_Down(D3DXVECTOR3 vMouse)
{
	if (m_bMatTrans || m_bMatScale)
	{
		//x�浹
		if (ColArrow(vMouse, RECT{ -5,-5,100,5 }))
		{
			CString wstrResultName;
			int iresultIdx = m_Result_ListBox.GetCurSel();
			m_Result_ListBox.GetText(iresultIdx, wstrResultName);
			auto& iter_find = m_mapPlacementInfo.find(wstrResultName);
			
			m_vStartMouse = vMouse;
			m_vStartPos = iter_find->second->m_tMatInfo.mat[MATID::TRANS];
			m_bArrowX = true;
			m_bMouseDown = true;
		}
		//y�浹
		if (ColArrow(vMouse, RECT{ -5,-100,5,5 }))
		{
			CString wstrResultName;
			int iresultIdx = m_Result_ListBox.GetCurSel();
			m_Result_ListBox.GetText(iresultIdx, wstrResultName);
			auto& iter_find = m_mapPlacementInfo.find(wstrResultName);

			m_vStartMouse = vMouse;
			m_vStartPos = iter_find->second->m_tMatInfo.mat[MATID::TRANS];
			m_bArrowY = true;
			m_bMouseDown = true;
		}
	}
 	if (m_bMatRot)
	{
		if (ColCircle(vMouse))
		{
			CString wstrResultName;
			int iresultIdx = m_Result_ListBox.GetCurSel();
			m_Result_ListBox.GetText(iresultIdx, wstrResultName);

			auto& iter_find = m_mapPlacementInfo.find(wstrResultName);

			m_vStartPos = iter_find->second->m_tMatInfo.mat[MATID::TRANS]- vMouse;
			D3DXVec3Normalize(&m_vStartPos, &m_vStartPos);
			m_bCircle = true; 
			m_bMouseDown = true;
		}
	}
}

void CUiTool::Collision_Up()
{
	if (m_bMouseDown)
	{
		m_bArrowX = false;
		m_bArrowY = false;
		m_bCircle = false;
		m_bMouseDown = false;
	}
}


void CUiTool::PickingPos(D3DXVECTOR3 vMouse)
{
	UpdateData(TRUE);

	if (m_bMatTrans || m_bMatScale || m_bMatScale||m_bPicking)
		return;

	int iIdx = m_Result_ListBox.GetCurSel();
	if (iIdx == LB_ERR)	//���õȰ� ������ 
	{
		ERR_MSG(L"����Ʈ ������ ����");
		return;
	}

	CString wstrName;
	m_Result_ListBox.GetText(iIdx, wstrName);
	auto& iter = m_mapPlacementInfo.find(wstrName);
	if (iter == m_mapPlacementInfo.end())
	{
		ERR_MSG(L"�ʿ� �ش� Ű�� ����");
		return;
	}
	iter->second->m_tMatInfo.mat[MATID::TRANS]= vMouse;
	m_fPosX = vMouse.x;
	m_fPosY = vMouse.y;

	if (!iter->second->m_bRender)
	{
		iter->second->m_tMatInfo.mat[MATID::SCALE] = { 1.f,1.f,0.f };
		iter->second->m_tMatInfo.mat[MATID::ROT] = { 0.f,0.f,0.f };
		iter->second->m_bRender = true;	
	}

	m_bPicking = true;
	m_wstrMatMod = L"Null";
	
	UpdateData(FALSE);
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
		ERR_MSG(L"�ʿ� �ش� Ű�� ����");
		return;
	}
	iter->second->m_tMatInfo.mat[MATID::TRANS] = { m_fPosX ,m_fPosY,0.f };
	iter->second->m_tMatInfo.mat[MATID::ROT] = { 0.f,0.f,m_fRotZ };
	iter->second->m_tMatInfo.mat[MATID::SCALE] = { m_fScaleX,m_fScaleY ,0.f };
	iter->second->m_bRender = true;
	m_pView->Invalidate(FALSE);
	UpdateData(FALSE);
}


void CUiTool::OnBnClickedAdd()
{
	UpdateData(TRUE);
	PLACEMENT* pPlacementInfo = new PLACEMENT{};
	//�޺� �ڽ� ����
 	int iIdx = m_ComboID.GetCurSel();
	CString strID;
	m_ComboID.GetLBText(iIdx, strID);

	if (strID.IsEmpty())
	{
		ERR_MSG(L"���̵� ���þ���");
		return;
	}

	//�̹��� ����Ʈ ����
	int iImageIdx = m_Image_ListBox.GetCurSel();
	if (LB_ERR == iImageIdx)
	{
		ERR_MSG(L"�̹��� ������ ���ߴ�");
		return;
	}
	CString Image_Key;
	m_Image_ListBox.GetText(iImageIdx, Image_Key);

	//������Ʈ �̸� ��������
	CString ObjKey = m_wstrObjID.GetString();
	if (ObjKey.IsEmpty())
	{
		ERR_MSG(L"�̸��� ��������");
		return;
	}
	//������ ���̵� �ٸ��� �ڿ� ���� �̸� �ε��� �ʱ�ȭ
	if (m_wstrID != ObjKey)
		m_KeyIndex = 0;
	m_wstrID = ObjKey;


	auto& iter_find = m_mapFileInfo.find(Image_Key);

	if (iter_find == m_mapFileInfo.end())
	{
		ERR_MSG(L"���� �ʿ� �̹����� ����");
		return;
	}

	pPlacementInfo->wstrObjectKey = iter_find->first;
	pPlacementInfo->wstrFilePath = iter_find->second;

	if (strID == L"BACKGROUND")
		pPlacementInfo->eRenderID = RENDERID::BACKGROUND;
	else if(strID == L"UI")
		pPlacementInfo->eRenderID = RENDERID::UI;
	else if (strID == L"OBJECT")
	{
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map<CString, OBJECTINFO*>& map = pForm->m_tObjectTool.m_mapObject;

		pPlacementInfo->eRenderID = RENDERID::OBJECT;

		//������ ����Ʈ�ڽ� ����
		int iPrefabIdx = m_Prefab_ListBox.GetCurSel();
		if (LB_ERR == iPrefabIdx)
		{
			ERR_MSG(L"������ ���� ����");
			return;
		}
		CString PrefabName;
		m_Prefab_ListBox.GetText(iPrefabIdx, PrefabName);
		//�̸� �����ͼ� Ű������ ���� ���� 
		auto& iter = map.find(PrefabName);
		pPlacementInfo->wstrPrefabName = iter->second->cstrName;
	}

	//�̸��ڿ� �ε����� �ٿ���
	//������ �ε����� ã��
	while (true)
	{
		CString index;
		CString ObjName = m_wstrObjID.GetString();
		index.Format(_T("_%d"), m_KeyIndex);
		ObjName.Append(index);
		auto& iter_mapPalce = m_mapPlacementInfo.find(ObjName);
		
		if (iter_mapPalce == m_mapPlacementInfo.end())
		{
			pPlacementInfo->wstrName = ObjName;
			m_mapPlacementInfo.emplace(ObjName, pPlacementInfo);
			m_Result_ListBox.AddString(ObjName);
			m_KeyIndex++;
			break;
		}
		else 
			m_KeyIndex++;
	}
	UpdateData(FALSE);
}


void CUiTool::OnBnClickedClear()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CUiTool::OnBnClickedTranslation()
{
	UpdateData(TRUE);
	if (m_bMatTrans)
	{
		m_bMatTrans = false;
		m_wstrMatMod = L"Null";
		m_pView->Invalidate(FALSE);
		UpdateData(FALSE);
		return;
	}
	if (!m_bMatTrans)
	{
		m_bMatTrans = true;
		m_bMatRot = false;
		m_bMatScale = false;
		m_bPicking = true;
		m_wstrMatMod = L"Trans";
		m_pView->Invalidate(FALSE);
		UpdateData(FALSE);
		return;
	}

}


void CUiTool::OnBnClickedRotation()
{
	UpdateData(TRUE);
	if (m_bMatRot)
	{
		m_bMatRot = false;

		m_wstrMatMod = L"Null";
		m_pView->Invalidate(FALSE);
		UpdateData(FALSE);
		return;
	}
	if (!m_bMatRot)
	{
		m_bMatRot = true;
		m_bMatTrans = false;
		m_bMatScale = false;
		m_bPicking = true;
		m_wstrMatMod = L"Rot";
		m_pView->Invalidate(FALSE);
		UpdateData(FALSE);
		return;
	}


	//������ �߽����� 360�� �� 30�� �� 

	D3DXVECTOR2 vLinePosZ[3] = { { 100.f, 100.f },{ 200.f, 200.f },{ -10.f,0.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosZ, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}


void CUiTool::OnBnClickedScale()
{
	UpdateData(TRUE);
	if (m_bMatScale)
	{
		m_bMatScale = false;
		m_wstrMatMod = L"Null";
		m_pView->Invalidate(FALSE);
		UpdateData(FALSE);
		return;
	}
	if (!m_bMatScale)
	{
		m_bMatScale = true;
		m_bMatRot = false;
		m_bMatTrans = false;
		m_bPicking = true;
		m_wstrMatMod = L"Scale";
		m_pView->Invalidate(FALSE);
		UpdateData(FALSE);
		return;
	}

	CGraphic_Device::Get_Instance()->Get_Sprite()->End();
	D3DXVECTOR2 vLinePosX[3] = { { 0.f, 0.f },{ 200.f, 0.f },{ -10.f,-10.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosX, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DXVECTOR2 vLinePosY[3] = { { 0.f, 0.f },{ 0.f, 200.f },{ -10.f,10.f } };
	CGraphic_Device::Get_Instance()->Get_Line()->Draw(vLinePosY, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphic_Device::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
}



// ������ �ε� 
void CUiTool::OnBnClickedPrefabLoad()
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map<CString, OBJECTINFO*>& map = pForm->m_tObjectTool.m_mapObject;

	for (auto& rPair : map)
	{
		CString cstrTemp = rPair.second->cstrName;
		if (m_Prefab_ListBox.FindStringExact(-1, cstrTemp.GetString()) == -1)
			m_Prefab_ListBox.AddString(cstrTemp);
	}
}

// 
void CUiTool::OnBnClickedResultSave()
{
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

		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;

		for (auto& rPair : m_mapPlacementInfo)
		{
			WriteFile(hFile, &rPair.second->eRenderID, sizeof(int), &dwbyte, nullptr);

			WriteFile(hFile, &rPair.second->m_tMatInfo.mat[MATID::TRANS], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->m_tMatInfo.mat[MATID::ROT], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->m_tMatInfo.mat[MATID::SCALE], sizeof(D3DXVECTOR3), &dwbyte, nullptr);

			WriteFile(hFile, &rPair.second->m_bRender, sizeof(bool), &dwbyte, nullptr);
		
			//if (rPair.second->eRenderID == RENDERID::OBJECT)
			//{
			if (rPair.second->wstrPrefabName.IsEmpty())
			{
				dwStringSize = 0;
				WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
				WriteFile(hFile, rPair.second->wstrPrefabName.GetString(), dwStringSize, &dwbyte, nullptr);
			}
			else
			{
				dwStringSize = (rPair.second->wstrPrefabName.GetLength() + 1) * sizeof(TCHAR);
				WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
				WriteFile(hFile, rPair.second->wstrPrefabName.GetString(), dwStringSize, &dwbyte, nullptr);
			}
			dwStringSize = (rPair.second->wstrName.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrName.GetString(), dwStringSize, &dwbyte, nullptr);

			dwStringSize = (rPair.second->wstrObjectKey.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrObjectKey.GetString(), dwStringSize, &dwbyte, nullptr);

			dwStringSize = (rPair.second->wstrFilePath.GetLength() + 1) * sizeof(TCHAR);
			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrFilePath.GetString(), dwStringSize, &dwbyte, nullptr);

		}
		CloseHandle(hFile);
	}
}


void CUiTool::OnBnClickedResultLoad()
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
		for (auto& rPair : m_mapPlacementInfo)
			Safe_Delete(rPair.second);
		m_mapPlacementInfo.clear();
		m_Result_ListBox.ResetContent();

		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;

		TCHAR* pBuf = nullptr;
		PLACEMENT* pPlacement = nullptr;

		while (true)
		{
			pPlacement = new PLACEMENT;
			ReadFile(hFile, &pPlacement->eRenderID, sizeof(int), &dwbyte, nullptr);
			if (0 == dwbyte)
			{
				Safe_Delete(pPlacement);
				break;
			}
			ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::TRANS], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
			ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::ROT], sizeof(D3DXVECTOR3), &dwbyte, nullptr);
			ReadFile(hFile, &pPlacement->m_tMatInfo.mat[MATID::SCALE], sizeof(D3DXVECTOR3), &dwbyte, nullptr);

			ReadFile(hFile, &pPlacement->m_bRender, sizeof(bool), &dwbyte, nullptr);


			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrPrefabName = pBuf;
			if(pBuf != NULL)
				m_beIDObject = true;
			Safe_Delete(pBuf);

			//	}

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrName = pBuf;
			Safe_Delete(pBuf);

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrObjectKey = pBuf;
			Safe_Delete(pBuf);

			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			pBuf = new TCHAR[dwStringSize];
			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			pPlacement->wstrFilePath = pBuf;
			Safe_Delete(pBuf);

			m_mapPlacementInfo.emplace(pPlacement->wstrName,pPlacement);
			m_mapFileInfo.emplace(pPlacement->wstrObjectKey, pPlacement->wstrFilePath);
			m_Result_ListBox.AddString(pPlacement->wstrName);
			if(LB_ERR == m_Image_ListBox.FindStringExact(-1,pPlacement->wstrObjectKey))
				m_Image_ListBox.AddString(pPlacement->wstrObjectKey);
			//===============�̹��� Insert==============

			if (pPlacement->wstrObjectKey.GetString() != L"")
			{
				if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(CTexture_Manager::SINGLE_TEX, pPlacement->wstrFilePath.GetString(), pPlacement->wstrObjectKey.GetString())))
				{
					ERR_MSG(L"�̱� �ؽ��� ����");
					return;
				}
			}
		}
		if (m_beIDObject)
		{
			CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
			CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
			const map<CString, OBJECTINFO*>& map = pForm->m_tObjectTool.m_mapObject;

			for (auto& rPair : map)
			{
				CString cstrTemp = rPair.second->cstrName;
				if (m_Prefab_ListBox.FindStringExact(-1, cstrTemp.GetString()) == -1)
					m_Prefab_ListBox.AddString(cstrTemp);
			}
		}
		m_pView->Invalidate(FALSE);
		CloseHandle(hFile);
	}
}



void CUiTool::OnLbnSelchangeResultList()
{
	UpdateData(TRUE);


	m_iNowResultIdx = m_Result_ListBox.GetCurSel();
	CString wstrName;
	m_Result_ListBox.GetText(m_iNowResultIdx, wstrName);
	auto& iter = m_mapPlacementInfo.find(wstrName);
	
	m_wstrObjID = iter->second->wstrName.GetString();
	int idx = 0;
	switch (iter->second->eRenderID)
	{
	case RENDERID::BACKGROUND:
		idx = m_ComboID.FindString(-1,L"BACKGROUND");
		m_ComboID.SetCurSel(idx);
		break;
	case RENDERID::OBJECT:
		idx = m_ComboID.FindString(-1, L"OBJECT");
		m_ComboID.SetCurSel(idx);
		break;
	case RENDERID::UI:
		idx = m_ComboID.FindString(-1, L"UI");
		m_ComboID.SetCurSel(idx);
		break;
	}
	//�׷����������� ��Ʈ������ ����
	if (iter->second->m_bRender)
	{
		m_fPosX = iter->second->m_tMatInfo.mat[MATID::TRANS].x;
		m_fPosY = iter->second->m_tMatInfo.mat[MATID::TRANS].y;
		m_fRotZ = iter->second->m_tMatInfo.mat[MATID::ROT].z;
		m_fScaleX = iter->second->m_tMatInfo.mat[MATID::SCALE].x;
		m_fScaleY = iter->second->m_tMatInfo.mat[MATID::SCALE].y;
	}
	else
	{
		m_fPosX = 0.f;
		m_fPosY = 0.f;
		m_fRotZ = 0.f;
		m_fScaleX = 1.f;
		m_fScaleY = 1.f;
	}
	int iImageIdx = 0; 
	//�̹��� ����Ʈ�ڽ����� Ű���� ���� ��Ʈ���� ã�� Ŀ���� ����Ŵ 
	if ((iImageIdx = m_Image_ListBox.FindStringExact(-1, iter->second->wstrObjectKey)) != LB_ERR)
	{
		m_Image_ListBox.SetCurSel(iImageIdx);
	}
	//�̹����� ���
	CString wstrFileName;
	m_Image_ListBox.GetText(iImageIdx, wstrFileName);
	SetImageView(wstrFileName.GetString(), m_Picture);

	if (iter->second->eRenderID == RENDERID::OBJECT)
	{
		int iImageIdx = 0;
		//������ ����Ʈ�ڽ����� Ű���� ���� ��Ʈ���� ã�� Ŀ���� ����Ŵ 
		if ((iImageIdx = m_Prefab_ListBox.FindStringExact(-1, iter->second->wstrPrefabName)) == LB_ERR)
		{
			ERR_MSG(L"������ �̹����� ���� �� ����? ���� �̻��ѵ�?");
			return;
		}

			m_Prefab_ListBox.SetCurSel(iImageIdx);
		CString wstrFileName;
		m_Prefab_ListBox.GetText(iImageIdx, wstrFileName);

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
		const map<CString, OBJECTINFO*>& map = pForm->m_tObjectTool.m_mapObject;

		CString wstrPrefabKey;
		auto& iter_find = map.find(wstrFileName);
		wstrPrefabKey = iter_find->second->cstrObjectImage_ObjectKey;
		SetImageView(wstrPrefabKey.GetString(), m_Picture_Prefab);
	}




	m_bMatTrans = false;
	m_bMatScale = false;
	m_bMatRot = false;
	m_bPicking = false;
	m_wstrMatMod = L"Mouse";
	m_pView->Invalidate(FALSE);
	UpdateData(FALSE);
}


void CUiTool::OnBnClickedMouseTrans()
{
	UpdateData(TRUE);
	if (m_bPicking) 
	{
		m_bPicking = false;
		m_bMatTrans = false;
		m_bMatScale = false;
		m_bMatRot = false;
		m_wstrMatMod = L"Mouse";
		UpdateData(FALSE);
		return;
	}
	else
	{
		m_bPicking = true;
		m_wstrMatMod = L"Null";
		UpdateData(FALSE);
		return;
	}

}


void CUiTool::OnLbnSelchangePrefabList()
{
	int iListCursor = m_Prefab_ListBox.GetCurSel();
	CString wstrFileName;
	m_Prefab_ListBox.GetText(iListCursor, wstrFileName);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CForm*	pForm = dynamic_cast<CForm*>(pMain->m_tSecondSplitter.GetPane(1, 0));
	const map<CString, OBJECTINFO*>& map = pForm->m_tObjectTool.m_mapObject;

	CString wstrPrefabKey;
	auto& iter_find = map.find(wstrFileName);
	wstrPrefabKey = iter_find->second->cstrObjectImage_ObjectKey;
	SetImageView(wstrPrefabKey.GetString(), m_Picture_Prefab);
}
