//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

#if !defined(AFX_FILTERDLG_H__8A158A85_7A74_4D82_8DC2_510685FBA62D__INCLUDED_)
#define AFX_FILTERDLG_H__8A158A85_7A74_4D82_8DC2_510685FBA62D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// filterdlg dialog

class filterdlg : public CDialog
{
	// Construction
public:
	filterdlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(filterdlg)
	enum { IDD = IDD_FILTER_DLG }; 
	CString	m_filter;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(filterdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(filterdlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERDLG_H__8A158A85_7A74_4D82_8DC2_510685FBA62D__INCLUDED_)
