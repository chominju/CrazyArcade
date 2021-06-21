// MapTool.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTool.h"
#include "MapTool.h"
#include "afxdialogex.h"


// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSave)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeTile)
	ON_STN_CLICKED(IDC_PICTURE, &CMapTool::OnStnClickedPicture)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


void CMapTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool::OnLbnSelchangeTile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int index = m_listBox.GetCurSel();
	CString fileName;
	m_listBox.GetText(index, fileName);
	int i = 0;
	for (; i < fileName.GetLength(); ++i)
	{
		if (isdigit(fileName[i]))
			break;
	}
	fileName.Delete(0, i);
	m_drawID = _ttoi(fileName.GetString());

	CGraphic_Device::Get_Instance()->Render_Begin();
	D3DXMATRIX matScale, matTrans, matWorld;
	float ratioX = float(WINCX) / TILECX;
	float ratioY = float(WINCY) / TILECY;
	D3DXMatrixScaling(&matScale, ratioX, ratioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
	matWorld = matScale * matTrans;

	const Texture_Info * textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Terrain", L"Tile", m_drawID);
	if (nullptr == textureInfo)
		return;
	float centerX = textureInfo->imageInfo.Width >> 1;
	float centerY = textureInfo->imageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->GetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(centerX, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_picture.m_hWnd);
	UpdateData(FALSE);
}


void CMapTool::OnStnClickedPicture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
