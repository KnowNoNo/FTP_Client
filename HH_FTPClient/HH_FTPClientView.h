
// HH_FTPClientView.h : CHH_FTPClientView ��Ľӿ�
//


#pragma once


class CHH_FTPClientView : public CView
{
protected: // �������л�����
	CHH_FTPClientView();
	DECLARE_DYNCREATE(CHH_FTPClientView)

// ����
public:
	CHH_FTPClientDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CHH_FTPClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HH_FTPClientView.cpp �еĵ��԰汾
inline CHH_FTPClientDoc* CHH_FTPClientView::GetDocument() const
   { return reinterpret_cast<CHH_FTPClientDoc*>(m_pDocument); }
#endif

