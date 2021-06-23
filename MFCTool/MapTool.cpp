// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCToolView.h"
#include "Terrain.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_iDrawID(0)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeTileList)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapTool::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	UpdateData(TRUE); 
	int iShotGunCount = DragQueryFile(hDropInfo,0xffffffff, nullptr,0 );
	TCHAR szFileFullPath[MAX_PATH]{}; 

	for (int i = 0; i < iShotGunCount; ++i)
	{
		DragQueryFile(hDropInfo, i, szFileFullPath, MAX_PATH);
		CString wstrRelativePath = CFileInfo::ConvertRelativePath(szFileFullPath);
		CString wstrFileNameAndEx = PathFindFileName(wstrRelativePath);
		TCHAR szFileName[MAX_PATH]{}; 
		lstrcpy(szFileName, wstrFileNameAndEx.GetString()); 
		PathRemoveExtension(szFileName); 
		m_ListBox.AddString(szFileName);
	}
	m_ListBox.SetHorizontalExtent(500); 
	UpdateData(FALSE); 
	CDialog::OnDropFiles(hDropInfo);
}


void CMapTool::OnLbnSelchangeTileList()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iListCursor = m_ListBox.GetCurSel(); 
	CString wstrFileName; 
	m_ListBox.GetText(iListCursor, wstrFileName); 
	
	int i = 0;
	//Tile0
	for (; i < wstrFileName.GetLength(); ++i)
	{
		//0~9������ ���ڸ� �Ǻ����ִ� �Լ�. 
		if (isdigit(wstrFileName[i]))
			break; 
	}
	wstrFileName.Delete(0, i); 
	
	m_iDrawID = _ttoi(wstrFileName.GetString()); 

	CGraphic_Device::Get_Instance()->Render_Begin(); 
	const TEXINFO* pTexInfo = CTexture_Manager::Get_Instance()->Get_TexInfo(L"Terrain", L"Tile", m_iDrawID); 
	if (nullptr == pTexInfo)
		return; 
	D3DXMATRIX matScale, matTrans, matWorld; 
	D3DXMatrixScaling(&matScale, WINCX / TILECX, WINCX / TILECX, 0.f); 
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f); 
	matWorld = matScale * matTrans; 
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1); 
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld); 
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255,255,255,255)); 

	CGraphic_Device::Get_Instance()->Render_End(m_Picture.m_hWnd);

}

// Save
void CMapTool::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(FALSE,// FALSE�� �ٸ��̸����� ����. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	//D:\�ں���\118\118AFramework_2(2)
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
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd()); 
		CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1)); 
		vector<TILE*>& vecTile = pView->m_pTerrain->Get_VecTile();
		//�ϳ��� ���� �ؾ���. ���� ���鼭 . 
		DWORD dwbyte = 0;
		// �� ���� �־�. �ʳ� ������ . 
		for (auto& pTile : vecTile)
			WriteFile(hFile, pTile, sizeof(TILE), &dwbyte, nullptr);

		CloseHandle(hFile);
	}

}

// Load 
void CMapTool::OnBnClickedButton6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE,// FALSE�� �ٸ��̸����� ����. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	//D:\�ں���\118\118AFramework_2(2)
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
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMFCToolView* pView = dynamic_cast<CMFCToolView*>(pMain->m_tMainSplitter.GetPane(0, 1));
		CTerrain* pTerrain = pView->m_pTerrain; 

		vector<TILE*>& vecTile = pTerrain->Get_VecTile();
		for (auto& pTile : vecTile)
			Safe_Delete(pTile); 
		vecTile.clear(); 
		vecTile.shrink_to_fit();

		//�ϳ��� ���� �ؾ���. ���� ���鼭 . 
		DWORD dwbyte = 0;
		// �� ���� �־�. �ʳ� ������ . 
		TILE* pTile = nullptr; 
		while (true)
		{
			pTile = new TILE;
			ReadFile(hFile, pTile, sizeof(TILE), &dwbyte, nullptr); ; 
			if (0 == dwbyte)
			{
				Safe_Delete(pTile); 
				break;
			}
			pTerrain->Set_TileInfo(pTile); 
		}

		CloseHandle(hFile);
	}
}
