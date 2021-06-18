#pragma once



// CForm �� ���Դϴ�.
#include "ObjectTool.h"
#include "MapTool.h"
#include "UiTool.h"
#include "BulletTool.h"
#include "AnimationTool.h"
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CForm();

public:
	CObjectTool m_tObjectTool; 
	CMapTool m_tMapTool; 
	CAnimationTool m_tAnimationTool;
	CBulletTool	m_tBulletTool;
	CUiTool	m_tUiTool;
	
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUnitTool();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedMapTool();
	afx_msg void OnBnClickedUiTool();
	afx_msg void OnBnClickedAnimationTool();
	afx_msg void OnBnClickedBulletTool();
};


