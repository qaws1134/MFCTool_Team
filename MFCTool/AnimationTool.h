#pragma once
#include "afxwin.h"


// CAnimationTool ��ȭ �����Դϴ�.

class CAnimationTool : public CDialog
{
	DECLARE_DYNAMIC(CAnimationTool)

public:
	CAnimationTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimationTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATIONTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public :
	void SetImageView(CString Objectkey, int Index, const CStatic& PictureBox, CString StateKey = 0 );  
	void Reset_Info();		//������ 0���� ������
	void Reset_ImageList();

public:
	CString m_wstrObject_Key;
	CString m_wstrState_Key;
	float m_fPlay_Speed;
	int m_iMax_Index;
	int m_iDrawID;
	int m_iInput_Index;
	CString m_wstrFilePath;	//����� ���� ����
	HDROP m_Drop;
	int m_iAnimationCount;
	int m_iTimer;
	int m_iKillTimer;

	afx_msg void OnBnClickedIndex_Apply();
	afx_msg void OnBnClickedAddAnimation();
	afx_msg void OnBnClickedSave();
	afx_msg void OnLbnSelchangeAnimation_List();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedAddInfo();
	afx_msg void OnBnClickedDeleteAll();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnLbnSelchangeImageList();


	CStatic m_Start_Image;
	CStatic m_End_Image;
	CStatic m_Index_To_Selected_Image;
	CStatic m_List_To_Selected_Image;
	CListBox m_Image_ListBox;
	CListBox m_Animation_ListBox;
	CButton m_IsSingle;
	CButton m_Loop;

	map<CString, ANIMATION*> m_mapAnima;

	afx_msg void OnBnClickedDeleteAnimationList();
	afx_msg void OnBnClickedClearBox();
	afx_msg void OnBnClickedClear_Info();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedStop();
	int m_iPlay_Speed;
};
