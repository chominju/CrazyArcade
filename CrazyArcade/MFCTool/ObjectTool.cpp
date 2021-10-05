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
	, listName("Item")
{

}

CObjectTool::~CObjectTool()
{
}

void CObjectTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_imageListBox);
	DDX_Control(pDX, IDC_LIST3, m_settingList);
	DDX_Control(pDX, IDC_RADIO2, m_radio_ItemUse[0]);
	DDX_Control(pDX, IDC_RADIO1, m_radio_ItemUse[1]);

	DDX_Control(pDX, IDC_RADIO3, m_radio_Kick[0]);
	DDX_Control(pDX, IDC_RADIO4, m_radio_Kick[1]);

	DDX_Control(pDX, IDC_RADIO5, m_radio_isWaterBall[0]);
	DDX_Control(pDX, IDC_RADIO6, m_radio_isWaterBall[1]);

	DDX_Control(pDX, IDC_RADIO7, m_radio_isWater[0]);
	DDX_Control(pDX, IDC_RADIO8, m_radio_isWater[1]);

	DDX_Control(pDX, IDC_RADIO9, m_radio_isSpeed[0]);
	DDX_Control(pDX, IDC_RADIO10, m_radio_isSpeed[1]);

	DDX_Control(pDX, IDC_RADIO11, m_radio_isRide[0]);
	DDX_Control(pDX, IDC_RADIO12, m_radio_isRide[1]);

	DDX_Control(pDX, IDC_RADIO13, m_radio_isShield[0]);
	DDX_Control(pDX, IDC_RADIO14, m_radio_isShield[1]);

	DDX_Control(pDX, IDC_RADIO15, m_radio_isRevival[0]);
	DDX_Control(pDX, IDC_RADIO16, m_radio_isRevival[1]);


	DDX_Control(pDX, IDC_PICTURE2, m_picture);
}


BEGIN_MESSAGE_MAP(CObjectTool, CDialog)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectTool::OnLbnSelchangeItemList)
	ON_LBN_SELCHANGE(IDC_LIST3, &CObjectTool::OnLbnSelchangeSettingList)
	ON_BN_CLICKED(IDC_BUTTON7, &CObjectTool::OnBnClickedButton_Add)
	ON_BN_CLICKED(IDC_BUTTON6, &CObjectTool::OnBnClickedButton_Delete)
	ON_BN_CLICKED(IDC_BUTTON2, &CObjectTool::OnBnClickedButton_Save)
	ON_BN_CLICKED(IDC_BUTTON1, &CObjectTool::OnBnClickedButton_Load)
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

		m_imageListBox.AddString(szFileName);
	}
	m_imageListBox.SetHorizontalExtent(500);
	UpdateData(FALSE);
	CDialog::OnDropFiles(hDropInfo);
}


void CObjectTool::OnLbnSelchangeItemList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	for (int i = 0; i < 2; ++i)
	{
		m_radio_ItemUse[i].SetCheck(FALSE);
		m_radio_Kick[i].SetCheck(FALSE);
		m_radio_isWaterBall[i].SetCheck(FALSE);
		m_radio_isWater[i].SetCheck(FALSE);
		m_radio_isSpeed[i].SetCheck(FALSE);
		m_radio_isRide[i].SetCheck(FALSE);
		m_radio_isShield[i].SetCheck(FALSE);
		m_radio_isRevival[i].SetCheck(FALSE);

	}


	int iIndex = m_imageListBox.GetCurSel();
	CString wstrFileName;
	m_imageListBox.GetText(iIndex, wstrFileName);
	m_selectItem = wstrFileName;
	int i = 0;
	for (; i < wstrFileName.GetLength(); ++i)
	{
		// isdigit 0~ 9까지의 숫자인지 아닌지를 판별해주는 함수. 
		if (isdigit(wstrFileName[i]))
			break;
	}
	wstrFileName.Delete(0, i);
	m_byDrawID = _ttoi(wstrFileName.GetString());

	CGraphic_Device::Get_Instance()->Render_Begin();
	const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Item", L"ItemObj", m_byDrawID);
	if (nullptr == textureInfo)
		return;

	D3DXMATRIX matScale, matTrans, matWorld;
	float ratioX = float(WINCX) / textureInfo->imageInfo.Width;
	float ratioY = float(WINCY) / textureInfo->imageInfo.Height;

	D3DXMatrixScaling(&matScale, ratioX, ratioY, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX/2, WINCY/2, 0.f);
	matWorld = matScale * matTrans;

	float fCenterX = textureInfo->imageInfo.Width >> 1;
	float fCenterY = textureInfo->imageInfo.Height >> 1;

	CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CGraphic_Device::Get_Instance()->Render_End(m_picture.m_hWnd);
	UpdateData(FALSE);
}


