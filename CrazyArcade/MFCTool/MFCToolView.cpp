
// MFCToolView.cpp: CMFCToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCTool.h"
#endif

#include "MainFrm.h"
#include "MFCToolDoc.h"
#include "MFCToolView.h"
#include "Terrain.h"
#include "Form.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCToolView
HWND g_hwnd;

IMPLEMENT_DYNCREATE(CMFCToolView, CView)

BEGIN_MESSAGE_MAP(CMFCToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMFCToolView 생성/소멸

CMFCToolView::CMFCToolView()
	:m_graphic_Device(CGraphic_Device::Get_Instance())
	, m_terrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFCToolView::~CMFCToolView()
{
}

BOOL CMFCToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCToolView 그리기

void CMFCToolView::OnDraw(CDC* /*pDC*/)
{
	CMFCToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_graphic_Device->Render_Begin();
	m_terrain->Render_Terrain();
	m_graphic_Device->Render_End();

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CMFCToolView 인쇄

BOOL CMFCToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFCToolView 진단

#ifdef _DEBUG
void CMFCToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCToolDoc* CMFCToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCToolDoc)));
	return (CMFCToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCToolView 메시지 처리기


void CMFCToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	g_hwnd = m_hWnd;

	CMainFrame * main = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	RECT rcMain{};
	main->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView{};
	GetClientRect(&rcView);
	int gapX = rcMain.right - rcView.right; //1440 - 1420 = 20
	int gapY = rcMain.bottom - rcView.bottom;

	main->SetWindowPos(nullptr, 0, 0, WINCX + gapX + 1, WINCY + gapY + 1, SWP_NOMOVE);

	if (FAILED(m_graphic_Device->Ready_Graphic_Device()))
	{
		ERR_MSG(L"Ready Graphic Device");
		return;
	}

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
		L"../Resource/Tile/Tile%d.png",
		L"Terrain", L"Tile", 23)))
		return;

	if (FAILED(CTexture_Manager::Get_Instance()->Insert_Texture_Manager(TEXTURE_ID::TEXTURE_MULTI,
		L"../Resource/Object/Obj%d.png",
		L"Object", L"Object", 22)))
		return;

	m_terrain = new CTerrain;
	if (FAILED(m_terrain->Ready_Terrain()))
		return;
	m_terrain->Set_View(this);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CMFCToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	D3DXVECTOR3 mouse{ float(point.x),float(point.y),0.f };
	CMainFrame* main = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* form = dynamic_cast<CForm*>(main->m_mainSplitter.GetPane(0, 0));
	BYTE drawID = form->m_mapTool.m_drawID;
	m_terrain->Tile_Change_Terrain(mouse, drawID,1);
	Invalidate(FALSE);

	CView::OnLButtonDown(nFlags, point);
}
