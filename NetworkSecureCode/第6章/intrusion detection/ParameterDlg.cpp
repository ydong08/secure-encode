//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

// ParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Protocolanalysis.h"
#include "ParameterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CParameterDlg dialog
extern bool gSaveDumpFile;
extern bool gShowPacketData;
extern bool gIntrusionDetect;
CParameterDlg::CParameterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CParameterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParameterDlg)
	m_bSaveDumpFile = FALSE;
	m_bShowPacketData = TRUE;
	m_bIntrusionDetect = TRUE;
	//}}AFX_DATA_INIT
}
void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParameterDlg)
	DDX_Check(pDX, IDC_CHECK_SAVE_DUMP_FILE, m_bSaveDumpFile);
	DDX_Check(pDX, IDC_CHECK_SHOW_PACKET_DATA, m_bShowPacketData);
	DDX_Check(pDX, IDC_CHECK_INTRUSION_DETECT, m_bIntrusionDetect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParameterDlg, CDialog)
//{{AFX_MSG_MAP(CParameterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParameterDlg message handlers

void CParameterDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	gSaveDumpFile=m_bSaveDumpFile;
	gShowPacketData=m_bShowPacketData;
	gIntrusionDetect=m_bIntrusionDetect;
	if((gSaveDumpFile==false)&&(gShowPacketData==false)&&(gIntrusionDetect==false))
	{
		MessageBox("请选择至少一项结果形式！","网络入侵检测系统《网络安全编程技术与实例》");
		return ;
	}
	CDialog::OnOK();
}



BOOL CParameterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bSaveDumpFile=gSaveDumpFile;
	m_bShowPacketData=gShowPacketData;
	m_bIntrusionDetect=gIntrusionDetect;
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
