
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

typedef struct Center
{
	int x;
	int y;
};
typedef CArray<Center,Center>IntCenter;

typedef struct _doublerect
{
	double left;
	double top;
	double right;
	double bottom;
}DOUBLERECT;

typedef struct _doublepoint
{
	double x;
	double y;
}DOUBLEPOINT;

typedef CArray<DOUBLEPOINT,DOUBLEPOINT>DOUBLEPOINTARRAY;//第一个参数为类型，第二个参数为返回值，可选择类型or指针
typedef CArray<POINT,POINT>POINTARRAY;
typedef CArray<CRect,CRect>CRECTARRAY;

typedef struct z_verinfo
{
	float            fVer;//the number of dem version
	SYSTEMTIME       st;//the time when the dem is created.
	char             strMemo[1000];		
}VERINFO;

typedef struct _dct_tri
{
	LONG p1;
	LONG p2;
	LONG p3;
	BYTE b;//为便于动态维护，将此作为该三角形是否存在的标志 1--存在   0--已删除 2005.02.15
}dct_tri;

typedef struct z_doublepoint3d
{
	double x;
	double y;
	double z;
}DOUBLEPOINT3D;

typedef struct _dct_edge2
{
	LONG ptNum1;
	LONG ptNum2;
}DCT_EDGE2;

typedef CArray<DOUBLEPOINT3D,DOUBLEPOINT3D>DOUBLEPOINT3DARRAY;
typedef CArray<dct_tri,dct_tri>TRIARRAY;

typedef CArray<int,int>INTARRAY;







#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


