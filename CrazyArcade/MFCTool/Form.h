#pragma once



// CForm 폼 보기
#include "MapTool.h"
#include "ObjectTool.h"

class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

public:
	CMapTool m_mapTool;
	CObjectTool m_objectTool;
protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMapTool();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedObjectTool();
};


