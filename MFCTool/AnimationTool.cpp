// AnimationTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "AnimationTool.h"
#include "afxdialogex.h"


// CAnimationTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAnimationTool, CDialog)

CAnimationTool::CAnimationTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ANIMATIONTOOL, pParent)
{

}

CAnimationTool::~CAnimationTool()
{
}

void CAnimationTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnimationTool, CDialog)
END_MESSAGE_MAP()


// CAnimationTool �޽��� ó�����Դϴ�.
