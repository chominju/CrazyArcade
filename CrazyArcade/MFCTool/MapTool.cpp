// MapTool.cpp : ���� �����Դϴ�.
//

#include "pch.h"
#include "framework.h"
#include "MFCTool.h"
#include "MapTool.h"
#include "afxdialogex.h"


// CMapTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
END_MESSAGE_MAP()


// CMapTool �޽��� ó�����Դϴ�.
