
// spatial analysisView.h : CspatialanalysisView 类的接口
//
#include  <string>
#include  <iostream>
#include <fstream>
#include<vector>
#include <math.h>
#include "Tin.h"
#include "spatial analysisDoc.h"
#pragma once
using namespace std;
class CspatialanalysisView : public CView
{
protected: // 仅从序列化创建
	CspatialanalysisView();
	DECLARE_DYNCREATE(CspatialanalysisView)

// 特性
public:
	CspatialanalysisDoc* GetDocument() const;

	DOUBLERECT       m_rcRealMap;
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
	virtual ~CspatialanalysisView();

	void GetCoorArray(DOUBLEPOINTARRAY& doublePointArray,POINTARRAY& pointArray,BOOL bFlag = TRUE); 

	CTin m_tin;
	double           m_dResolution;//数据分辨率

	struct coord
	{
		double x;
		double y;
		bool record;
	};
	void Douglas(CDC* pDC);
	double Distance(coord *p1,coord *p2,coord *target);
	void Selectpoint(int start,int end,double thre);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

	BOOL init;
	BOOL m_YNLoaded;
	float m_scale;
	int m_WindowOrgX,m_WindowOrgY;
	bool m_bLeftButtonDown;
	bool m_bGlide;
	bool m_fileread;

	bool m_douglas;
	CPoint m_Pointup;
	CPoint m_Pointdown;

	CRect zoomRect; //缩放窗口
	CPoint pts[3];
	vector<vector<double>> data;

	coord *douglas;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFileOpen();

	afx_msg void OnDouglas();
	afx_msg void OnReadTin();
};

#ifndef _DEBUG  // spatial analysisView.cpp 中的调试版本
inline CspatialanalysisDoc* CspatialanalysisView::GetDocument() const
   { return reinterpret_cast<CspatialanalysisDoc*>(m_pDocument); }
#endif

