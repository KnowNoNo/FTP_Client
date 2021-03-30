#include "StdAfx.h"
#include "FTPCmdClass.h"
#include "ServeView.h"
#include <mstcpip.h>

CFTPCmdClass::CFTPCmdClass(void)
{
	m_socketCtrl = NULL;
	m_socketData = NULL;
	m_bIsConnect = FALSE;
	m_bSupportResume = FALSE;
	m_chLine     = NULL;
	m_pFile       = NULL;
	m_hEvtEndModule = NULL;
}

CFTPCmdClass::~CFTPCmdClass(void)
{
	LogOff();
	//::WSACleanup();
}

BOOL CFTPCmdClass::Open(CString csHost,int nPort,SOCKET &socketCtrl,BOOL bSetKeepAlive/* = TRUE*/)
{
	if(csHost.GetLength() <= 0 || nPort < 1)
	{
		TRACE(L"CFTPCmdClass::Open host/port 有问题");
		return FALSE;
	}
	//csLog.Format(L"Connect to server[%s:%d] ...",csHost,nPort);
	Log(L_NORMAL,L"Try  to Connect to server[%s:%d] ...",csHost,nPort);
	Close(socketCtrl);

	char pAddr[MAX_PATH];

	//建立套接字
	socketCtrl = socket(AF_INET,SOCK_STREAM,0);
	//建立套接字失败，则返回false
	if (socketCtrl == SOCKET_ERROR) 
	{
		socketCtrl = NULL;
		TRACE(L"socker create fail!");
		return FALSE;
	}

	//设置套接字KEEPALIVE机制  默认：超时60s，每5秒发一次,探测次数为5 次
	if(bSetKeepAlive && !SetKeepAlive(socketCtrl))
	{
		Log(L_WARNING,L"keepalive 机制设置失败");
	}

	//定义结构体sockaddr_in，用于设置地址信息。
	struct sockaddr_in sockAddr;
	//将一个点间隔地址转换成一个in_addr
	CStringToChar(csHost,pAddr);
	sockAddr.sin_addr.S_un.S_addr= inet_addr(pAddr);
	//设置协议族，在socket编程中只能是AF_INET
	sockAddr.sin_family=AF_INET;
	//将主机的无符号短整形数转换成网络字节顺序
	sockAddr.sin_port=htons(nPort);
	//连接服务器
	if(connect(socketCtrl, (sockaddr*)&sockAddr,sizeof(sockAddr)) != 0)
	{	
		Close(socketCtrl);
		return FALSE;
	}
	return TRUE;
}

void CFTPCmdClass::Close(SOCKET &socketCtrl)
{
	if(socketCtrl !=NULL && socketCtrl != SOCKET_ERROR)
	{
		::shutdown(socketCtrl, 2);
		::closesocket(socketCtrl);
		socketCtrl = NULL;
	}
}

BOOL CFTPCmdClass::LogOn(CString szFtpName,int nPort,CString szFtpUser,CString szFtpPassword)
{
	m_csIp = szFtpName;
	m_nPort = nPort>0?nPort:DEFAULT_FTP_PORT;
	m_csUsername = szFtpUser.GetLength()>0?szFtpUser:DEFAULT_FTP_USER;
	m_csPassword = szFtpPassword;
	//Control socket connect serve
	if(!Open(m_csIp,m_nPort,m_socketCtrl) || Response() != 220)
	{
		Log(L_DEBUG,L"Connect to server[%s:%d] failed!",m_csIp,m_nPort);
		return FALSE;
	}
	Log(L_DEBUG,L"Connect to server[%s:%d] successfully!",m_csIp,m_nPort);

	// 登录
	if ( m_csUsername.IsEmpty () )
	{
		m_csUsername = L"anonymous";
	}
	csLog.Format( L"USER %s\r\n", m_csUsername);
	if ( !Send(csLog) ) return FALSE;
	nResponseCode = Response ();
	
	// 需要密码
	if(nResponseCode == 331 )
	{
		csLog.Format(L"PASS %s\r\n", m_csPassword );
		if ( !Send(csLog)) return FALSE;
		nResponseCode = Response ();
	}
	
	// 登录失败
	if(nResponseCode != 230 )
	{
		Log ( L_WARNING, L" Login failed");
		return FALSE;
	}
	
	m_bIsConnect = TRUE;
	//服务器是否支持断点续传
	if( !Send(L"REST 1\r\n") || Response() != 350)
	{
		m_bSupportResume = FALSE;
		Log(L_NORMAL,L"服务器不支持断点续传");
	}
	else
	{
		m_bSupportResume = TRUE;
		Log(L_NORMAL,L"服务器支持断点续传");
	}
	
	Log ( L_NORMAL,L"(R) Welcome message：%s\r\n",m_csRetMsg);
	
	m_csRetMsg.Empty();
	return TRUE;
}

