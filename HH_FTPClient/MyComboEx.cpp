// MyComboEx.cpp : ʵ���ļ�
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



// CMyComboEx ��Ϣ�������



BOOL CMyComboEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if( pMsg->message == WM_MOUSEWHEEL)
	{
		return TRUE;
	}

	return CComboBoxEx::PreTranslateMessage(pMsg);
}
