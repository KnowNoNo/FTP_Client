#include "stdafx.h"
#include "PublicFunction.h"
#include "ServeView.h"
#include "LocView.h"
#include "MainFrame.h"

//
void TcharToChar(TCHAR *tchar,char *_char)
{
	int nLength = WideCharToMultiByte(CP_ACP,0,tchar,-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,tchar,-1,_char,nLength,NULL,NULL);
	_char[nLength + 1] = '\0';
}

void CStringToChar(CString &csStr,char *_char)
{
	int nLength = WideCharToMultiByte(CP_ACP,0,csStr.GetBuffer(),-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,csStr.GetBuffer(),-1,_char,nLength,NULL,NULL);
	_char[nLength + 1] = '\0';
}

void CharToCString(char *_char,CString &csStr)
{
	int nLength = MultiByteToWideChar(CP_ACP,0,_char,-1,NULL,0);
	TCHAR *buf = new TCHAR[nLength+1];
	MultiByteToWideChar(CP_ACP,0,_char,-1,buf,nLength);
	buf[nLength] = '\0';
	csStr.Format(L"%s",buf);
	delete []buf;
}

int CStringToInt(CString &csStr)
{
	return _ttoi(csStr);
}

long CStringToLong(CString &csStr)
{
	return _ttol(csStr);
}

void CStringToTchar(CString &csStr,TCHAR *pTchar)
{
	_tcscpy(pTchar,csStr);
}

