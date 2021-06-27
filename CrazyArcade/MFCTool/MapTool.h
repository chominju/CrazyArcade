#pragma once


// CMapTool 대화 상자

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTool();
	BYTE m_drawID;
	BYTE m_option;
	wstring m_objectKey;
	wstring m_stateKey;
	D3DXVECTOR3 m_size;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listBox;
	CStatic m_picture;
	afx_msg void OnBnClickedSave();
	afx_msg void OnLbnSelchangeTile();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLbnSelchangeBox();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CListBox m_listBox2;
	afx_msg void OnBnClickedButtonTile();
	afx_msg void OnBnClickedButtonBox();
	afx_msg void OnBnClickedButtonWall();
	CListBox m_listBox3;
	afx_msg void OnLbnSelchangeWall();
};