void CObjectTool::OnLbnSelchangeSettingList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	for (int i = 0; i < 2; ++i)
	{
		m_radio_ItemUse[i].SetCheck(FALSE);
		m_radio_Kick[i].SetCheck(FALSE);
		m_radio_isWaterBall[i].SetCheck(FALSE);
		m_radio_isWater[i].SetCheck(FALSE);
		m_radio_isSpeed[i].SetCheck(FALSE);
		m_radio_isRide[i].SetCheck(FALSE);
		m_radio_isShield[i].SetCheck(FALSE);
		m_radio_isRevival[i].SetCheck(FALSE);
		
	}
	int iIndex = m_settingList.GetCurSel();
	if (LB_ERR == iIndex)
		return;


	CString wstrUnitName;
	m_settingList.GetText(iIndex, wstrUnitName);

	int i = 0;
	for (; i < wstrUnitName.GetLength(); ++i)
	{
		// isdigit 0~ 9까지의 숫자인지 아닌지를 판별해주는 함수. 
		if (isdigit(wstrUnitName[i]))
			break;
	}
	wstrUnitName.Delete(0, i);
	m_byDrawID = _ttoi(wstrUnitName.GetString());


	map<BYTE, Item_Info*>::iterator iter = m_mapItem.find(m_byDrawID);

	if (iter == m_mapItem.end())
		return;

	//m_byDrawID = iter->second->drawID;
	m_tempItemInfo.isCollision = iter->second->isCollision;
	m_tempItemInfo.isUserUse = iter->second->isUserUse;
	m_tempItemInfo.isKick = iter->second->isKick;
	m_tempItemInfo.isRevival =iter->second->isRevival;
	m_tempItemInfo.isRide =iter->second->isRide;
	m_tempItemInfo.isShield =iter->second->isShield;
	m_tempItemInfo.isWaterBall =iter->second->isWaterBall;
	m_tempItemInfo.isWaterLengthUp =iter->second->isWaterLengthUp;
	m_tempItemInfo.isSpeedUp = iter->second->isSpeedUp;



		if (iter->second->isUserUse)
			m_radio_ItemUse[0].SetCheck(true);
		else
			m_radio_ItemUse[1].SetCheck(true);

		if (iter->second->isKick)
			m_radio_Kick[0].SetCheck(true);
		else
			m_radio_Kick[1].SetCheck(true);

		if (iter->second->isWaterBall)
			m_radio_isWaterBall[0].SetCheck(true);
		else
			m_radio_isWaterBall[1].SetCheck(true);

		if (iter->second->isWaterLengthUp)
			m_radio_isWater[0].SetCheck(true);
		else
			m_radio_isWater[1].SetCheck(true);

		if (iter->second->isSpeedUp)
			m_radio_isSpeed[0].SetCheck(true);
		else
			m_radio_isSpeed[1].SetCheck(true);

		if (iter->second->isRide)
			m_radio_isRide[0].SetCheck(true);
		else
			m_radio_isRide[1].SetCheck(true);

		if (iter->second->isShield)
			m_radio_isShield[0].SetCheck(true);
		else
			m_radio_isShield[1].SetCheck(true);

		if (iter->second->isRevival)
			m_radio_isRevival[0].SetCheck(true);
		else
			m_radio_isRevival[1].SetCheck(true);


		iter->second->drawID = m_byDrawID;


		CGraphic_Device::Get_Instance()->Render_Begin();
		const Texture_Info* textureInfo = CTexture_Manager::Get_Instance()->Get_TextureInfo_Manager(L"Item", L"ItemObj", m_byDrawID);
		if (nullptr == textureInfo)
			return;

		D3DXMATRIX matScale, matTrans, matWorld;
		float ratioX = float(WINCX) / textureInfo->imageInfo.Width;
		float ratioY = float(WINCY) / textureInfo->imageInfo.Height;

		D3DXMatrixScaling(&matScale, ratioX, ratioY, 0.f);
		D3DXMatrixTranslation(&matTrans, WINCX / 2, WINCY / 2, 0.f);
		matWorld = matScale * matTrans;

		float fCenterX = textureInfo->imageInfo.Width >> 1;
		float fCenterY = textureInfo->imageInfo.Height >> 1;

		CGraphic_Device::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		CGraphic_Device::Get_Instance()->Get_Sprite()->Draw(textureInfo->texture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		CGraphic_Device::Get_Instance()->Render_End(m_picture.m_hWnd);

	UpdateData(FALSE);

}


