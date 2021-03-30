#pragma once


// CMyComboEx

class CMyComboEx : public CComboBoxEx
{
	DECLARE_DYNAMIC(CMyComboEx)

public:
	CMyComboEx();
	virtual ~CMyComboEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


