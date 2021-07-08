#pragma once


// CObjectTool 대화 상자

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ObjectTool };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BYTE m_byDrawID;

	afx_msg void OnDropFiles(HDROP hDropInfo);
	CListBox m_imageListBox;
	CListBox m_settingList;
	afx_msg void OnLbnSelchangeItemList();
	afx_msg void OnLbnSelchangeSettingList();
	CButton m_radio_ItemUse[2];
	CButton m_radio_Kick[2];
	CButton m_radio_isWaterBall[2];
	CButton m_radio_isWater[2];
	CButton m_radio_isSpeed[2];
	CButton m_radio_isRide[2];
	CButton m_radio_isShield[2];
	CButton m_radio_isRevival[2];
	afx_msg void OnBnClickedButton_Add();
	afx_msg void OnBnClickedButton_Delete();
	afx_msg void OnBnClickedButton_Save();
	afx_msg void OnBnClickedButton_Load();

	map<BYTE, Item_Info*> m_mapItem;
	CStatic m_picture;
	CString listName;
};
