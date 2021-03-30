#pragma once

// CInfoDeskView ��ͼ

class CInfoDeskView : public CEditView
{
	DECLARE_DYNCREATE(CInfoDeskView)

protected:
	CInfoDeskView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


