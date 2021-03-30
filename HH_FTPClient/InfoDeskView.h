#pragma once

// CInfoDeskView 视图

class CInfoDeskView : public CEditView
{
	DECLARE_DYNCREATE(CInfoDeskView)

protected:
	CInfoDeskView();           // 动态创建所使用的受保护的构造函数
	virtual ~CInfoDeskView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	CFont  m_NewFont;
	CEdit* m_pEdit;
protected:
	//{{AFX_MSG(CInfoDeskView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	afx_msg LRESULT OnRecordFtpInfo(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


