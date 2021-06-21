// Form.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTool.h"
#include "Form.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CForm::OnBnClickedMapTool)
END_MESSAGE_MAP()


// CForm 진단

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기


void CForm::OnBnClickedMapTool()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_mapTool.ShowWindow(SW_SHOW);
}


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if (nullptr == m_mapTool.GetSafeHwnd())
		m_mapTool.Create(IDD_MAPTOOL);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

