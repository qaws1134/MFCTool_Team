// UnitTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"


// CUnitTool 대화 상자입니다.

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
	DDX_Control(pDX, IDC_RADIO1, m_Radio[전사]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[법사]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[도적]);

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


// CUnitTool 메시지 처리기입니다.


void CObjectTool::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 팀과제 짜야지... 크큭... 신나는 팀과제 !! 팀 선정이 다가왔습니다!!! 
	UpdateData(TRUE); // 컨트롤에서 데이터를 얻어올때 TRUE를 사용하고 
	m_wstrDst = m_wstrSrc; 
	UpdateData(FALSE); // 컨트롤로 데이터를 내보낼때 FALSE를 사용한다. 
}

//Add
void CObjectTool::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE); 
	auto& iter = m_mapUnitInfo.find(m_strName); 
	if (iter != m_mapUnitInfo.end())
		return; 

	UNITINFO* pUnitInfo = new UNITINFO{};
	//ZeroMemory(pUnitInfo, sizeof(UNITINFO)); 
	pUnitInfo->wstrName = m_strName; 
	pUnitInfo->iAtt = m_iAtt; 
	pUnitInfo->iDef = m_iDef; 

	for (int i = 0; i < 끝; ++i)
	{
		if (m_Radio[i].GetCheck())
		{
			pUnitInfo->byJob = i; 
			break;
		}
	}
	if (m_Check[0].GetCheck())
		pUnitInfo->byItem |= 활; 
	// 0000 0001

	if (m_Check[1].GetCheck())
		pUnitInfo->byItem |= 봉;
	// 0000 0001	byItem 
	// 0000 0010	봉
	// 0000	0011	byItem 
	if (m_Check[2].GetCheck())
		pUnitInfo->byItem |= 검;


	m_mapUnitInfo.emplace(pUnitInfo->wstrName, pUnitInfo); 
	m_ListBox.AddString(m_strName); 
	UpdateData(FALSE); 
}


void CObjectTool::OnLbnSelRenderUnitData()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE); 
	for (int i = 0 ; i < 끝; ++i)
	{
		m_Radio[i].SetCheck(FALSE); 
		m_Check[i].SetCheck(FALSE); 
	}
	int iIndex = m_ListBox.GetCurSel(); // 리스트 박스에 선택된 문자열이 몇번째에 있는지 반환해주는 함수. 
	CString wstrFindName; 
	m_ListBox.GetText(iIndex, wstrFindName); // 원하는 인덱스 번째의 문자열을 얻어오는 함수. 
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
	if (iter_find->second->byItem & 활)
		m_Check[0].SetCheck(TRUE); 
	if (iter_find->second->byItem & 봉)
		m_Check[1].SetCheck(TRUE);
	if (iter_find->second->byItem & 검)
		m_Check[2].SetCheck(TRUE);
	UpdateData(FALSE); 
	int i = 0; 
}

// Delete 
void CObjectTool::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 지우려면 !? 어떻게 해야할 가 !? !? !?!?!!?!?!?!?!?!? 
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE,// FALSE가 다른이름으로 저장. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{}; 

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath); 
	
	//D:\박병건\118\118AFramework_2(2)
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

		//하나씩 저장 해야지. 폴문 돌면서 . 
		DWORD dwbyte = 0; 
		DWORD dwStringSize = 0; 
		// 난 쓸수 있어. 너넨 쓰지마 . 
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE,// TRUE면 열기. 
		L"dat",
		L"*.dat",
		OFN_OVERWRITEPROMPT);
	TCHAR szFilePath[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szFilePath);
	PathRemoveFileSpec(szFilePath);

	//D:\박병건\118\118AFramework_2(2)
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
		for (auto& rPair : m_mapUnitInfo)
			Safe_Delete(rPair.second); 
		m_mapUnitInfo.clear(); 
		m_ListBox.ResetContent(); // 리스트 박스의 모든 문자열 삭제! 

		//하나씩 저장 해야지. 와이루문 돌면서 . 
		DWORD dwbyte = 0;
		DWORD dwStringSize = 0;
		UNITINFO* pUnitInfo = 0; 
		TCHAR* pBuf = nullptr; 
		// 난 쓸수 있어. 너넨 쓰지마 . 
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
