//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//
// DeviceDialog.cpp : implementation file
//


#include "stdafx.h"
#include "Protocolanalysis.h"
#include "DeviceDialog.h"
#include "Protocolanalysisdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND hWnd;
extern int network_device;
extern char  network_device_name[1024][1024];
extern int network_device_number;

/////////////////////////////////////////////////////////////////////////////
// CDeviceDialog dialog
//by liuwen tao 

CDeviceDialog::CDeviceDialog(CWnd* pParent /*=NULL*/)
: CDialog(CDeviceDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceDialog)
	//}}AFX_DATA_INIT
}

void CDeviceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceDialog)
	DDX_Control(pDX, IDC_LIST_DEVICE, m_list_device);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDeviceDialog, CDialog)
//{{AFX_MSG_MAP(CDeviceDialog)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDeviceDialog message handlers
BOOL CDeviceDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	for(int i=0;i<network_device_number;i++)
	{
		m_list_device.InsertString (0,network_device_name[i]);
	}
	return TRUE;  
}
void CDeviceDialog::OnOK() 
{
	UpdateData(TRUE);
	// TODO: Add extra validation here
	network_device=m_list_device.GetCurSel   ();
	if(network_device==-1)
	{
		MessageBox("请选择网络接口","网络入侵检测系统《网络安全编程技术与实例》（作者：刘文涛）");
		return ;
	}
	network_device++;
	CProtocolAnalysisDlg *p1=(CProtocolAnalysisDlg *)GetParent();
	CButton *p=(CButton*)p1->GetDlgItem (IDC_BUTTON_START);
	p->EnableWindow (FALSE);
	CMenu *pp=(CMenu *)p1->GetMenu();
	pp->EnableMenuItem (MENU_START,TRUE);
	CButton *p2=(CButton*)p1->GetDlgItem (IDC_BUTTON_END);
	p2->EnableWindow (TRUE);
	CMenu *pp2=(CMenu *)p1->GetMenu();
	pp2->EnableMenuItem (MENU_STOP,FALSE);
	CDialog::OnOK();
}
