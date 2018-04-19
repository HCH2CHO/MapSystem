
// spatial analysisView.cpp : CspatialanalysisView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "spatial analysis.h"
#endif

#include "spatial analysisDoc.h"
#include "spatial analysisView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MainFrm.h"
#include <string>

// CspatialanalysisView

IMPLEMENT_DYNCREATE(CspatialanalysisView, CView)

BEGIN_MESSAGE_MAP(CspatialanalysisView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CspatialanalysisView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_OPEN, &CspatialanalysisView::OnFileOpen)
//	ON_COMMAND(ID_Douglas, &CspatialanalysisView::OnDouglas)
ON_COMMAND(ID_Douglas, &CspatialanalysisView::OnDouglas)
ON_COMMAND(ID_ReadTin, &CspatialanalysisView::OnReadTin)
END_MESSAGE_MAP()

// CspatialanalysisView 构造/析构

CspatialanalysisView::CspatialanalysisView()
{
	// TODO: 在此处添加构造代码
	init=false;

	m_bLeftButtonDown=false;
	m_bGlide=true;
	m_YNLoaded = TRUE;
	m_fileread=FALSE;
	m_douglas=FALSE;

	m_WindowOrgX=15000;
	m_WindowOrgY=10000;
	m_scale=1;

	//zoomRect = CRect(m_WindowOrgX,m_WindowOrgY,nWidth,nHeight);
	data.push_back(vector<double>());
	data.push_back(vector<double>());//建立二维数组

	m_dResolution = 1000.;
}

CspatialanalysisView::~CspatialanalysisView()
{
}

BOOL CspatialanalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}


//点坐标缩放
POINT scalingPoint(POINT point,int sc)  
{  
    POINT newpoint;  
    newpoint.x= point.x*sc;  
    newpoint.y= point.y*sc;  
    return newpoint;  
}

// CspatialanalysisView 绘制
void CspatialanalysisView::OnDraw(CDC* pDC)
{
	CspatialanalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	// TODO: 在此处为本机数据添加绘制代码
	//pDC->MoveTo(100,100);
	//pDC->LineTo(600,600);

	/*
	pts[0].x = 100; pts[0].y = 100;
	pts[1].x = 300; pts[1].y = 300;
	pts[2].x = 100; pts[2].y = 300;

	pDC->Polygon(pts,3);
	*/

	CPoint p1;
	CPoint p2;
	if(m_fileread)
	{
		for (int i=0;i<data[0].size()-1;i++)
		{
			p1=CPoint(data[0][i],data[1][i]);
			p2=CPoint(data[0][i+1],data[1][i+1]);
			pDC->MoveTo(p1);
			pDC->LineTo(p2);
		}
	}

	if(m_douglas)
	{
		Douglas(pDC);
	}
	m_tin.DrawTin(pDC);
}

//筛选点
void CspatialanalysisView::Selectpoint(int start,int end,double thre)
{
	//寻找距离最大点
	double dis;
	double maxdis=0;//两点间最大距离
	int pointNumber;
	for(int i=start;i<end;i++)
	{
		dis=Distance(&douglas[start],&douglas[end],&douglas[i]);
		if(dis>maxdis)
		{
			maxdis=dis;
			pointNumber=i;
		}
	}
	if (maxdis>thre)
	{
		douglas[pointNumber].record=TRUE;
		Selectpoint(start,pointNumber,thre);
		Selectpoint(pointNumber,end,thre);
	}
}

//Douglas-Peucker算法
void CspatialanalysisView::Douglas(CDC* pDC)
{
	double threshold=50;//设定阈值

	const int num=data[0].size();
	douglas=new coord[num];
	for (int i=0;i<data[0].size();i++)
	{
		douglas[i].x=data[0][i];
		douglas[i].y=data[1][i];
		douglas[i].record=false;
	}

	int mid=num/2;
	Selectpoint(0,mid-1,threshold);
	Selectpoint(mid-1,num-1,threshold);

	douglas[mid-1].record=TRUE;

	vector<vector<double>> newdata(2);
	for(int i=0;i<num;i++)
	{
		if (douglas[i].record)
		{
			newdata[0].push_back(douglas[i].x);
			newdata[1].push_back(douglas[i].y);
		}
	}
	newdata[0].push_back(newdata[0][0]);
	newdata[1].push_back(newdata[1][0]);

	CPen pen;
	pen.CreatePen(PS_SOLID,0,RGB(255,0,0));
	pDC->SelectObject(&pen);

	CPoint p1,p2;
	for (int i=0;i<newdata[0].size()-1;i++)
	{
		p1=CPoint(newdata[0][i],newdata[1][i]);
		p2=CPoint(newdata[0][i+1],newdata[1][i+1]);
		pDC->MoveTo(p1);
		pDC->LineTo(p2);
	}
}

