#pragma once


// CMapTool 대화 상자

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMapTool();
	BYTE m_drawID;

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
	afx_msg void OnStnClickedPicture();
	afx_msg void OnBnClickedLoad();
};
