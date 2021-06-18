#pragma once
#include "afxwin.h"


// CUnitTool ��ȭ �����Դϴ�.

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CObjectTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	// TestData
	CString m_wstrSrc;
	CString m_wstrDst;
	afx_msg void OnBnClickedButton1();
	// UnitData
	CString m_strName;
	int m_iAtt;
	int m_iDef;
	afx_msg void OnBnClickedButton2();
	map<CString, UNITINFO*> m_mapUnitInfo; 
	CListBox m_ListBox;
	afx_msg void OnLbnSelRenderUnitData();
	CButton m_Radio[��];
	CButton m_Check[3];
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
