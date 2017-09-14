//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------// 
// helpdialog.cpp : implementation file
//

#include "stdafx.h"
#include "Protocolanalysis.h"
#include "helpdialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// helpdialog dialog
//by liuwentao

helpdialog::helpdialog(CWnd* pParent /*=NULL*/) : CDialog(helpdialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(helpdialog)
	//}}AFX_DATA_INIT
	
}

void helpdialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(helpdialog)
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(helpdialog, CDialog)
//{{AFX_MSG_MAP(helpdialog)
ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// helpdialog message handlers

void helpdialog::OnOK()
{
	CDialog::OnOK();
}

BOOL helpdialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon;	
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CenterWindow();
	return TRUE;
}

void helpdialog::OnPaint()
{
	CPaintDC dc(this);
}
