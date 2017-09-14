//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
// ProtocolAnalysis.h : main header file for the ProtocolAnalysis application
//

#if !defined(AFX_ProtocolAnalysis_H__5FCF698F_E377_4CF7_A137_CDDEAA10A6B7__INCLUDED_)
#define AFX_ProtocolAnalysis_H__5FCF698F_E377_4CF7_A137_CDDEAA10A6B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__ 
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CProtocolAnalysisApp:
// See ProtocolAnalysis.cpp for the implementation of this class
//by liuwen t

struct ProtocolNumber
{
	int count;
	int ethernet;
	int arp;
	int ip;
	int tcp;
	int udp;
	int icmp;
};

class CProtocolAnalysisApp : public CWinApp
{
public:
	CProtocolAnalysisApp();


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProtocolAnalysisApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	// Implementation

	//{{AFX_MSG(CProtocolAnalysisApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
// !defined(AFX_ProtocolAnalysis_H__5FCF698F_E377_4CF7_A137_CDDEAA10A6B7__INCLUDED_)
