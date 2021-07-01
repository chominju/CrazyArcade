// ObjectTool.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTool.h"
#include "ObjectTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCToolView.h"
#include "FileInfo.h"

// CObjectTool 대화 상자

IMPLEMENT_DYNAMIC(CObjectTool, CDialog)

CObjectTool::CObjectTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ObjectTool, pParent)
{

}

CObjectTool::~CObjectTool()
{
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_comboBox);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_WM_DROPFILES()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CObjectTool::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CObjectTool 메시지 처리기



void CObjectTool::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int size = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR szBuf[MAX_PATH] = L"";
	CString relativePath;
	for (int i = 0; i < size; ++i)
	{
		DragQueryFile(hDropInfo, i, szBuf, MAX_PATH);
		relativePath = CFileInfo::ConvertRelativePath(szBuf);
		CString fileNameAndExtension = PathFindFileName(szBuf);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, fileNameAndExtension.GetString());

		PathRemoveExtension(szFileName);

		m_comboBox.AddString(szFileName);

	}

	m_comboBox.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CObjectTool::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
