// MapTool.cpp: 구현 파일
//

#include "pch.h"
#include "MFCTool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "MFCToolView.h"
#include "Terrain.h"
#include "FileInfo.h"



// CMapTool 대화 상자

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MAPTOOL, pParent)
	, m_drawID(0)
	, m_objectKey(L"Terrain")
	, m_stateKey(L"Tile")
	, m_size(1.5f,1.5f,1.f)
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
	DDX_Control(pDX, IDC_LIST2, m_listBox2);
	DDX_Control(pDX, IDC_LIST3, m_listBox3);
}


BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedSave)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnLbnSelchangeTile)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedLoad)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST2, &CMapTool::OnLbnSelchangeBox)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedButtonTile)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedButtonBox)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapTool::OnBnClickedButtonWall)
	ON_LBN_SELCHANGE(IDC_LIST3, &CMapTool::OnLbnSelchangeWall)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기


void CMapTool::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dig(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	//D:\박병건\118C\D2D\Framework_v1
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");
	dig.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == dig.DoModal())
	{
		CString filePath = dig.GetPathName();
		HANDLE file = CreateFile(filePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == file)
			return;
		CMainFrame* main = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMFCToolView* view = dynamic_cast<CMFCToolView*>(main->m_mainSplitter.GetPane(0, 1));
		const vector<Tile_Info*>& vecTile = view->m_terrain->Get_VecTile();
		const vector<Tile_Info*>& vecObj = view->m_terrain->Get_VecObj();

		DWORD byte = 0;
		for (auto& pTile : vecTile)
			WriteFile(file, pTile, sizeof(Tile_Info), &byte, nullptr);

		byte = 0;
		for (auto& pObj : vecObj)
			WriteFile(file, pObj, sizeof(Tile_Info), &byte, nullptr);

		CloseHandle(file);
	}
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
	m_option = 0;
	//if (m_drawID >= 23)
	//{
	//	m_objectKey = L"Wall";
	//	m_stateKey = L"WallObj";
	//}

	CGraphic_Device::Get_Instance()->Render_Begin();
	const Texture_Info * textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Terrain", L"Tile", m_drawID);
	if (nullptr == textureInfo)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;
	float ratioX = float(WINCX) / textureInfo->imageInfo.Width;
	float ratioY = float(WINCY) / textureInfo->imageInfo.Height;
	D3DXMatrixScaling(&matScale, ratioX, ratioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 500.f, 400.f, 0.f);
	matWorld = matScale * matTrans;

	//m_size.x = 1.f;//1.5f;
	//m_size.y = 1.f;//1.5f;

	float centerX = textureInfo->imageInfo.Width >> 1;
	float centerY = textureInfo->imageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(centerX, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_picture.m_hWnd);
	UpdateData(FALSE);
}



void CMapTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	//D:\박병건\118C\D2D\Framework_v1
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");
	dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == dlg.DoModal())
	{
		CString filePath = dlg.GetPathName();
		HANDLE hFile = CreateFile(filePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		CMainFrame* main = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMFCToolView* view = dynamic_cast<CMFCToolView*>(main->m_mainSplitter.GetPane(0, 1));
		view->m_terrain->Release_Terrain();
		DWORD byte = 0;
		Tile_Info* tile = nullptr;
		while (true)
		{
			tile = new Tile_Info;
			ReadFile(hFile, tile, sizeof(Tile_Info), &byte, nullptr);

			if (0 == byte)
			{
				Safe_Delete(tile);
				break;
			}
			if (tile->objectKey == 1)
				view->m_terrain->Set_Tile(tile);
			else if (tile->objectKey == 2 || tile->objectKey == 3)
				view->m_terrain->Set_Obj(tile);
		}
		view->Invalidate(FALSE);
		CloseHandle(hFile);
	}
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
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

		if (m_listBox.IsWindowVisible())
			m_listBox.AddString(szFileName);
		if (m_listBox2.IsWindowVisible())
			m_listBox2.AddString(szFileName);
		if (m_listBox3.IsWindowVisible())
			m_listBox3.AddString(szFileName);
	}
	m_listBox.SetHorizontalExtent(500);
	m_listBox2.SetHorizontalExtent(500);
	m_listBox3.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CMapTool::OnLbnSelchangeBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int index = m_listBox2.GetCurSel();
	CString fileName;
	m_listBox2.GetText(index, fileName);
	int i = 0;
	for (; i < fileName.GetLength(); ++i)
	{
		if (isdigit(fileName[i]))
			break;
	}
	fileName.Delete(0, i);
	m_drawID = _ttoi(fileName.GetString());
	m_option = 2;

	CGraphic_Device::Get_Instance()->Render_Begin();

	const Texture_Info * textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Box", L"BoxObj", m_drawID);
	if (nullptr == textureInfo)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;
	float ratioX = float(WINCX) / textureInfo->imageInfo.Width;
	float ratioY = float(WINCY) / textureInfo->imageInfo.Height;

	D3DXMatrixScaling(&matScale, ratioX, ratioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 500.f, 400.f, 0.f);
	matWorld = matScale * matTrans;
	//m_size.x = 1.f;//1.5f;
	//m_size.y = 1.f;//1.5f;
	float centerX = textureInfo->imageInfo.Width >> 1;
	float centerY = textureInfo->imageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(centerX, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_picture.m_hWnd);
	UpdateData(FALSE);
}


