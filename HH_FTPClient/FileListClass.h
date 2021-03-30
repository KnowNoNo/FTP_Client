#pragma once
#include "PublicFunction.h"
#include <map>
using namespace std;

#define		LOCAL_DEFAULT_DIRECTORYL	L"C:\\"
typedef struct		// 文件信息
{
	TCHAR szFileName[MAX_PATH];
	TCHAR szFileDate[MAX_PATH];
	TCHAR szFileSize[30];
	TCHAR szFileByte[30];
	int nType;
}FILE_FTP_INFO;

typedef struct		//
{
	BOOL	m_bDirectory;
	WORD	m_bSec;
	//CString length;
	CString m_strSec;
	CString m_strName;
	CString m_strDate;
	CString m_strSize;
	CString m_strByte;
	int m_index;
}FILE_FTP_INFO2;

typedef struct
{
	CString		csTPFileName;						// 传输文件名   也是MAP_ALL_UPLOADFILE_INFO map结构的中的KEY值
	ULONGLONG	ullTPFileByte;						// 传输文件大小

	CString		csLocFileDir;
	CString		csServFileDir;
	CString		csServIp;
	int			nServPort;
	CString		csServUser;
	CString		csServPassword;
	bool		bIsPort;
	bool		bIsForce;
	UINT        uIsTpOk;
	bool		bTPMode;							// 0 为上传  1 为下载
	CWinThread* pUploadThread;						// 任务线程
	HANDLE		hEvtEndModule;						// 模块结束事件
	ULONGLONG   ullTPDestnFileSize ;				// 传输目的地所在同名文件字节
	volatile ULONGLONG	ullTpFileSize;				// 实时记录传输字节数
	volatile ULONGLONG  ullTPFileLastSize;			// 记录前一秒的传输字节数，用来统计网络速度
}FILE_UPLOAD_INFO;

typedef map<CString,FILE_UPLOAD_INFO>  MAP_ALL_UPLOADFILE_INFO;

typedef struct
{
	MAP_ALL_UPLOADFILE_INFO mpFileInfo;
	int nThreadCount;
}FILE_UPLOAD_COUNT_INFO;


typedef struct  
{
	CString csLocDir;
	CString csUpOrDownLoad;
	CString csServDir;
	CString csFileName;
	CString csFileSize;
	CString csStatus;
	//UINT    uDlgSign;
}FILE_FTP_TP_STATUS;

enum
{
	FILE_TP_JOIN,
	FILE_TP_FAIL,
	FILE_TP_SUCCESS
};

class CFileListClass
{
public:
	CFileListClass(void);
	virtual ~CFileListClass(void);

protected:
	HCURSOR m_hDragCursor;			// 拖曳列表框中某项时显示的图标
	BOOL    m_bDragging;
	CImageList* m_pDragImageList;	// 图像列表对象
	CImageList  m_ctImageList;
	CMenu		m_LocServMenu;
	
public:
	void SetFileColumns(CListCtrl* pListCtrl);
	BOOL SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo);
	void GetFileIcon(CString& fileName,int* iIcon,int* iIconSel=NULL,int nCur = LOCFILE);
	void SetPopMenu(int nIndex);
	void InsertFile(CListCtrl* pListCtrl,FILE_FTP_INFO* pInfo,int lParam,UINT& nDirNum);
	BOOL TransformLine(CString strLine,FILE_FTP_INFO &serFileInfo);
	CString ByteConnvertType(ULONGLONG uByte);
};
