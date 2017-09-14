//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

#if !defined(AFX_DEVICEDIALOG_H__1EF1D5B5_178C_4F8C_917D_36AD54DB0AC0__INCLUDED_)
#define AFX_DEVICEDIALOG_H__1EF1D5B5_178C_4F8C_917D_36AD54DB0AC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceDialog.h : header file
//by liuwentao 

/////////////////////////////////////////////////////////////////////////////
// CDeviceDialog dialog

class CDeviceDialog : public CDialog
{
// Construction
public:
	CDeviceDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeviceDialog)
	enum { IDD = IDD_DIALOG_DEVICE };
	CListBox	m_list_device;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeviceDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEDIALOG_H__1EF1D5B5_178C_4F8C_917D_36AD54DB0AC0__INCLUDED_)