BOOL CFTPCmdClass::LogOff()
{
	if(m_bIsConnect && m_socketCtrl > NULL)
	{
		if(!Send(L"QUIT\r\n") || Response() != 221)
		{
			Log(L_WARNING,L"服务器关闭出现错误");
		}
		else Log(L_NORMAL,L"服务器关闭");
	}
	if(m_chLine)
	{
		delete m_chLine;
		m_chLine = NULL;
	}
	if(m_pFile)
	{
		CLOSE_FILE_HANDLE(m_pFile);
	}
	m_bIsConnect = FALSE;
	Close(m_socketCtrl);
	Close(m_socketData);
	
	return TRUE;
}

BOOL CFTPCmdClass::Send(CString csSend)
{
	int  nRet = 0;
	char *pSend = new char[MAX_SEND_SIZE];
	//csLog.Format(L"(S) %s",csSend);
	Log(L_DEBUG,L"Send string\r\n-------->\r\n%s\r-------->",csSend);
	
	CStringToChar(csSend,pSend);
	nRet = ::send(m_socketCtrl, pSend,strlen(pSend), 0);
	if(nRet == SOCKET_ERROR )
	{
		Log(L_DEBUG,L"send failed:has send data size = %d",nRet);
		delete[] pSend;
		return FALSE;
	}
	delete[] pSend;
	return TRUE;
}

//
// return : ------------------------------------------
//		> 0		-	回应代码
//		= 0		-	未读到数据
//		= -1	-	网络出错
//
int CFTPCmdClass::Response()
{
	ASSERT(m_socketCtrl != INVALID_SOCKET );

	int nRet = 0;
	char *pRecv = new char[MAX_RECV_CMD_SIZE];
	nRet = ::recv(m_socketCtrl, pRecv, MAX_RECV_CMD_SIZE, 0);
	if(nRet <= 0)
	{
		if ( nRet < 0 ) Log ( L_DEBUG, L"(R) Receive response data failed" );
		delete [] pRecv;
		return nRet;
	}
	pRecv[nRet] = 0;
	CharToCString(pRecv,m_csRetMsg);
	Log(L_DEBUG,L"(R)%s",m_csRetMsg);

	delete [] pRecv;
	return GetDigitStrAtHead(m_csRetMsg);
}

int CFTPCmdClass::GetDigitStrAtHead(CString csStr)
{
	csStr.TrimLeft(); csStr.TrimRight();
	CString csDigitStr;
	for ( int i=0; isdigit ( (int)csStr[i] ); i++ )
	{
		csDigitStr += csStr[i];
	}

	return CStringToInt(csDigitStr);
}

BOOL CFTPCmdClass::List(CString &csFTPDir)
{
	SOCKET socketData;//数据套接字
	//set transfrom  type ASCII
	if( !Send(L"TYPE A\r\n") || Response() != 200)
	{
		Log(L_DEBUG,L"set type A faile！");
		return FALSE;
	}
	
	// set ftp dir
	csLog.Format(L"CWD %s\r\n",csFTPDir);
	if( !Send(csLog) || Response() != 250)
	{
		Log(L_DEBUG,L"Dir set fail!");
		return FALSE;
	}
	
	if( !Send(L"PWD \r\n") || Response() != 257)
	{
		Log(L_DEBUG,L"Dir set fail!");
		return FALSE;
	}
	//获取当前路径
	if( !GetCurrentDir(csFTPDir))
	{
		return FALSE;
	}

	//create data connect
	if(!CreateDataConnect(socketData)) 
	{
		Close(socketData);
		return FALSE;
	}

	//
	if( !Send(L"LIST \r\n") || Response() != 150 || Response() != 226)
	{
		Close(socketData);
		Log(L_DEBUG,L"set type A fail!");
		return FALSE;
	}

	if( ReceiveData(socketData) < 0)
	{
		Close(socketData);
		return FALSE;
	}
	
	Close(socketData);
	return TRUE;
}