int CMapTool::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	////int size = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	//TCHAR szBuf[MAX_PATH] = L"";
	//CString relativePath;
	//for (int i = 0; i < size; ++i)
	//{
	//	//DragQueryFile(hDropInfo, i, szBuf, MAX_PATH);
	//	relativePath = CFileInfo::ConvertRelativePath(szBuf);
	//	CString fileNameAndExtension = PathFindFileName(szBuf);
	//	TCHAR szFileName[MAX_PATH]{};
	//	lstrcpy(szFileName, fileNameAndExtension.GetString());

	//	PathRemoveExtension(szFileName);

	//	m_listBox.AddString(szFileName);
	//}
	//m_listBox.SetHorizontalExtent(500);
	//UpdateData(FALSE);
	//CDialog::OnDropFiles(hDropInfo);


	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CMapTool::OnBnClickedButtonTile()
{
	m_objectKey = L"Terrain";
	m_stateKey = L"Tile";
	m_listBox.ShowWindow(SW_SHOW);
	m_listBox2.ShowWindow(SW_HIDE);
	m_listBox3.ShowWindow(SW_HIDE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}




void CMapTool::OnBnClickedButtonBox()
{
	m_objectKey = L"Box";
	m_stateKey = L"BoxObj";
	m_listBox.ShowWindow(SW_HIDE);
	m_listBox2.ShowWindow(SW_SHOW);
	m_listBox3.ShowWindow(SW_HIDE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool::OnBnClickedButtonWall()
{
	m_objectKey = L"Wall";
	m_stateKey = L"WallObj";
	m_listBox.ShowWindow(SW_HIDE);
	m_listBox2.ShowWindow(SW_HIDE);
	m_listBox3.ShowWindow(SW_SHOW);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMapTool::OnLbnSelchangeWall()
{
	UpdateData(TRUE);
	int index = m_listBox3.GetCurSel();
	CString fileName;
	m_listBox3.GetText(index, fileName);
	int i = 0;
	for (; i < fileName.GetLength(); ++i)
	{
		if (isdigit(fileName[i]))
			break;
	}
	fileName.Delete(0, i);
	m_drawID = _ttoi(fileName.GetString());
	if (m_drawID==17 || m_drawID == 18 || m_drawID == 25 || m_drawID == 26 || m_drawID == 33 || m_drawID == 45 || m_drawID == 46 || m_drawID == 47 || (m_drawID >=48 && m_drawID%2==0 ))
	{
		m_option = 1;
	}
	else
	{
		m_option = 0;
	}

	CGraphic_Device::Get_Instance()->Render_Begin();

	const Texture_Info * textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Wall", L"WallObj", m_drawID);
	if (nullptr == textureInfo)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;
	float ratioX = float(WINCX) / textureInfo->imageInfo.Width;
	float ratioY = float(WINCY) / textureInfo->imageInfo.Height;
	D3DXMatrixScaling(&matScale, ratioX, ratioY, 0.f);
	D3DXMatrixTranslation(&matTrans, 500.f, 500.f, 0.f);
	matWorld = matScale * matTrans;

	//m_size.x = 1.f;// float(TILECX) / textureInfo->imageInfo.Width * 1.5f;
	//m_size.y = 1.f;// float(TILECY) / textureInfo->imageInfo.Height * 1.5f;

	float centerX = textureInfo->imageInfo.Width >> 1;
	float centerY = textureInfo->imageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(centerX, centerY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_picture.m_hWnd);
	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
