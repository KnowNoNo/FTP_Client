//#include "stdafx.h"
#pragma once
#include "afxwin.h"

#define  INFODESK_BEGIN_SHOW \
L"\r\n\
/*********************/\r\n \
FTP文件传输-支持断点续传\r\n \
					-ZLH\r\n \
/********************/\r\n\
"
// 有效的句柄
#define HANDLE_IS_VALID(h) ((HANDLE)(h) && ((HANDLE)(h)!=INVALID_HANDLE_VALUE))
// 关闭句柄
#define CLOSE_HANDLE(h)\
{\
	if ( HANDLE_IS_VALID(h) )\
	{\
	::CloseHandle ( h );\
	(h) = NULL;\
	}\
}

#define CLOSE_FILE_HANDLE(h)\
{\
	if ( HANDLE_IS_VALID(h->m_hFile) )\
	{\
	h->Close();\
	delete h;\
	(h) = NULL;\
	}\
}

#define SLEEP_RETURN(x)\
{\
	if ( ::WaitForSingleObject ( hEvtEndModule, x ) == WAIT_OBJECT_0 )\
	{\
		CLOSE_FILE_HANDLE(m_pFile);\
		return FALSE;\
	}\
}

//默认FTP信息
#define DEFAULT_FTP_INITDIRECTORY	L"/"
#define DEFAULT_FTP_PORT			21
#define DEFAULT_FTP_USER			L"anonymous"
#define DEFAULT_FTP_UPLOAD_TIMES	5

//
#define TIMER_ID_TO_UPDATETPINFO	1
//
#define MAX_UPLOAD_THREAD_SIZE		50

//
#define MAX_DIR_SIZE				260*4
#define MAX_LOGBUF_SIZE				1024*4						// 日志临时缓冲大小
#define EDIT_MAX_TEXT				1024*20						// CEdit 控件最大容纳的文本
#define NET_BUFFER_SIZE				4096						// 默认缓冲大小
// 日志级别
#define L_VERBOSE					0x00000000					// 详细，一般是调试信息
#define L_NORMAL					0x00000001					// 正常，程序运行时显示的一般信息，纪录运行过程
#define L_WARNING					0x00000002					// 警告，程序运行有错误，但不是致命错误，程序仍然可以继续运行下去
#define L_ERROR						0x00000003					// 错误，程序运行出现了严重错误，程序必须立即终止执行
#define L_DEBUG						0x00000004					// 调试，程序在调试版里输出的日志，在Publish版里就没有了
#define L_LOGNUM					5							// 日志级别的数量

const TCHAR g_szLogLevelStr[6][8] = 
{
	L"VERBOSE",L"NORMAL",L"WARNING",L"ERROR",L"DEBUG",L"INVALID",
};

const UINT g_nMsgTextColor[6] =
{
	RGB(64,64,64), RGB(0,0,128), RGB(128,0,0), RGB(255,0,0), RGB(255,0,0), RGB(0,0,0),
};

void Log ( UINT nLevel, WCHAR* format, ...);

#ifdef UNICODE
//unicode  类型转化
void TcharToChar(TCHAR *tchar,char *_char);
void CStringToChar(CString &csStr,char *_char);//可用
void CharToCString(char *_char,CString &csStr);
int  CStringToInt(CString &csStr);//可用
long CStringToLong(CString &csStr);
void CStringToTchar(CString &csStr,TCHAR *pTchar);
//bool MBToUTF8(char* pmb,char *pu8) ;
//bool UTF8ToMB(vector<char>& pmb, const char* pu8, int utf8Len);
bool UTF8ToUnicode(char* pu8,CString &pun); //可用
//bool UnicodeToUTF8(vector<char>& pu8, const wchar_t* pun, int uLen)  ;

#endif
//

enum
{
	FILEICON,
	DIRECTORYICON,
	LOCFILE,
	SERVFILE
};

class CPublicData
{
public:
	CPublicData(void);
	~CPublicData(void);

static BOOL IsFree;
static CString szAppName;
static CString szFtpName;
static CString szFtpUser;
static CString szFtpPassword;
static CString szFtpDirectory;
static CString szLocDirectory;
static int  nPort;
static bool bIsPort;
static bool bIsForce;
static int  nCount;
static int  nDirIcon;
static bool bButtonEnable;
static CCriticalSection CSFor_UploadCount;
};

class CMyAppThread :public CWinThread
{
public:
	CMyAppThread(void);
	~CMyAppThread(void);

	// Operations
public:
	static UINT ThreadProc_FindLocFile(LPVOID lParam);
	static UINT ThreadProc_FindServFile(LPVOID lParam);
	static UINT ThreadProc_UploadOrDownloadFile(LPVOID lParam);
	static UINT ThreadProc_CreateServDir(LPVOID lParam);
	static UINT ThreadProc_DeleteServDir(LPVOID lParam);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

};