BOOL CFTPCmdClass::GetFileItem(int& nIndex)
{
	int nSum = 0;
	BOOL bLine = FALSE;
	int nBytes = m_buf.GetSize();
	CString csTemp;
	m_csLine.Empty();
	m_chLine = new char[nBytes*2];
	while ( bLine == FALSE && nIndex < nBytes )
	{
		char ch = (char)(m_buf.GetAt( nIndex ));
		switch( ch )
		{
		case '\n': // 行尾
			bLine = TRUE;
			break;
		default:   // 其他情况
			m_chLine[nSum++] = ch;
			break;
		}
		++nIndex;
	}
	m_chLine[nSum] = 0;

	UTF8ToUnicode(m_chLine,m_csLine);
	//m_csLine;
	delete[] m_chLine;
	m_chLine = NULL;
	return bLine;
}

BOOL CFTPCmdClass::GetCurrentDir(CString &csFTPDir)
{
	int nBgn,nEnd;
	nBgn = m_csRetMsg.Find(L'"');
	nEnd = m_csRetMsg.Find(L'"',nBgn+1);
	if(nBgn >= nEnd-1) return FALSE;
	csFTPDir = m_csRetMsg.Mid(nBgn+1,nEnd-nBgn-1);
	return TRUE;
}

BOOL CFTPCmdClass::OpenFile(CFile* file,CString csFileName,BOOL bUpload,ULONGLONG& nStartPos)
{
	ASSERT ( !csFileName.IsEmpty() );
	if(!file) return FALSE;
	// 打开本地文件

	BOOL bRet = FALSE;
	TRY
	{
		//上传
		if(	bUpload )
		{
			if( file->Open ( csFileName, CFile::modeReadWrite) )
			{
				if(file->Seek ( nStartPos, CFile::begin ) == nStartPos )
					bRet = TRUE;
			}
		}
		else//下载
		{	
			if( file->Open ( csFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeBinary|CFile::shareDenyNone))
			{
				//nStartPos 0 为force 1为 noforce
				if(nStartPos)
				{
					nStartPos = file->GetLength();
				}

				if(file->Seek ( nStartPos, CFile::begin ) == nStartPos ) 
					bRet = TRUE;
			}		
		}
	}
	CATCH( CFileException, e )
	{
		e->Delete ();
		bRet = FALSE;
	}
	END_CATCH
	if ( !bRet )
	{
		Log ( L_DEBUG, L"Open file [%s] failed. ", csFileName);
	}

	return bRet;
}