//计算两点间距离
double CspatialanalysisView::Distance(coord *p1,coord *p2,coord *target)
{
	double x0=target->x;
	double y0=target->y;
	double A,B,C;
	A=p2->y-p1->y;
	B=p1->x-p2->x;
	C=p2->x*p1->y-p1->x*p2->y;
	double d;
	d=fabs((A*x0+B*y0+C)/sqrt(A*A+B*B));
	return d;
}


// CspatialanalysisView 打印
void CspatialanalysisView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CspatialanalysisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CspatialanalysisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CspatialanalysisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CspatialanalysisView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CspatialanalysisView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CspatialanalysisView 诊断

#ifdef _DEBUG
void CspatialanalysisView::AssertValid() const
{
	CView::AssertValid();
}

void CspatialanalysisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CspatialanalysisDoc* CspatialanalysisView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CspatialanalysisDoc)));
	return (CspatialanalysisDoc*)m_pDocument;
}
#endif //_DEBUG


// CspatialanalysisView 消息处理程序


void CspatialanalysisView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
	CSize size;
	CPoint pt;
	CRect rc;
	GetClientRect(&rc);
	pDC->SetMapMode(MM_ANISOTROPIC);
	//pDC->SetViewportOrg(rc.left,rc.bottom);  //调整视口原点
	//pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetViewportOrg(rc.CenterPoint());
	pDC->SetViewportExt(rc.Width(),-rc.Height());//调整宽高

	if (init==false) 
	{
		zoomRect = CRect(m_WindowOrgX,m_WindowOrgY,m_WindowOrgX+m_scale*rc.Width(),m_WindowOrgY+m_scale*rc.Height());//初始化，保持窗口和视口比例一致
		init=true;
	}


	size=zoomRect.Size();
	pt=zoomRect.CenterPoint();
	pDC->SetWindowOrg(pt);
	pDC->SetWindowExt(size);

	//pDC->SetWindowOrg(m_WindowOrgX,m_WindowOrgY);
	//pDC->SetWindowExt(nWidth,nHeight);

	CView::OnPrepareDC(pDC, pInfo);
}


BOOL CspatialanalysisView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_YNLoaded)
	{	
		if(zDelta>0)
		{
			m_scale=1+(zDelta*1.0/480);
		}
		else
		{
			m_scale=1+zDelta*1.0/480;
		}

			//nHeight*=m_scale;
			//nWidth*=m_scale;//进行缩放

			CPoint zoompoint=pt;//注意此处为pt，屏幕（父窗口）坐标
			ScreenToClient(&zoompoint); //pt为屏幕（父窗口）坐标，转换为客户区坐标
			CClientDC dc(this);
			OnPrepareDC(&dc);
			dc.DPtoLP(&zoompoint);//设备坐标转换成逻辑坐标

			CPoint oldpoint(zoomRect.left,zoomRect.top);
			CPoint newpoint(zoompoint.x+(oldpoint.x-zoompoint.x)*m_scale,zoompoint.y+(oldpoint.y-zoompoint.y)*m_scale);
			m_WindowOrgX=newpoint.x;
			m_WindowOrgY=newpoint.y;//将鼠标放置点作为窗口中心点
			zoomRect=CRect(m_WindowOrgX,m_WindowOrgY,m_WindowOrgX+m_scale*zoomRect.Width(),m_WindowOrgY+m_scale*zoomRect.Height());

	}
	Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CspatialanalysisView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_YNLoaded)
	{
		if(m_bGlide)
		{
			m_Pointdown=point;
		}
		m_bLeftButtonDown=true;
	}	

	CView::OnLButtonDown(nFlags, point);
}


void CspatialanalysisView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_YNLoaded)
	{
		if(m_bGlide)
		{
			m_Pointup=point;
		
			CDC *MyCDC=this->GetDC();
			this->OnPrepareDC(MyCDC,NULL);
			MyCDC->DPtoLP(&m_Pointup); //设备坐标转换成逻辑坐标
			MyCDC->DPtoLP(&m_Pointdown);

			/*CString str;
			str.Format("%d", m_Pointup.x);
			AfxMessageBox(str);*/
				
		    m_WindowOrgX-=m_Pointup.x-m_Pointdown.x;
		    m_WindowOrgY-=m_Pointup.y-m_Pointdown.y;//移动窗口原点
			
			zoomRect=CRect(m_WindowOrgX,m_WindowOrgY,m_WindowOrgX+zoomRect.Width(),m_WindowOrgY+zoomRect.Height());

			Invalidate();
			
		}
		m_bLeftButtonDown=false;

	CView::OnLButtonUp(nFlags, point);
	}
}


