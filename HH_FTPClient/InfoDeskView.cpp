// InfoDeskView.cpp : 实现文件
//

#include "stdafx.h"
#include "HH_FTPClient.h"
#include "InfoDeskView.h"


// CInfoDeskView

IMPLEMENT_DYNCREATE(CInfoDeskView, CEditView)

CInfoDeskView::CInfoDeskView()
{
	m_pEdit = NULL;
}

CInfoDeskView::~CInfoDeskView()
{
	m_NewFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(CInfoDeskView, CEditView)
	ON_WM_CREATE()
	ON_WM_CHAR()

	ON_MESSAGE(WM_RECORDFTPINFO,OnRecordFtpInfo)
END_MESSAGE_MAP()


// CInfoDeskView 诊断

#ifdef _DEBUG
void CInfoDeskView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoDeskView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInfoDeskView 消息处理程序

int CInfoDeskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_NewFont.CreatePointFont(80,L"Arial",NULL);	// 创建字体GDI对象
	GetEditCtrl().SetFont(&m_NewFont,true);		// 设置字体
	
	m_pEdit = &GetEditCtrl();
	m_pEdit->SetReadOnly(TRUE);
	return 0;
}

void CInfoDeskView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}

LRESULT CInfoDeskView::OnRecordFtpInfo(WPARAM wParam,LPARAM lParam)
{
	//CEdit& edit=GetEditCtrl(); 
	
	CString* str=(CString*)lParam;
	int nLine   = m_pEdit->GetLineCount();
	int nStart  = m_pEdit->LineIndex(nLine);
	m_pEdit->SetSel(nStart,str->GetLength()+nStart);
	m_pEdit->ReplaceSel(L"\r\n"+*str);
/*
	// 设置文本颜色
	CHARFORMAT cf;
	edit.GetSelectionCharFormat(cf);
	if (cf.dwEffects & CFE_AUTOCOLOR)
		cf.dwEffects -= CFE_AUTOCOLOR;
	cf.crTextColor = g_nMsgTextColor[nLogLevel];
	cf.dwMask = CFM_COLOR;	
	edit.SetSelectionCharFormat(cf);
	edit.HideSelection(TRUE, FALSE);

	// 滚动在最底行
	edit.SendMessage ( WM_VSCROLL, (WPARAM)SB_BOTTOM, (LPARAM)NULL );
*/

	delete str;
	return 0L;
}
