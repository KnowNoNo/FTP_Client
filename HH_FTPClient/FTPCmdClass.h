#pragma once
#include "PublicFunction.h"

#define MAX_SEND_SIZE		 1024 //255+strlen("SIZE ");
#define MAX_RECV_CMD_SIZE    1024
#define ONCE_RECV_DATA_SIZE  4096
class CFTPCmdClass
{
public:
	CFTPCmdClass(void);
	virtual ~CFTPCmdClass(void);

public:
	SOCKET m_socketCtrl; //����������׽���
	SOCKET m_socketData; //���ݴ���

	int     m_nPort;
	CString m_csIp;
	CString m_csUsername;
	CString m_csPassword;
	
	bool	m_bIsConnect;
	bool    m_bIsPort;
	bool    m_bIsForce;
	bool    m_bSupportResume;
	CString csLog;
	CString m_csLine;
	char*   m_chLine;
	CByteArray m_buf;
	CString m_csRetMsg;
	int nResponseCode;//��Ӧ��
	CFile*  m_pFile;
	HANDLE  m_hEvtEndModule;
public:
	BOOL Open(CString csHost,int nPort,SOCKET &socketCtrl,BOOL bSetKeepAlive = TRUE);
	void Close(SOCKET &socketCtrl);
	BOOL LogOn(CString szFtpName,int nPort,CString szFtpUser,CString szFtpPassword);
	BOOL LogOff();
	BOOL Send(CString csSend);					// ��������
	int  Response();							// �ӷ���������ͨ����ȡһ����Ӧ
	int  GetDigitStrAtHead (CString csStr);
	BOOL CreateDataConnect (SOCKET &socketData);
	BOOL GetIPAndPortByPasvString ( LPCTSTR lpszPasvString, CString &csIP, int &nPort );
	LONGLONG ReceiveData(SOCKET &socketRecv);	// �ӿ���ͨ���������ݲ�����m_buf

	BOOL SendData (CFile* file,SOCKET &socketSend,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule);		// �ϴ�
	BOOL ReceiveData(CFile* file,SOCKET &socketReceive,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule); // ����
	
	BOOL UploadFile(CString csLocDir,CString csLocFileName,ULONGLONG ullLocFileSize,CString csServDir,ULONGLONG* pullServFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort=false,bool bIsForce=false);
	BOOL DownloadFile(CString csLocDir,CString csServFileName,ULONGLONG ullServFileSize,CString csServDir,ULONGLONG* pullLocFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort=false,bool bIsForce=false);
	BOOL CreateDir(CString csDirName);
	BOOL DeleteDirOrFile(CString csName , UINT uMark);
	
	BOOL SetKeepAlive(SOCKET socket_handle);
	BOOL List(CString &csFTPDir);
	BOOL GetFileItem(int& nIndex);
	BOOL GetCurrentDir(CString &csFTPDir);
	BOOL OpenFile(CFile* file,CString csFileName,BOOL bUpload,ULONGLONG& nStartPos);
	
	void SetEventOfEndModule(HANDLE hEvtEndModule);
public:
	

};
