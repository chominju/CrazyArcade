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
	int iIndex = m_imageListBox.GetCurSel();
	CString wstrFileName;
	m_imageListBox.GetText(iIndex, wstrFileName);
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
	//m_settingList.GetText(iIndex, wstrUnitName);
	//map<wstring, Item_Info*>::iterator iter = m_mapItem.find(wstrUnitName.GetString());

	//if (iter == m_mapItem.end())
	//	return;

	//m_wstrName = iter->second->wstrName;
	//m_iAtt = iter->second->iAtt;
	//m_iDef = iter->second->iDef;
	//m_Radio[iter->second->byJob].SetCheck(TRUE);
	//// 0000 0100
	//// 0000 0001 &
	//// 0000 0000

	//if (iter->second->byItem & 검)
	//	m_CheckBox[0].SetCheck(TRUE);
	//if (iter->second->byItem & 활)
	//	m_CheckBox[1].SetCheck(TRUE);
	//if (iter->second->byItem & 봉)
	//	m_CheckBox[2].SetCheck(TRUE);

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

	

	for (int i = 0; i < 2; i++)
	{
		if (m_radio_ItemUse[i].GetCheck())
			itemInfo->isImmediateUse = true;

		if (m_radio_Kick[i].GetCheck())
			itemInfo->isKick = true;

		if (m_radio_isWaterBall[i].GetCheck())
			itemInfo->isWaterBall=true;

		if (m_radio_isWater[i].GetCheck())
			itemInfo->isWaterLengthUp = true;

		if (m_radio_isSpeed[i].GetCheck())
			itemInfo->speedUp = true;

		if (m_radio_isRide[i].GetCheck())
			itemInfo->isRide = true;

		if (m_radio_isShield[i].GetCheck())
			itemInfo->isShield = true;

		if (m_radio_isRevival[i].GetCheck())
			itemInfo->isRevival = true;

		itemInfo->drawID = m_byDrawID;
		itemInfo->isCollision = true;
	}

	CString temp = listName + 

	m_mapItem.emplace(m_byDrawID, itemInfo);
	m_settingList.AddString(listName + m_byDrawID)


}


void CObjectTool::OnBnClickedButton_Delete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjectTool::OnBnClickedButton_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CObjectTool::OnBnClickedButton_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


