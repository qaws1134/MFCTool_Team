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
	bool ColArrow(D3DXVECTOR3 vMouse,RECT rc);

	bool ColCircle(D3DXVECTOR3 vMouse);

	void Collision_Move(D3DXVECTOR3 vMouse);
	void Collision_Down(D3DXVECTOR3 vMouse);
	void Collision_Up();

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

	CStatic m_Picture;
	CStatic m_Picture_Prefab;

	CComboBox m_ComboID;
	CListBox m_Image_ListBox;
	CListBox m_Prefab_ListBox;
	CListBox m_Result_ListBox;
	afx_msg void OnBnClickedMouseTrans();
	CString m_wstrMatMod;

	float m_fPosX;
	float m_fPosY;
	float m_fRotZ;
	float m_fScaleX;
	float m_fScaleY;
	OBJECTINFO m_tPrefabInfo;

private :
	map<CString, PLACEMENT*> m_mapPlacementInfo;
	map<CString, CString> m_mapFileInfo;

	list<int>m_listResult;

	CString m_wstrID;		//�ؽ�Ʈ�ڽ� ���̵� ������ ����

	CMFCToolView* m_pView;

	bool m_bMatTrans;
	bool m_bMatRot;
	bool m_bMatScale;
	bool m_bPicking;
	bool m_bPicKingStart;

	bool m_bArrowX;
	bool m_bArrowY;
	bool m_bCircle;
	bool m_bMouseDown;
	D3DXVECTOR3 m_vStartMouse;
	D3DXVECTOR3 m_vStartPos;

	int m_iNowResultIdx;

	int m_KeyIndex;


	bool m_beIDObject;
public:
	afx_msg void OnLbnSelchangePrefabList();
};