bool MBToUTF8(char* pmb,char *pu8)  
{  
	// convert an MBCS string to widechar   
	int mLen = strlen(pmb);
	int nLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);  
	
	WCHAR* lpszW = NULL;  

	lpszW = new WCHAR[nLen];  

	int nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, lpszW, nLen);  
	// convert an widechar string to utf8  
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, pu8, utf8Len, NULL, NULL);  
	pu8[utf8Len] = 0;
	delete[] lpszW;  
	return true;  
}  
/*
// UTF8编码转为多字节编码  
bool UTF8ToMB(vector<char>& pmb, const char* pu8, int32 utf8Len)  
{  
	// convert an UTF8 string to widechar   
	int32 nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
		lpszW = new WCHAR[nLen];  
	}  
	catch(bad_alloc &memExp)  
	{  
		return false;  
	}  

	int32 nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
		delete[] lpszW;  
		return false;  
	}  

	// convert an widechar string to Multibyte   
	int32 MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (MBLen <=0)  
	{  
		return false;  
	}  
	pmb.resize(MBLen);  
	nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, &*pmb.begin(), MBLen, NULL, NULL);  
	delete[] lpszW;  

	if(nRtn != MBLen)  
	{  
		pmb.clear();  
		return false;  
	}  
	return true;  
}  
*/
//
bool UTF8ToUnicode(char* pu8,CString &pun)  
{  
	// convert an UTF8 string to widechar   
	int utf8Len = strlen(pu8);
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);  
	TCHAR *buf = new TCHAR[nLen+1];
	int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, buf, nLen);  
	buf[nLen] = 0;
	pun.Format(L"%s",buf);
	delete[] buf;
	return true;  
}  
/*
// Unicode编码转为UTF8  
bool UnicodeToUTF8(vector<char>& pu8, const wchar_t* pun, int32 uLen)  
{  
	// convert an widechar string to utf8  
	int32 utf8Len = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, NULL, 0, NULL, NULL);  
	if (utf8Len<=0)  
	{  
		return false;  
	}  
	pu8.resize(utf8Len);  
	int32 nRtn = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, &*pu8.begin(), utf8Len, NULL, NULL);  

	if (nRtn != utf8Len)  
	{  
		pu8.clear();  
		return false;  
	}  
	return true;  
} 
*/
////
void Log(UINT nLevel, WCHAR* format, ...)
{
#ifndef _DEBUG
	if(nLevel == L_DEBUG)
		return;
#endif

	//LPCWSTR	 format = csFormat.AllocSysString();
	CString* csInfo = new CString;
	//int k = _tcslen(format);
	// 获取当前时间
	SYSTEMTIME st;
	GetLocalTime(&st);
	csInfo->Format(L"[%04d-%02d-%02d %02d:%02d:%02d] ",st.wYear,
		st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	// 日志级别
	int nLogLevel = (nLevel & 0xf);
	ASSERT ( nLogLevel >= L_VERBOSE && nLogLevel < L_LOGNUM );
	*csInfo += L"<";
	*csInfo += g_szLogLevelStr[nLogLevel];
	*csInfo += L"> : ";
	// 格式化日志信息
	TCHAR szDataBuf[NET_BUFFER_SIZE] = {0};
	va_list  va;
	va_start (va, format);
	_vsnwprintf ( szDataBuf, NET_BUFFER_SIZE-1, (const wchar_t*)format, va);
	va_end(va);
	*csInfo += szDataBuf;
	//*csInfo += format;
	//// 添加回车换行符
	//*csInfo += L"\r\n";
	CInfoDeskView* m_pWnd =  ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pInfoDeskView;
	if(::IsWindow(m_pWnd->m_hWnd))
		m_pWnd->SendMessage(WM_RECORDFTPINFO,0,(LPARAM)csInfo);
}


//
//{
	BOOL CPublicData::IsFree = NULL;
	CString CPublicData::szAppName;
	CString CPublicData::szFtpName;
	CString CPublicData::szFtpPassword;;
	CString CPublicData::szFtpUser;
	CString CPublicData::szFtpDirectory;
	CString CPublicData::szLocDirectory;
	int CPublicData::nPort = NULL;
	bool CPublicData::bIsPort = FALSE;
	bool CPublicData::bIsForce  = FALSE;
	int CPublicData::nCount   = NULL;
	int CPublicData::nDirIcon = NULL;
	bool CPublicData::bButtonEnable = TRUE;//未登录
	CCriticalSection CPublicData::CSFor_UploadCount;
//}


CMyAppThread::CMyAppThread(void)
{
}

CMyAppThread::~CMyAppThread(void)
{
}


BOOL CMyAppThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMyAppThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CMyAppThread message handlers

UINT CMyAppThread::ThreadProc_FindLocFile(LPVOID lParam)
{	
	CLocView* pLocView = (CLocView*)lParam;
	ASSERT(pLocView);
	pLocView->ThreadProc_FindLocFile();
	TRACE(L"ThreadProc_FindLocFile线程函数 执行完毕\n");
	//if(pLocView->m_pFindLocFileThread)AfxMessageBox(L"asdsadsads");
	return true;
}

UINT CMyAppThread::ThreadProc_FindServFile(LPVOID lParam)
{
	CServeView* pServView = (CServeView*)lParam;
	ASSERT(pServView);
	pServView->ThreadProc_FindServFile();
	TRACE(L"ThreadProc_FindServFile线程函数 执行完毕\n");
	return true;
}

UINT CMyAppThread::ThreadProc_UploadOrDownloadFile(LPVOID lParam)
{
	CServeView* pServView =  ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_pServeView;
	ASSERT(pServView);
	pServView->ThreadProc_UploadOrDownloadFile((FILE_UPLOAD_INFO*)lParam);
	TRACE(L"ThreadProc_UploadOrDownloadFile线程函数 执行完毕\n");
	return true;
}

UINT CMyAppThread::ThreadProc_CreateServDir(LPVOID lParam)
{
	CServeView* pServView = (CServeView*)lParam;
	ASSERT(pServView);
	pServView->ThreadProc_CreateServDir();
	TRACE(L"ThreadProc_CreateServDir线程函数 执行完毕\n");
	//delete (CString*)lParam;
	return true;
}

UINT CMyAppThread::ThreadProc_DeleteServDir(LPVOID lParam)
{
	CServeView* pServView = (CServeView*)lParam;
	ASSERT(pServView);
	pServView->ThreadProc_DeleteServDir();
	TRACE(L"ThreadProc_DeleteServDir线程函数 执行完毕\n");
	return true;
}
