#pragma once



// CForm �� ���Դϴ�.
#include "MapTool.h"
#include "UiTool.h"
#include "ObjectTool.h"
#include "AnimationTool.h"
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CForm();

public:
	CMapTool m_tMapTool; 
	CAnimationTool m_tAnimationTool;
	CObjectTool	m_tObjectTool;
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
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedMapTool();
	afx_msg void OnBnClickedUiTool();
	afx_msg void OnBnClickedAnimationTool();
	afx_msg void OnBnClickedBulletTool();
};