BOOL CFTPCmdClass::UploadFile(CString csLocDir,CString csLocFileName,ULONGLONG ullLocFileSize,CString csServDir,ULONGLONG* pullServFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort/*=false*/,bool bIsForce/*=false*/)
{
	ASSERT(m_socketCtrl && m_bIsConnect);
	//data
	SOCKET       socketData;
	ULONGLONG	 nServFileSize = 0;
	ULONGLONG    nStartPos;
	BOOL         bHaveServFile = FALSE;
	CString      csLocFileDir;

	//检测服务器路径
	csLog.Format(L"CWD %s\r\n",csServDir);
	if( !Send(csLog) || Response() != 250)
	{
		Log(L_DEBUG,L"Dir set fail!");
		return FALSE;
	}
	SLEEP_RETURN(0);

	//检测服务器文件大小
	csLog.Format(L"SIZE %s\r\n",csLocFileName);
	if( !Send(csLog))	
		return FALSE;
	nResponseCode = Response ();
	if(nResponseCode == 550)	//未发现文件
	{
		nServFileSize = 0;
		*pullServFileSize = 0;
	}
	else if(nResponseCode == 213)	//已有文件
	{
		bHaveServFile = TRUE;
		nServFileSize = CStringToLong( m_csRetMsg.Right(m_csRetMsg.GetLength()-4));
		*pullServFileSize = nServFileSize;
	}
	else
	{	
		Log(L_DEBUG,L"size command fail!");
		return FALSE;
	}
	SLEEP_RETURN(0);

	// 同名文件存在 且服务器文件 大于等于 本地文件
	if(!bIsForce && *pullServFileSize >= ullLocFileSize)
	{
		Log(L_WARNING,L"上传[%s]文件发生错误: 本地文件小于服务器文件",csLocFileName);
		return TRUE;
	}

	//打开本地文件
	csLocFileDir = csLocDir;//文件目录+文件名
	if(csLocDir.Right(1) != L"\\")
		csLocFileDir += L"\\";
	csLocFileDir += csLocFileName;

	nStartPos =  bIsForce ? 0:nServFileSize;//起始位置
	m_pFile = new CFile;
	if(!OpenFile(m_pFile,csLocFileDir , TRUE , nStartPos))
	{
		CLOSE_FILE_HANDLE(m_pFile);
		Log(L_WARNING,L"%s文件打不开",csLocFileName);
		return FALSE;
	}

	SLEEP_RETURN(0);

	//设置传输数据类型为 二进制模式
	if( !Send(L"TYPE I\r\n") || Response() != 200)
	{
		CLOSE_FILE_HANDLE(m_pFile);
		Log(L_DEBUG,L"set type I failed！");
		return FALSE;
	}
	SLEEP_RETURN(0);

	//创建pasv模式下的数据通道
	if(1 || !bIsPort)
	{
		if( !CreateDataConnect(socketData) )
		{
			CLOSE_FILE_HANDLE(m_pFile);
			return FALSE;
		}
	}
	else
	{
		//port模式
	}
	SLEEP_RETURN(0);

	//断点续传
	if(!bIsForce && m_bSupportResume)
	{
		csLog.Format(L"REST %d\r\n",nStartPos);
		if( !Send(csLog) || Response() != 350)
		{
			CLOSE_FILE_HANDLE(m_pFile);
			Log(L_DEBUG,L"rest command failed！");
			return FALSE;
		}
	}
	
	csLog.Format(L"STOR %s\r\n",csLocFileName);
	if( !Send(csLog) || Response() != 150)
	{
		CLOSE_FILE_HANDLE(m_pFile);
		Log(L_DEBUG,L"STOR command failed！");
		return FALSE;
	}
	SLEEP_RETURN(0);

	if( !SendData(m_pFile , socketData , ullTPSize , hEvtEndModule) )
	{
		CLOSE_FILE_HANDLE(m_pFile);
		return FALSE;
	}
	//关闭SOCKET
	Close(socketData);
	//返回值为226则传输完成
	if( Response() != 226 )
	{
		CLOSE_FILE_HANDLE(m_pFile);
		return FALSE;
	}

	CLOSE_FILE_HANDLE(m_pFile);
	return TRUE;
}

