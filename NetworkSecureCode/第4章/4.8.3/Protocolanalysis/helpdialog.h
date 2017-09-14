//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
#if !defined(AFX_MYDIALOG_H__FE67E2FF_56CF_463A_B83D_192DD7C40283__INCLUDED_)
#define AFX_MYDIALOG_H__FE67E2FF_56CF_463A_B83D_192DD7C40283__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// helpdialog dialog

class helpdialog : public CDialog
{
	// Construction
public:
	helpdialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(helpdialog) 
	enum { IDD = IDD_DIALOG_F1 };
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(helpdialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(helpdialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIALOG_H__FE67E2FF_56CF_463A_B83D_192DD7C40283__INCLUDED_)
