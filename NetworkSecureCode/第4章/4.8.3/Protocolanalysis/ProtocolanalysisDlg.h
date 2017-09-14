//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
// ProtocolAnalysisDlg.h : header file
//by liu wen tao 

#if !defined(AFX_ProtocolAnalysisDLG_H__9F879FF2_E47F_45D0_A1F8_029396B03303__INCLUDED_)
#define AFX_ProtocolAnalysisDLG_H__9F879FF2_E47F_45D0_A1F8_029396B03303__INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000
#define WM_MY_MESSAGE_COMMON (WM_USER+100) 
#define WM_MY_MESSAGE_ARP (WM_USER+101)
#define WM_MY_MESSAGE_IP (WM_USER+102)
#define WM_MY_MESSAGE_TCP (WM_USER+103)
#define WM_MY_MESSAGE_UDP (WM_USER+104)
#define WM_MY_MESSAGE_ICMP  (WM_USER+105)
#define WM_MY_MESSAGE_ETHERNET  (WM_USER+106)

#include "stdafx.h"
#include "resource.h"
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CProtocolAnalysisDlg : public CDialog
{
	
	// Construction
public:
	void ShowTabWindow(void);
	CProtocolAnalysisDlg(CWnd* pParent = NULL);	
	void DrawTitleBar(CDC* pDC);
	// Dialog Data
	//{{AFX_DATA(CProtocolAnalysisDlg)
	enum { IDD = IDD_ProtocolAnalysis_DIALOG };
	CListCtrl	m_list_ethernet;
	CStatic	m_protocol_number;
	CListCtrl	m_list_icmp;
	CListCtrl	m_list_udp;
	CListCtrl	m_list_tcp;
	CListCtrl	m_list_ip;
	CListCtrl	m_list_arp;
	CTabCtrl	m_tab1;
	CListCtrl	m_list_common;
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProtocolAnalysisDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CProtocolAnalysisDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonAbout();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg LRESULT  OnPacket(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  OnArp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  OnIp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  OnTcp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  OnUdp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  OnIcmp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  OnEthernet(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnExit();
	afx_msg void OnTStart();
	afx_msg void OnTStop();
	afx_msg void OnTAbout();
	afx_msg void OnHelp();
	afx_msg void OnTExit();
	afx_msg void OnTuoAbout();
	afx_msg void OnTuoExit();
	afx_msg void OnSetFileter();
	afx_msg void OnTHide();
	afx_msg void OnButtonHide();
	afx_msg void OnCenter();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuArp();
	afx_msg void OnMenuIp();
	afx_msg void OnMenuTcp();
	afx_msg void OnMenuUdp();
	afx_msg void OnMenuIcmp();
	afx_msg void OnMenuPacket();
	afx_msg void OnMenuOpenDumpFile();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMenuEthernet();
	afx_msg void OnMenuhelp();
	afx_msg void OnButtonOpenDumpFile();
	afx_msg void OnButtonSetFilter();
	afx_msg void OnButtonParameter();
	afx_msg void OnMenuSetParameter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ProtocolAnalysisDLG_H__9F879FF2_E47F_45D0_A1F8_029396B03303__INCLUDED_)