void CObjectTool::OnBnClickedButton_Add()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	Item_Info * itemInfo = new Item_Info;

	map< BYTE, Item_Info*>::iterator iter = m_mapItem.find(m_byDrawID);
	if (m_mapItem.end() != iter)
	{
		Safe_Delete(itemInfo);
		return;
	}

	
		if (m_radio_ItemUse[0].GetCheck())
			itemInfo->isUserUse = true;
		else
			itemInfo->isUserUse = false;

		if (m_radio_Kick[0].GetCheck())
			itemInfo->isKick = true;
		else
			itemInfo->isKick = false;

		if (m_radio_isWaterBall[0].GetCheck())
			itemInfo->isWaterBall=true;
		else
			itemInfo->isWaterBall = false;

		if (m_radio_isWater[0].GetCheck())
			itemInfo->isWaterLengthUp = true;
		else
			itemInfo->isWaterLengthUp = false;

		if (m_radio_isSpeed[0].GetCheck())
			itemInfo->isSpeedUp = true;
		else
			itemInfo->isSpeedUp = false;

		if (m_radio_isRide[0].GetCheck())
			itemInfo->isRide = true;
		else
			itemInfo->isRide = false;

		if (m_radio_isShield[0].GetCheck())
			itemInfo->isShield = true;
		else
			itemInfo->isShield = false;

		if (m_radio_isRevival[0].GetCheck())
			itemInfo->isRevival = true;
		else
			itemInfo->isRevival = false;

		itemInfo->drawID = m_byDrawID;
		itemInfo->isCollision = true;


	//char chTemp = (char)m_byDrawID;
	//CString convert = listName;

	//m_imageListBox.

	///*CString stringTemp = */convert.AppendChar(chTemp); //+chTemp;

	//int iIndex = m_imageListBox.GetCurSel();
	//CString wstrFileName;
	//m_imageListBox.GetText(iIndex, wstrFileName);
	
 	m_mapItem.emplace(itemInfo->drawID, itemInfo);
	m_settingList.AddString(m_selectItem);


}


void CObjectTool::OnBnClickedButton_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjectTool::OnBnClickedButton_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	//D:\박병건\118C\D2D\Framework_v1
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		DWORD byte = 0;
		for (auto& rPair : m_mapItem)
		{
			WriteFile(hFile, rPair.second, sizeof(Item_Info), &byte, nullptr);
		/*	dwStringCount = (rPair.second->wstrName.GetLength() + 1) * sizeof(wchar_t);
			WriteFile(hFile, &dwStringCount, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, rPair.second->wstrName.GetString(), dwStringCount, &dwByte, nullptr);*/

		/*	WriteFile(hFile, &rPair.second->drawID, sizeof(BYTE), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isCollision, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isUserUse, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isKick, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isWaterBall, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isWaterLengthUp, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isSpeedUp, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isRide, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isShield, sizeof(bool), &byte, nullptr);
			WriteFile(hFile, &rPair.second->isRevival, sizeof(bool), &byte, nullptr);*/

		}
		CloseHandle(hFile);

		vector<int> temp;
		
	}
}


void CObjectTool::OnBnClickedButton_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	//D:\박병건\118C\D2D\Framework_v1
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		m_settingList.ResetContent();
		for (auto& rPair : m_mapItem)
			Safe_Delete(rPair.second);
		m_mapItem.clear();

		DWORD dwByte = 0;
		Item_Info* itemInfo = nullptr;
		while (true)
		{
			//if (0 == dwByte)
			//	break;
			itemInfo = new Item_Info;
			ReadFile(hFile, itemInfo, sizeof(Item_Info), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(itemInfo);
				break;
			}

		/*	ReadFile(hFile, &itemInfo->drawID, sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isCollision, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isUserUse, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isKick, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isWaterBall, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isWaterLengthUp, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isSpeedUp, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isRide, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isShield, sizeof(bool), &dwByte, nullptr);
			ReadFile(hFile, &itemInfo->isRevival, sizeof(bool), &dwByte, nullptr);*/


			m_mapItem.emplace(itemInfo->drawID, itemInfo);
			//char ch[1] = { itemInfo->drawID };
			//CString temp(ch);
			//temp.GetString();
			//CString temp2 = listName;
			//temp2.Append(temp.GetString());
			m_settingList.AddString(listName);
		}
		CloseHandle(hFile);
	}
}


