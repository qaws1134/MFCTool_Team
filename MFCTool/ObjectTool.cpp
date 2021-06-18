// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_OBJECTTOOL, pParent)
	, m_wstrSrc(_T(""))
	, m_wstrDst(_T(""))
	, m_strName(_T(""))
	, m_iAtt(0)
	, m_iDef(0)
{

}

CObjectTool::~CObjectTool()
{
	for (auto& rPair : m_mapUnitInfo)
		Safe_Delete(rPair.second); 
	m_mapUnitInfo.clear(); 
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_wstrSrc);
	DDX_Text(pDX, IDC_EDIT2, m_wstrDst);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iAtt);
	DDX_Text(pDX, IDC_EDIT5, m_iDef);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[����]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[����]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[����]);

	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);

}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectTool::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectTool::OnLbnSelRenderUnitData)
	ON_BN_CLICKED(IDC_BUTTON3, &CObjectTool::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CObjectTool::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CObjectTool::OnBnClickedButton5)
END_MESSAGE_MAP()


// CUnitTool �޽��� ó�����Դϴ�.


void CObjectTool::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ������ ¥����... ũŪ... �ų��� ������ !! �� ������ �ٰ��Խ��ϴ�!!! 
	UpdateData(TRUE); // ��Ʈ�ѿ��� �����͸� ���ö� TRUE�� ����ϰ� 
	m_wstrDst = m_wstrSrc; 
	UpdateData(FALSE); // ��Ʈ�ѷ� �����͸� �������� FALSE�� ����Ѵ�. 
}

//Add
void CObjectTool::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE); 
	auto& iter = m_mapUnitInfo.find(m_strName); 
	if (iter != m_mapUnitInfo.end())
		return; 

	UNITINFO* pUnitInfo = new UNITINFO{};
	//ZeroMemory(pUnitInfo, sizeof(UNITINFO)); 
	pUnitInfo->wstrName = m_strName; 
	pUnitInfo->iAtt = m_iAtt; 
	pUnitInfo->iDef = m_iDef; 

	for (int i = 0; i < ��; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnitInfo->byJob = i; 
			break;
		}
	}
	if (m_Check[0].GetCheck())
		pUnitInfo->byItem |= Ȱ; 
	// 0000 0001

	if (m_Check[1].GetCheck())
		pUnitInfo->byItem |= ��;
	// 0000 0001	byItem 
	// 0000 0010	��
	// 0000	0011	byItem 
	if (m_Check[2].GetCheck())
		pUnitInfo->byItem |= ��;


	m_mapUnitInfo.emplace(pUnitInfo->wstrName, pUnitInfo); 
	m_ListBox.AddString(m_strName); 
	UpdateData(FALSE); 
}


void CObjectTool::OnLbnSelRenderUnitData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE); 
	for (int i = 0 ; i < ��; ++i)
	{
		m_Radio[i].SetCheck(FALSE); 
		m_Check[i].SetCheck(FALSE); 
	}
	int iIndex = m_ListBox.GetCurSel(); // ����Ʈ �ڽ��� ���õ� ���ڿ��� ���°�� �ִ��� ��ȯ���ִ� �Լ�. 
	CString wstrFindName; 
	m_ListBox.GetText(iIndex, wstrFindName); // ���ϴ� �ε��� ��°�� ���ڿ��� ������ �Լ�. 
	auto& iter_find = m_mapUnitInfo.find(wstrFindName); 
	if (iter_find == m_mapUnitInfo.end())
		return; 
	m_strName = iter_find->second->wstrName;
	m_iAtt = iter_find->second->iAtt; 
	m_iDef = iter_find->second->iDef; 
	m_Radio[iter_find->second->byJob].SetCheck(TRUE); 
	// 0000 0011 
	// 0000 0001 &
	// 0000 0001
	if (iter_find->second->byItem & Ȱ)
		m_Check[0].SetCheck(TRUE); 
	if (iter_find->second->byItem & ��)
		m_Check[1].SetCheck(TRUE);
	if (iter_find->second->byItem & ��)
		m_Check[2].SetCheck(TRUE);
	UpdateData(FALSE); 
	int i = 0; 
}

// Delete 
void CObjectTool::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// ������� !? ��� �ؾ��� �� !? !? !?!?!!?!?!?!?!?!? 
	UpdateData(TRUE); 
	int iSelect = m_ListBox.GetCurSel(); 
	CString strFindName; 
	m_ListBox.GetText(iSelect, strFindName); 

	auto& iter_find = m_mapUnitInfo.find(strFindName); 
	if (iter_find == m_mapUnitInfo.end())
		return; 

	Safe_Delete(iter_find->second); 
	m_mapUnitInfo.erase(iter_find); 
	m_ListBox.DeleteString(iSelect); 
	UpdateData(FALSE); 
}

// Save
void CObjectTool::OnBnClickedButton4()
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

		//�ϳ��� ���� �ؾ���. ���� ���鼭 . 
		DWORD dwbyte = 0; 
		DWORD dwStringSize = 0; 
		// �� ���� �־�. �ʳ� ������ . 
		for (auto& rPair : m_mapUnitInfo)
		{
			dwStringSize = (rPair.second->wstrName.GetLength() + 1) * sizeof(TCHAR); 

			WriteFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			WriteFile(hFile, rPair.second->wstrName.GetString(), dwStringSize, &dwbyte, nullptr); 
			WriteFile(hFile, &rPair.second->iAtt, sizeof(int), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->iDef, sizeof(int), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->byJob, sizeof(BYTE), &dwbyte, nullptr);
			WriteFile(hFile, &rPair.second->byItem, sizeof(BYTE), &dwbyte, nullptr);
		}
		CloseHandle(hFile); 
	}

}

//Load
void CObjectTool::OnBnClickedButton5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog Dlg(TRUE,// TRUE�� ����. 
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
		for (auto& rPair : m_mapUnitInfo)
			Safe_Delete(rPair.second); 
		m_mapUnitInfo.clear(); 
		m_ListBox.ResetContent(); // ����Ʈ �ڽ��� ��� ���ڿ� ����! 

		//�ϳ��� ���� �ؾ���. ���̷繮 ���鼭 . 
		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;
		UNITINFO* pUnitInfo = 0; 
		TCHAR* pBuf = nullptr; 
		// �� ���� �־�. �ʳ� ������ . 
		while (true)
		{
			ReadFile(hFile, &dwStringSize, sizeof(DWORD), &dwbyte, nullptr);
			
			if (0 == dwbyte)
				break;
			pBuf = new TCHAR[dwStringSize];
			pUnitInfo = new UNITINFO;

			ReadFile(hFile, pBuf, dwStringSize, &dwbyte, nullptr);
			
			pUnitInfo->wstrName = pBuf; 
			Safe_Delete(pBuf); 

			ReadFile(hFile, &pUnitInfo->iAtt, sizeof(int), &dwbyte, nullptr);
			ReadFile(hFile, &pUnitInfo->iDef, sizeof(int), &dwbyte, nullptr);
			ReadFile(hFile, &pUnitInfo->byJob, sizeof(BYTE), &dwbyte, nullptr);
			ReadFile(hFile, &pUnitInfo->byItem, sizeof(BYTE), &dwbyte, nullptr);

			m_mapUnitInfo.emplace(pUnitInfo->wstrName, pUnitInfo); 
			m_ListBox.AddString(pUnitInfo->wstrName); 
		}
		CloseHandle(hFile);
	}
}
