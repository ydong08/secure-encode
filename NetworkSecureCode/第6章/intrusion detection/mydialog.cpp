//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

// mydialog.cpp : implementation file
//by liuwen tao

#include "stdafx.h"
#include "Protocolanalysis.h"
#include "mydialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// mydialog dialog


mydialog::mydialog(CWnd* pParent /*=NULL*/) : CDialog(mydialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(mydialog)
	//}}AFX_DATA_INIT

}

void mydialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(mydialog)
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(mydialog, CDialog)
	//{{AFX_MSG_MAP(mydialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// mydialog message handlers

void mydialog::OnOK()
{
	CDialog::OnOK();
}

BOOL mydialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon;	
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CenterWindow();
	return TRUE;
}

void mydialog::OnPaint()
{
	CPaintDC dc(this);
}
