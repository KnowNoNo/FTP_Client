
// HH_FTPClient.h : HH_FTPClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "PublicFunction.h"

// CHH_FTPClientApp:
// �йش����ʵ�֣������ HH_FTPClient.cpp
//

class CHH_FTPClientApp : public CWinApp
{
public:
	CHH_FTPClientApp();


// ��д
public:
	virtual BOOL InitInstance();

public:
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHH_FTPClientApp theApp;
