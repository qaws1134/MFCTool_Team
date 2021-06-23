#pragma once
#include "afxwin.h"


// CUiTool ��ȭ �����Դϴ�.
class CMFCToolView;
class CUiTool : public CDialog
{
	DECLARE_DYNAMIC(CUiTool)

public:
	CUiTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUiTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UITOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	void SetImageView(CString Objectkey, const CStatic& PictureBox);
	void PickingPos(D3DXVECTOR3 vMouse);
	void Render_UI();
	void SetView(CMFCToolView* pView) { m_pView = pView; }
public :
	afx_msg void OnBnClickedTranslation();
	afx_msg void OnBnClickedRotation();
	afx_msg void OnBnClickedScale();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedResultSave();
	afx_msg void OnBnClickedResultLoad();
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedPrefabLoad();

	afx_msg void OnLbnSelchangeImageListBox();

	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeResultList();

	CString m_wstrObjID;
	CString m_strID;
	CStatic m_Picture;
	CComboBox m_ComboID;
	CListBox m_Image_ListBox;
	CListBox m_Prefab_ListBox;
	CListBox m_Result_ListBox;

	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;
	float m_fScaleX;
	float m_fScaleY;
	OBJECTINFO m_tPrefabInfo;

private :
	map<CString, PLACEMENT*> m_mapPlacementInfo;
	map<CString, CString> m_mapFileInfo;

	list<int>m_listResult;


	MATRIXINFO m_tMatInfo;
	CMFCToolView* m_pView;

	bool m_bMatLine;


};
