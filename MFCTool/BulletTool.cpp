// BulletTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCTool.h"
#include "BulletTool.h"
#include "afxdialogex.h"


// CBulletTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CBulletTool, CDialog)

CBulletTool::CBulletTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BULLETTOOL, pParent)
{

}

CBulletTool::~CBulletTool()
{
}

void CBulletTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBulletTool, CDialog)
END_MESSAGE_MAP()


// CBulletTool �޽��� ó�����Դϴ�.