BOOL CFTPCmdClass::CreateDir(CString csDirName)
{	
	csLog.Format(L"MKD %s\r\n",csDirName);
	if( !Send(csLog) || Response() != 257)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CFTPCmdClass::DeleteDirOrFile(CString csName , UINT uMark)
{
	csLog = uMark==FILEICON ? L"DELE ":L"RMD ";
	csLog += csName + L"\r\n";
	if( !Send(csLog) || Response() != 250)
	{
		return FALSE;
	}
	return TRUE;
}

void CFTPCmdClass::SetEventOfEndModule(HANDLE hEvtEndModule)
{
	m_hEvtEndModule = hEvtEndModule;
}

BOOL CFTPCmdClass::CreateDataConnect(SOCKET &socketData)
{
	CString csIP;
	int     nPort;
	unsigned long   uArgp = 0;//SOCKET 禁止非阻塞模式
	if( !Send(L"PASV\r\n") || Response() != 227)
	{
		Log(L_DEBUG,L"set type A faile！");
		return FALSE;
	}
	GetIPAndPortByPasvString(m_csRetMsg,csIP,nPort);

	if(!Open(csIP,nPort,socketData,FALSE)) return FALSE;
	
	//SOCKET 设置禁止非阻塞模式
	if((ioctlsocket (socketData, FIONBIO, &uArgp)) != 0) return FALSE;
	return TRUE;
}

BOOL CFTPCmdClass::GetIPAndPortByPasvString(LPCTSTR lpszPasvString, CString &csIP, int &nPort)
{
	if ( !lpszPasvString ) return FALSE;
	TCHAR *p =const_cast<TCHAR*>(wcschr ( lpszPasvString, L'(' ));
	if ( !p ) return FALSE;
	CString csPasvStr = p+1, csTemp;
	int nPosStart = 0, nPosEnd = 0;
	int nMultiple = 0, nMantissa = 0;
	for ( int i=0; ; i++ )
	{
		nPosEnd = csPasvStr.Find ( L",", nPosStart );
		if ( nPosEnd < 0 )
		{
			if ( i == 5 )
			{
				nPosEnd = csPasvStr.Find ( L")", nPosStart );
				csTemp = csPasvStr.Mid ( nPosStart, nPosEnd-nPosStart );
				nMantissa = CStringToInt( csTemp );
				break;
			}
			else return FALSE;
		}
		csTemp = csPasvStr.Mid ( nPosStart, nPosEnd-nPosStart );
		csTemp.TrimLeft(); csTemp.TrimRight();
		if ( i < 4 )
		{
			if ( !csIP.IsEmpty () ) csIP += L".";
			csIP += csTemp;
		}
		else if ( i == 4 )
		{
			nMultiple = CStringToInt( csTemp );
		}
		else return FALSE;
		nPosStart = nPosEnd + 1;
	}
	nPort = nMultiple*256 + nMantissa;

	return TRUE;
	
}

LONGLONG CFTPCmdClass::ReceiveData(SOCKET &socketRecv)
{
	ULONGLONG sum = 0;
	ULONGLONG num = 0;
	m_buf.RemoveAll();
	m_buf.SetSize(ONCE_RECV_DATA_SIZE);

	while(1) { // 获得数据
		num = ::recv(socketRecv, (char *)(m_buf.GetData()+ sum), ONCE_RECV_DATA_SIZE, 0);
		if( num == SOCKET_ERROR)
			return -1;
		if( num == 0)break;
		TRACE(L"Received :%llu\n", num);
		Sleep(0);
		sum += num;
		m_buf.SetSize((int)sum + ONCE_RECV_DATA_SIZE);
	}
	return sum;
}

BOOL CFTPCmdClass::ReceiveData(CFile* file,SOCKET &socketReceive,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule)
{
	if(!file) return FALSE;
	int	nRecvSize;
	BOOL bRet = TRUE;
	char* pBuf = new char[NET_BUFFER_SIZE];
	ULONGLONG ullSum = 0;
	TRY 
	{
		while(1)
		{
			//
			if( ::WaitForSingleObject ( hEvtEndModule, 0 ) == WAIT_OBJECT_0 )
			{
				bRet = FALSE;
				break;
			}
			nRecvSize = ::recv(socketReceive, pBuf, NET_BUFFER_SIZE, 0);
			if(nRecvSize == NULL || nRecvSize == SOCKET_ERROR)	break;//EOF

			file->Write(pBuf,nRecvSize);

			ullSum += nRecvSize;
			*ullTPSize = ullSum;

		}
	}
	CATCH (CMemoryException, e)
	{
		e->Delete();
		bRet = FALSE;
	}
	END_CATCH
	delete[] pBuf;
	return bRet;
}

BOOL CFTPCmdClass::SendData(CFile* file,SOCKET &socketSend,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule)
{	
	if(!file) return FALSE;
	int	nReadFormFile,nSendSize;
	char* pBuf = new char[NET_BUFFER_SIZE];
	ULONGLONG ullSum = 0;
	while(1)
	{
		//
		if( ::WaitForSingleObject ( hEvtEndModule, 0 ) == WAIT_OBJECT_0 )
		{
			delete[] pBuf;
			return FALSE;
		}
		nReadFormFile = file->Read(pBuf , NET_BUFFER_SIZE);
		if(nReadFormFile == NULL)	break;//EOF
		nSendSize = ::send(socketSend , pBuf ,nReadFormFile , 0);
		if(nSendSize == SOCKET_ERROR)	break;
		ullSum += nSendSize;
		*ullTPSize = ullSum;
		//调整文件指针
		if(nReadFormFile != nSendSize)
		{
			file->Seek(nSendSize - nReadFormFile , CFile::current);
		}
	}
	delete[] pBuf;
	return TRUE;
}

BOOL CFTPCmdClass::DownloadFile(CString csLocDir,CString csServFileName,ULONGLONG ullServFileSize,CString csServDir,ULONGLONG* pullLocFileSize,volatile ULONGLONG *ullTPSize,HANDLE hEvtEndModule,bool bIsPort/*=false*/,bool bIsForce/*=false*/)
{
	ASSERT(m_socketCtrl && m_bIsConnect);
	//data
	SOCKET       socketData;
	ULONGLONG	 nServFileSize = 0;
	ULONGLONG    nStartPos = 0;
	BOOL         bHaveLocFile = FALSE;
	CString      csLocFileDir;

	//检测服务器路径
	csLog.Format(L"CWD %s\r\n",csServDir);
	if( !Send(csLog) || Response() != 250)
	{
		Log(L_DEBUG,L"Dir set fail!");
		return FALSE;
	}
	SLEEP_RETURN(0);

	//检测服务器文件大小

	csLog.Format(L"SIZE %s\r\n",csServFileName);
	if( !Send(csLog))	
		return FALSE;
	nResponseCode = Response ();
	if(nResponseCode == 550)	//未发现文件
	{
		Log(L_WARNING,L"服务器找不到%s文件!",csServFileName);
		return FALSE;
	}
	else if(nResponseCode != 213)	//没有文件
	{
		Log(L_WARNING,L"size command fail!");
		return FALSE;
	}
	SLEEP_RETURN(0);

	//打开本地文件
	csLocFileDir = csLocDir;//文件目录+文件名
	if(csLocDir.Right(1) != L"\\")
		csLocFileDir += L"\\";
	csLocFileDir += csServFileName;

	nStartPos =  bIsForce?0:1;//起始位置
	m_pFile = new CFile;
	if(!OpenFile(m_pFile,csLocFileDir , FALSE , nStartPos))
	{
		CLOSE_FILE_HANDLE(m_pFile);
		Log(L_WARNING,L"%s文件打开失败",csServFileName);
		return FALSE;
	}
	*pullLocFileSize = nStartPos;
	SLEEP_RETURN(0);

	// 同名文件存在 且本地文件 大于等于 服务器文件
	if(!bIsForce && *pullLocFileSize >= ullServFileSize)
	{
		Log(L_WARNING,L"下载[%s]文件错误: 服务器文件小于本地文件",csServFileName);
		CLOSE_FILE_HANDLE(m_pFile);
		return TRUE;
	}

	//设置传输数据类型为 二进制模式
	if( !Send(L"TYPE I\r\n") || Response() != 200)
	{
		CLOSE_FILE_HANDLE(m_pFile);
		Log(L_DEBUG,L"set type I failed！");
		return FALSE;
	}
	SLEEP_RETURN(0);

	//创建pasv模式下的数据通道
	if(1 || !bIsPort)
	{
		CreateDataConnect(socketData);
	}
	else
	{
		//port模式
	}
	SLEEP_RETURN(0);

	//断点续传
	if(!bIsForce && m_bSupportResume)
	{
		csLog.Format(L"REST %d\r\n",nStartPos);
		if( !Send(csLog) || Response() != 350)
		{
			CLOSE_FILE_HANDLE(m_pFile);
			Log(L_DEBUG,L"rest command failed！");
			return FALSE;
		}
	}

	csLog.Format(L"RETR %s\r\n",csServFileName);
	if( !Send(csLog) || Response() != 150)
	{
		CLOSE_FILE_HANDLE(m_pFile);
		Log(L_DEBUG,L"STOR command failed！");
		return FALSE;
	}
	SLEEP_RETURN(0);

	if( !ReceiveData(m_pFile , socketData , ullTPSize , hEvtEndModule) )
	{
		CLOSE_FILE_HANDLE(m_pFile);
		return FALSE;
	}
	//关闭SOCKET
	Close(socketData);
	//返回值为226则传输完成
	if( Response() != 226 )
	{
		CLOSE_FILE_HANDLE(m_pFile);
		return FALSE;
	}

	CLOSE_FILE_HANDLE(m_pFile);
	return TRUE;
}

BOOL CFTPCmdClass::SetKeepAlive(SOCKET socket_handle)
{
	// 开启KeepAlive
	BOOL bKeepAlive = TRUE;
	int nRet = ::setsockopt(socket_handle, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive));

	if (nRet == SOCKET_ERROR)
	{
		return FALSE;
	}

	// 设置KeepAlive参数
	tcp_keepalive alive_in = {0};
	tcp_keepalive alive_out = {0};
	alive_in.keepalivetime = 60000; // 开始首次KeepAlive探测前的TCP空闭时间

	alive_in.keepaliveinterval = 5000; // 两次KeepAlive探测间的时间间隔

	alive_in.onoff = TRUE;
	unsigned long ulBytesReturn = 0;

	nRet = WSAIoctl(socket_handle, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);
	if (nRet == SOCKET_ERROR)
	{
		return FALSE;
	}
	return TRUE;
}
