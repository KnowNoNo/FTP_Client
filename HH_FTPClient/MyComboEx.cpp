// MyComboEx.cpp : 实现文件
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "MyComboEx.h"


// CMyComboEx

IMPLEMENT_DYNAMIC(CMyComboEx, CComboBoxEx)

CMyComboEx::CMyComboEx()
{

}

CMyComboEx::~CMyComboEx()
{
}


BEGIN_MESSAGE_MAP(CMyComboEx, CComboBoxEx)
END_MESSAGE_MAP()



// CMyComboEx 消息处理程序



BOOL CMyComboEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if( pMsg->message == WM_MOUSEWHEEL)
	{
		return TRUE;
	}

	return CComboBoxEx::PreTranslateMessage(pMsg);
}
