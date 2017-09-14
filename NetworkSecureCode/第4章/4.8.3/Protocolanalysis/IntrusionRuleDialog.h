#if !defined(AFX_INTRUSIONRULEDIALOG_H__73E1AD62_196F_4030_8A12_7BF9696A55CE__INCLUDED_)
#define AFX_INTRUSIONRULEDIALOG_H__73E1AD62_196F_4030_8A12_7BF9696A55CE__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IntrusionRuleDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIntrusionRuleDialog dialog


class CIntrusionRuleDialog : public CDialog
{
	// Construction
public:
	CIntrusionRuleDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CIntrusionRuleDialog)
	enum { IDD = IDD_DIALOG_INTRUSION_RULE };
	CListCtrl	m_list_rule;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIntrusionRuleDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIntrusionRuleDialog)
	afx_msg void OnButtonAddRule();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDeleteRule();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTRUSIONRULEDIALOG_H__73E1AD62_196F_4030_8A12_7BF9696A55CE__INCLUDED_)
