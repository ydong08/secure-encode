//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

// filterdlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "filterdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char PacketFilter[1024];
/////////////////////////////////////////////////////////////////////////////
// filterdlg dialog


filterdlg::filterdlg(CWnd* pParent /*=NULL*/) : CDialog(filterdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(filterdlg)
	m_filter = _T("");
	//}}AFX_DATA_INIT

}


void filterdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(filterdlg)
	DDX_Text(pDX, IDC_EDIT_FILTER, m_filter);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(filterdlg, CDialog)
	//{{AFX_MSG_MAP(filterdlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// filterdlg message handlers

BOOL filterdlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CenterWindow(); 
	m_filter=PacketFilter;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void filterdlg::OnOK()
{
	// TODO: Add extra validation here

	UpdateData(TRUE);

	CDialog::OnOK();
}

void filterdlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}
