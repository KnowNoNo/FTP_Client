
// HH_FTPClientView.h : CHH_FTPClientView 类的接口
//


#pragma once


class CHH_FTPClientView : public CView
{
protected: // 仅从序列化创建
	CHH_FTPClientView();
	DECLARE_DYNCREATE(CHH_FTPClientView)

// 属性
public:
	CHH_FTPClientDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHH_FTPClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HH_FTPClientView.cpp 中的调试版本
inline CHH_FTPClientDoc* CHH_FTPClientView::GetDocument() const
   { return reinterpret_cast<CHH_FTPClientDoc*>(m_pDocument); }
#endif

