//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

#if !defined(AFX_PARAMETERDLG_H__973FFDA1_EA4B_4599_9D2C_CBDF4D20D97A__INCLUDED_)
#define AFX_PARAMETERDLG_H__973FFDA1_EA4B_4599_9D2C_CBDF4D20D97A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParameterDlg.h : header file
//by liu wen tao 

class CParameterDlg : public CDialog
{
// Construction
public:
	CParameterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParameterDlg)
	enum { IDD = IDD_DIALOG_PARAMETER };
	BOOL	m_bSaveDumpFile;
	BOOL	m_bShowPacketData;
	BOOL	m_bIntrusionDetect;
	//}}AFX_DATA
	//by liu wen tao(new) 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParameterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParameterDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMETERDLG_H__973FFDA1_EA4B_4599_9D2C_CBDF4D20D97A__INCLUDED_)