void CspatialanalysisView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame *frame = (CMainFrame*) (AfxGetApp()->m_pMainWnd);   //用于显示点的坐标
	CString info;
	CPoint pt=point;

	//设备坐标转换为逻辑坐标
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&pt);
	info.Format(_T("LP坐标:[x=%d,y=%d]"), pt.x,pt.y);

    frame->SetPaneText(0,info);

	CView::OnMouseMove(nFlags, point);
}


void CspatialanalysisView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码

	CFileDialog dialog(TRUE,_T("poy"),_T("*.poy"),OFN_HIDEREADONLY,_T("map files(*.poy)|*.poy|任何文件(*.*)|*.*"));

	if (dialog.DoModal()!=IDOK)
	{
		MessageBox(_T("读取失败"));
		return ;
	}
	CString FileName=dialog.GetPathName();
	//FILE*fp=fopen(CT2A(FileName),"r");
	ifstream is(FileName);
	if(is.bad())
	{
		MessageBox(_T("打开失败"));
		return;
	}
	
	string rpointX,rpointY;
	//vector<vector<double>> data(2);
	is.seekg(0,ios::beg);
	while(!is.eof())
	{	
		is>>rpointX>>rpointY;
		data[0].push_back(100*(atof(rpointX.c_str())-9000));
		data[1].push_back(100*atof(rpointY.c_str()));
	}
	m_fileread=TRUE;
	Invalidate();
}

void CspatialanalysisView::GetCoorArray(DOUBLEPOINTARRAY& doublePointArray,POINTARRAY& pointArray,BOOL bFlag/* = TRUE*/)
{
	int i;
	int nPoint;
	DOUBLEPOINT p0;
	p0.x = (m_rcRealMap.left+m_rcRealMap.right)/2.;
	p0.y = (m_rcRealMap.top+m_rcRealMap.bottom)/2.;
	if(bFlag)
	{
		pointArray.RemoveAll();
		nPoint = doublePointArray.GetSize();
		if(nPoint==0)
			return;
		pointArray.SetSize(nPoint);
		
		for (i=0;i<nPoint;i++)
		{
			POINT p;
			p.x = (doublePointArray[i].x-p0.x)*m_dResolution;
			p.y = (doublePointArray[i].y-p0.y)*m_dResolution;
			pointArray.SetAt(i,p);
		}

		m_WindowOrgX=-(-m_rcRealMap.left+m_rcRealMap.right)*500;
		m_WindowOrgY=-(-m_rcRealMap.top+m_rcRealMap.bottom)*500;
		zoomRect=CRect(m_WindowOrgX,m_WindowOrgY,m_WindowOrgX+(-m_rcRealMap.left+m_rcRealMap.right)*1000,m_WindowOrgY+(-m_rcRealMap.top+m_rcRealMap.bottom)*1000);

		return;
	}

	doublePointArray.RemoveAll();
	nPoint = pointArray.GetSize();
	if(nPoint==0)
		return;
	doublePointArray.SetSize(nPoint);
	
	for (i=0;i<nPoint;i++)
	{
		DOUBLEPOINT p;
		p.x = pointArray[i].x/m_dResolution+p0.x;
		p.y = pointArray[i].y/m_dResolution+p0.y;
		doublePointArray.SetAt(i,p);
	}

}

void CspatialanalysisView::OnDouglas()
{
	// TODO: 在此添加命令处理程序代码
	if(m_douglas==FALSE)
	{
		m_douglas=TRUE;
	}
	else
	{
		m_douglas=FALSE;
	}
	Invalidate();
}


void CspatialanalysisView::OnReadTin()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR tFileName[_MAX_PATH];	//修改char为TCHAR	
	char FileName[_MAX_PATH];

	CString strFilter2 = _T("TIN文件(*.ctin)|*.ctin||");
	CFileDialog FileOpenDlg(TRUE,_T("ctin"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter2,NULL);
	FileOpenDlg.m_ofn.lpstrTitle = _T("打开文件");	
	if(FileOpenDlg.DoModal()==IDCANCEL)
		return ;
	lstrcpy(tFileName,FileOpenDlg.GetPathName());
	
	int iLength = WideCharToMultiByte(CP_ACP, 0, tFileName, -1, NULL, 0, NULL, NULL);  
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tFileName, -1, FileName, iLength, NULL, NULL);   

	m_tin.SetView(this);
	m_tin.Read(FileName);	
	m_tin.Voronoi();
	//m_zoomRect.RemoveAll();//打开数据，初始化缩放信息
	Invalidate();//刷新图形
}
