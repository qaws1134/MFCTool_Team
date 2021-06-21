#pragma once
#include "afxwin.h"


// CBulletTool ��ȭ �����Դϴ�.

class CBulletTool : public CDialog
{
	DECLARE_DYNAMIC(CBulletTool)

public:
	CBulletTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBulletTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BULLETTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	map<CString, BULLETDATA*>	m_mapBullet;
public:
	afx_msg void OnCbnSelchangeBulletType();
	CStatic m_GroupBox;
	CComboBox m_BulletTypeSelectControl;
	afx_msg void OnBnClickedAdd();
	CString m_cstrName;
	int m_iAtk;
	float m_fSpeed;
	CButton m_CheckBoxDestructable;
	int m_iShotGunCount;
	float m_fShotGunAngle;
	CListBox m_ListBox_BulletImage;
	CListBox m_ListBox_BulletList;
	CListBox m_ListBox_AnimList;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeBulletImage();
	CStatic m_BulletPicture;
	afx_msg void OnLbnSelchangeBulletList();
	afx_msg void OnBnClickedButtonBulletDelete();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonLoadAnimation();
	afx_msg void OnLbnSelchangeAnimation();
	CStatic m_PictureAnimation;
	afx_msg void OnBnClickedButtonDeleteImage();
};
