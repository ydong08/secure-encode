//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------// 
#include "stdafx.h"
#include "Protocolanalysis.h"
#include "ProtocolanalysisDlg.h"
#include "helpdialog.h"
#include "filterdlg.h"
#include "sniffer.h"
#include "ipprotocol.h"
#include "arpprotocol.h"
#include "tcpprotocol.h"
#include "udpprotocol.h"
#include "icmpprotocol.h"
#include "ethernetprotocol.h"
#include "parameterdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
char PacketFilter[1024];
extern struct PacketInformation packet;
extern struct EtherHeader DisplayEthernet;
extern struct arpheader DisplayARP;
extern struct IpHeader DisplayIP;
extern struct TcpHeader DisplayTCP;
extern struct UdpHeader DisplayUDP;
extern struct IcmpHeader DisplayIcmp;
extern CString lwtProgramName;
extern HANDLE ProgramValue;
extern char lwtGlobal_FilePath[_MAX_PATH];
bool StopThread = TRUE;
CWinThread* pThread;
HWND hWnd;
HANDLE hand;
struct ProtocolNumber PacketNumber;
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
// CProtocolAnalysisDlg dialog
CProtocolAnalysisDlg::CProtocolAnalysisDlg(CWnd* pParent /*=NULL*/) : CDialog(CProtocolAnalysisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProtocolAnalysisDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CProtocolAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProtocolAnalysisDlg)
	DDX_Control(pDX, IDC_LIST_PPPOE, m_list_ethernet);
	DDX_Control(pDX, IDC_LIST_ICMP, m_list_icmp);
	DDX_Control(pDX, IDC_LIST_UDP, m_list_udp);
	DDX_Control(pDX, IDC_LIST_TCP, m_list_tcp);
	DDX_Control(pDX, IDC_LIST_IP, m_list_ip);
	DDX_Control(pDX, IDC_LIST3, m_list_arp);
	DDX_Control(pDX, IDC_TAB1, m_tab1);
	DDX_Control(pDX, IDC_LIST2, m_list_common);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CProtocolAnalysisDlg, CDialog)
//{{AFX_MSG_MAP(CProtocolAnalysisDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
ON_BN_CLICKED(IDC_BUTTON_END, OnButtonStop)
ON_MESSAGE(WM_MY_MESSAGE_COMMON, OnPacket) 
ON_MESSAGE(WM_MY_MESSAGE_ARP, OnArp) 
ON_MESSAGE(WM_MY_MESSAGE_IP, OnIp) 
ON_MESSAGE(WM_MY_MESSAGE_TCP, OnTcp) 
ON_MESSAGE(WM_MY_MESSAGE_UDP, OnUdp) 
ON_MESSAGE(WM_MY_MESSAGE_ICMP, OnIcmp) 
ON_MESSAGE(WM_MY_MESSAGE_ETHERNET, OnEthernet) 
ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
ON_COMMAND(MENU_START, OnStart)
ON_COMMAND(MENU_STOP, OnStop)
ON_COMMAND(MENU_EXIT, OnExit)
ON_COMMAND(ID_T_START, OnTStart)
ON_COMMAND(ID_T_STOP, OnTStop)
ON_COMMAND(ID_T_ABOUT, OnTAbout)
ON_COMMAND(MENU_HELP, OnHelp)
ON_COMMAND(ID_T_EXIT, OnTExit)
ON_COMMAND(ID_TUO_ABOUT, OnTuoAbout)
ON_COMMAND(ID_TUO_EXIT, OnTuoExit)
ON_COMMAND(MENU_SET_FILETER, OnSetFileter)
ON_COMMAND(ID_T_HIDE, OnTHide)
ON_BN_CLICKED(IDC_BUTTON_HIDE, OnButtonHide)
ON_BN_CLICKED(IDC_CENTER, OnCenter)
ON_WM_DRAWITEM()
ON_WM_MEASUREITEM()
ON_WM_RBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
ON_WM_SIZE()
ON_NOTIFY(NM_CLICK, IDC_TAB1, OnClickTab1)
ON_COMMAND(ID_MENU_ARP, OnMenuArp)
ON_COMMAND(ID_MENU_IP, OnMenuIp)
ON_COMMAND(ID_MENU_TCP, OnMenuTcp)
ON_COMMAND(ID_MENU_UDP, OnMenuUdp)
ON_COMMAND(ID_MENU_ICMP, OnMenuIcmp)
ON_COMMAND(ID_MENU_PACKET, OnMenuPacket)
ON_COMMAND(ID_MENU_OPEN_DUMP_FILE, OnMenuOpenDumpFile)
ON_COMMAND(ID_MENU_Ethernet, OnMenuEthernet)
ON_COMMAND(IDD_menu_Help, OnMenuhelp)

ON_BN_CLICKED(IDC_BUTTON_OPEN_DUMP_FILE, OnButtonOpenDumpFile)
ON_BN_CLICKED(IDC_BUTTON_SET_FILTER, OnButtonSetFilter)
ON_BN_CLICKED(IDC_BUTTON_PARAMETER, OnButtonParameter)
ON_WM_NCPAINT()
ON_COMMAND(ID_MENU_SET_PARAMETER, OnMenuSetParameter)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
// CProtocolAnalysisDlg message handlers
BOOL CProtocolAnalysisDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();
	AfxGetMainWnd()->CenterWindow(); 
	SetProp(m_hWnd, lwtProgramName, ProgramValue);
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	UpdateData(TRUE);
	UpdateData(FALSE);
	m_list_common.InsertColumn(0, "数目", LVCFMT_LEFT, 300, 1);
	m_list_common.InsertColumn(1, "源MAC", LVCFMT_LEFT, 300, 1);
	m_list_common.InsertColumn(2, "目的MAC", LVCFMT_LEFT, 300, 1);
	m_list_common.InsertColumn(3, "协议类型", LVCFMT_LEFT, 300, 1);
	m_list_common.InsertColumn(4, "源IP", LVCFMT_LEFT, 300, 1);
	m_list_common.InsertColumn(5, "源端口", LVCFMT_LEFT, 300, 1);
	m_list_common.InsertColumn(6, "目的IP", LVCFMT_LEFT, 300, 1);
	m_list_common.InsertColumn(7, "目的端口", LVCFMT_LEFT, 300, 1);
	m_list_common.SetColumnWidth(0, 40);
	m_list_common.SetColumnWidth(1, 120);
	m_list_common.SetColumnWidth(2, 120);
	m_list_common.SetColumnWidth(3, 60);
	m_list_common.SetColumnWidth(4, 110);
	m_list_common.SetColumnWidth(5, 50);
	m_list_common.SetColumnWidth(6, 110);
	m_list_common.SetColumnWidth(7, 60);
	ListView_SetExtendedListViewStyle(m_list_common.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);
	m_list_arp.InsertColumn(0, "数目", LVCFMT_LEFT, 300, 1);
	m_list_arp.InsertColumn(1, "硬件地址类型", LVCFMT_LEFT, 300, 1);
	m_list_arp.InsertColumn(2, "协议地址类型", LVCFMT_LEFT, 300, 1);
	m_list_arp.InsertColumn(3, "硬件地址长度", LVCFMT_LEFT, 300, 1);
	m_list_arp.InsertColumn(4, "协议地址长度", LVCFMT_LEFT, 300, 1);
	m_list_arp.InsertColumn(5, "ARP操作", LVCFMT_LEFT, 300, 1);
	m_list_arp.InsertColumn(6, "备注", LVCFMT_LEFT, 300, 1);
	m_list_arp.SetColumnWidth(0, 40);
	m_list_arp.SetColumnWidth(1, 120);
	m_list_arp.SetColumnWidth(2, 120);
	m_list_arp.SetColumnWidth(3, 120);
	m_list_arp.SetColumnWidth(4, 120);
	m_list_arp.SetColumnWidth(5, 60);
	m_list_arp.SetColumnWidth(6, 110);
	
	ListView_SetExtendedListViewStyle(m_list_arp.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);
	m_list_ip.InsertColumn(0, "数目", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(1, "协议版本", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(2, "首部长度", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(3, "服务质量", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(4, "IP长度", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(5, "ID号", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(6, "偏移量", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(7, "生存周期", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(8, "协议类型", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(9, "校验和", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(10, "源IP地址", LVCFMT_LEFT, 300, 1);
	m_list_ip.InsertColumn(11, "目的IP地址", LVCFMT_LEFT, 300, 1);
	m_list_ip.SetColumnWidth(0, 40);
	m_list_ip.SetColumnWidth(1, 120);
	m_list_ip.SetColumnWidth(2, 120);
	m_list_ip.SetColumnWidth(3, 120);
	m_list_ip.SetColumnWidth(4, 120);
	m_list_ip.SetColumnWidth(5, 60);
	m_list_ip.SetColumnWidth(6, 110);
	m_list_ip.SetColumnWidth(7, 120);
	m_list_ip.SetColumnWidth(8, 120);
	m_list_ip.SetColumnWidth(9, 120);
	m_list_ip.SetColumnWidth(10, 120);
	m_list_ip.SetColumnWidth(11, 60);
	ListView_SetExtendedListViewStyle(m_list_ip.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);
	m_list_tcp.InsertColumn(0, "数目", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(1, "源端口", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(2, "目的端口", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(3, "序列号", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(4, "确认号", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(5, "保留", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(6, "偏移量", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(7, "标志", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(8, "窗口", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(9, "校验和", LVCFMT_LEFT, 300, 1);
	m_list_tcp.InsertColumn(10, "紧急指针", LVCFMT_LEFT, 300, 1);
	m_list_tcp.SetColumnWidth(0, 40);
	m_list_tcp.SetColumnWidth(1, 120);
	m_list_tcp.SetColumnWidth(2, 120);
	m_list_tcp.SetColumnWidth(3, 120);
	m_list_tcp.SetColumnWidth(4, 120);
	m_list_tcp.SetColumnWidth(5, 60);
	m_list_tcp.SetColumnWidth(6, 110);
	m_list_tcp.SetColumnWidth(7, 120);
	m_list_tcp.SetColumnWidth(8, 60);
	m_list_tcp.SetColumnWidth(9, 110);
	m_list_tcp.SetColumnWidth(10, 110);
	ListView_SetExtendedListViewStyle(m_list_tcp.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);
	m_list_udp.InsertColumn(0, "数目", LVCFMT_LEFT, 300, 1);
	m_list_udp.InsertColumn(1, "源端口", LVCFMT_LEFT, 300, 1);
	m_list_udp.InsertColumn(2, "目的端口", LVCFMT_LEFT, 300, 1);
	m_list_udp.InsertColumn(3, "长度", LVCFMT_LEFT, 300, 1);
	m_list_udp.InsertColumn(4, "校验和", LVCFMT_LEFT, 300, 1);
	m_list_udp.SetColumnWidth(0, 40);
	m_list_udp.SetColumnWidth(1, 120);
	m_list_udp.SetColumnWidth(2, 120);
	m_list_udp.SetColumnWidth(3, 120);
	m_list_udp.SetColumnWidth(4, 120);
	ListView_SetExtendedListViewStyle(m_list_udp.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);
	m_list_icmp.InsertColumn(0, "数目", LVCFMT_LEFT, 300, 1);
	m_list_icmp.InsertColumn(1, "类型", LVCFMT_LEFT, 300, 1);
	m_list_icmp.InsertColumn(2, "代码", LVCFMT_LEFT, 300, 1);
	m_list_icmp.InsertColumn(3, "校验和", LVCFMT_LEFT, 300, 1);
	m_list_icmp.InsertColumn(4, "说明", LVCFMT_LEFT, 300, 1);
	m_list_icmp.SetColumnWidth(0, 40);
	m_list_icmp.SetColumnWidth(1, 120);
	m_list_icmp.SetColumnWidth(2, 120);
	m_list_icmp.SetColumnWidth(3, 120);
	m_list_icmp.SetColumnWidth(4, 120);
	ListView_SetExtendedListViewStyle(m_list_icmp.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);
	m_list_ethernet.InsertColumn(0, "序号", LVCFMT_LEFT, 300, 1);
	m_list_ethernet.InsertColumn(1, "源MAC地址", LVCFMT_LEFT, 300, 1);
	m_list_ethernet.InsertColumn(2, "目的MAC地址", LVCFMT_LEFT, 300, 1);
	m_list_ethernet.InsertColumn(3, "协议类型", LVCFMT_LEFT, 300, 1);
	m_list_ethernet.SetColumnWidth(0, 40);
	m_list_ethernet.SetColumnWidth(1, 120);
	m_list_ethernet.SetColumnWidth(2, 120);
	m_list_ethernet.SetColumnWidth(3, 120);
	m_list_ethernet.SetColumnWidth(4, 120);
	ListView_SetExtendedListViewStyle(m_list_ethernet.m_hWnd,
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_FLATSB |
		LVS_EX_HEADERDRAGDROP);
	m_list_common.SetBkColor (RGB(255,255,196));
	m_list_arp.SetBkColor (RGB(192,243,204));
	m_list_ip.SetBkColor (RGB(189,213,247));
	m_list_tcp.SetBkColor (RGB(128,89,38));
	m_list_udp.SetBkColor (RGB(177,151,225));
	m_list_icmp.SetBkColor (RGB(148,228,190));
	m_list_common.SetTextBkColor  (RGB(255,255,196));
	m_list_arp.SetTextBkColor (RGB(192,243,204));
	m_list_ip.SetTextBkColor (RGB(189,213,247));
	m_list_tcp.SetTextBkColor (RGB(128,89,38));
	m_list_udp.SetTextBkColor (RGB(177,151,225));
	m_list_icmp.SetTextBkColor (RGB(148,228,190));
	m_list_common.SetTextColor  (RGB(0,0,255));
	m_list_arp.SetTextColor (RGB(192,0,204));
	m_list_ip.SetTextColor (RGB(150,0,0));
	m_list_tcp.SetTextColor (RGB(255,242,69));
	m_list_udp.SetTextColor (RGB(0,0,0));
	m_list_icmp.SetTextColor (RGB(28,43,227));
	CTabCtrl* pTab = (CTabCtrl*) GetDlgItem(IDC_TAB1);
	CRect rectWnd;
	pTab->GetWindowRect(rectWnd);
	m_tab1.InsertItem(0, "网络信息  ", 0);	
	m_tab1.InsertItem(1, "ARP协议信息  ", 1);
	m_tab1.InsertItem(2, "IP协议信息  ", 2);	
	m_tab1.InsertItem(3, "TCP协议信息  ", 3);
	m_tab1.InsertItem(4, "UDP协议信息  ", 4);	
	m_tab1.InsertItem(5, "ICMP协议信息  ", 5);
	m_tab1.InsertItem(6, "以太网协议信息", 6);
	CRect rect;
	GetClientRect(&rect);
	CRect newrect(rect);
	newrect.top =rect.top +30;
	m_tab1.MoveWindow (newrect);
	CRect rect1, rect2;
	m_tab1.GetWindowRect(rect1); 
	m_tab1.GetItemRect(0, rect2); 
	ScreenToClient(rect1);
	rect1.left += 2;
	rect1.top += rect2.Height() + 3;
	m_list_common.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_common.ShowWindow(SW_SHOW);	 
	m_list_arp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_arp.ShowWindow(SW_HIDE);	 
	m_list_ip.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_ip.ShowWindow(SW_HIDE);	 
	m_list_tcp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_tcp.ShowWindow(SW_HIDE);
	m_list_udp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_udp.ShowWindow(SW_HIDE);	 
	m_list_icmp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	
	m_list_icmp.ShowWindow(SW_HIDE);
	m_list_ethernet.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_ethernet.ShowWindow(SW_HIDE);
	hWnd = GetSafeHwnd();
	SetWindowText("网络协议分析系统《网络安全编程技术与实例》（作者：刘文涛）"); 
	CButton *p=(CButton*)GetDlgItem (IDC_BUTTON_END);
	p->EnableWindow (FALSE);
	CMenu *pp=(CMenu *)GetMenu();
	pp->EnableMenuItem (MENU_STOP,TRUE);
	ShowWindow(SW_MAXIMIZE); 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CProtocolAnalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		int result =MessageBox("\n\n你真的要退出吗？","网络协议分析系统《网络安全编程技术与实例》（作者：刘文涛）",MB_OKCANCEL);
		if(result==IDOK)
			PostQuitMessage(1);	
	}
	else if ((nID & 0xFFF0) == SC_MAXIMIZE)
	{
		ShowWindow(SW_MAXIMIZE);
		CRect rect1, rect2;
		m_tab1.GetWindowRect(rect1); 
		m_tab1.GetItemRect(0, rect2); 
		ScreenToClient(rect1);
		rect1.left += 2;
		rect1.top += rect2.Height() + 3;
		m_list_common.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_arp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_ip.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_tcp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_udp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_icmp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		
		m_list_ethernet.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
void CProtocolAnalysisDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
HCURSOR CProtocolAnalysisDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


UINT ThreadPacketCapture(LPVOID pParam)
{
	PacketOperation();
	return 0;
}


UINT ThreadReadFile(LPVOID pParam)
{
	DumpFileOperation();
	return 0;
}

LRESULT CProtocolAnalysisDlg::OnPacket(WPARAM wParam, LPARAM lParam)
{
	static i = 1;  
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_common.InsertItem(0, "");
	m_list_common.SetItemText(0, 0, str);
	m_list_common.SetItemText(0, 1, packet.SourceMac);
	m_list_common.SetItemText(0, 2, packet.DestinationMac);
	m_list_common.SetItemText(0, 3, packet.NetType);
	m_list_common.SetItemText(0, 4, packet.SourceAddr);
	m_list_common.SetItemText(0, 5, packet.SourcePort);
	m_list_common.SetItemText(0, 6, packet.DestinationAddr);
	m_list_common.SetItemText(0, 7, packet.DestinationPort);
	UpdateData(TRUE);
	UpdateData(FALSE);
	PacketNumber.count = i;
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_PACKET_COUNT);
	CString strnum;
	strnum.Format("数据包个数:%d",PacketNumber.count);
	p->SetWindowText (strnum);
	i++;  
	return  0;
}

LRESULT CProtocolAnalysisDlg::OnArp(WPARAM wParam, LPARAM lParam)
{
	static i = 1;  
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_arp.InsertItem(0, "");
	m_list_arp.SetItemText(0, 0, str);
	m_list_arp.SetItemText(0, 1, DisplayARP.Hardware);
	m_list_arp.SetItemText(0, 2, DisplayARP.Protocol);
	m_list_arp.SetItemText(0, 3, DisplayARP.HardwareLength);
	m_list_arp.SetItemText(0, 4, DisplayARP.ProtocolLength);
	m_list_arp.SetItemText(0, 5, DisplayARP.OperationCode);
	m_list_arp.SetItemText(0, 6, DisplayARP.OperationInformatin);
	PacketNumber.arp = i;	
	i++;  
	return  0;
}

LRESULT CProtocolAnalysisDlg::OnIp(WPARAM wParam, LPARAM lParam)
{
	static i = 1; 
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_ip.InsertItem(0, "");
	m_list_ip.SetItemText(0, 0, str);
	m_list_ip.SetItemText(0, 1, DisplayIP.Version);
	m_list_ip.SetItemText(0, 2, DisplayIP.HeaderLength);
	m_list_ip.SetItemText(0, 3, DisplayIP.Tos);
	m_list_ip.SetItemText(0, 4, DisplayIP.Length);
	m_list_ip.SetItemText(0, 5, DisplayIP.Ident);
	m_list_ip.SetItemText(0, 6, DisplayIP.Flags_Offset);
	m_list_ip.SetItemText(0, 7, DisplayIP.TTL);
	m_list_ip.SetItemText(0, 8, DisplayIP.Protocol);
	m_list_ip.SetItemText(0, 9, DisplayIP.Checksum);
	m_list_ip.SetItemText(0, 10, DisplayIP.SourceAddr);
	m_list_ip.SetItemText(0, 11, DisplayIP.DestinationAddr);
	PacketNumber.ip = i;
	i++;  
	return  0;
}
LRESULT CProtocolAnalysisDlg::OnTcp(WPARAM wParam, LPARAM lParam)
{
	static i = 1;  
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_tcp.InsertItem(0, "");
	m_list_tcp.SetItemText(0, 0, str);
	m_list_tcp.SetItemText(0, 1, DisplayTCP.SrcPort);
	m_list_tcp.SetItemText(0, 2, DisplayTCP.DstPort);
	m_list_tcp.SetItemText(0, 3, DisplayTCP.SequenceNum);
	m_list_tcp.SetItemText(0, 4, DisplayTCP.Acknowledgment);
	m_list_tcp.SetItemText(0, 5, DisplayTCP.Zero);
	m_list_tcp.SetItemText(0, 6, DisplayTCP.HdrLen);
	m_list_tcp.SetItemText(0, 7, DisplayTCP.Flags);
	m_list_tcp.SetItemText(0, 8, DisplayTCP.AdvertisedWindow);
	m_list_tcp.SetItemText(0, 9, DisplayTCP.Checksum);
	m_list_tcp.SetItemText(0, 10, DisplayTCP.UrgPtr);
	PacketNumber.tcp = i;
	i++; 
	return  0;
}
LRESULT CProtocolAnalysisDlg::OnUdp(WPARAM wParam, LPARAM lParam)
{
	static i = 1;  
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_udp.InsertItem(0, "");
	m_list_udp.SetItemText(0, 0, str);
	m_list_udp.SetItemText(0, 1, DisplayUDP.SrcPort);
	m_list_udp.SetItemText(0, 2, DisplayUDP.DstPort);
	m_list_udp.SetItemText(0, 3, DisplayUDP.Length);
	m_list_udp.SetItemText(0, 4, DisplayUDP.Checksum);
	PacketNumber.udp = i;
	i++;  
	return  0;
}
LRESULT CProtocolAnalysisDlg::OnIcmp(WPARAM wParam, LPARAM lParam)
{
	static i = 1;  
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_icmp.InsertItem(0, "");
	m_list_icmp.SetItemText(0, 0, str);
	m_list_icmp.SetItemText(0, 1, DisplayIcmp.type);
	m_list_icmp.SetItemText(0, 2, DisplayIcmp.code);
	m_list_icmp.SetItemText(0, 3, DisplayIcmp.checksum);
	m_list_icmp.SetItemText(0, 4, DisplayIcmp.information);
	PacketNumber.icmp = i;
	i++;  
	return  0;
}

LRESULT CProtocolAnalysisDlg::OnEthernet(WPARAM wParam, LPARAM lParam)
{
	static i = 1;  
	char str[10]; 
	sprintf(str, "%d", i);
	m_list_ethernet.InsertItem(0, "");
	m_list_ethernet.SetItemText(0, 0, str);
	m_list_ethernet.SetItemText(0, 1, DisplayEthernet.SourceMac);
	m_list_ethernet.SetItemText(0, 2, DisplayEthernet.DestinationMac);
	m_list_ethernet.SetItemText(0, 3, DisplayEthernet.NetType);
	PacketNumber.ethernet = i;
	i++;  
	return  0;
}

void CProtocolAnalysisDlg::OnButtonAbout()
{
	helpdialog dia;
	dia.DoModal();
}

void CProtocolAnalysisDlg::OnButtonStart()
{
	StopThread = FALSE;
	hWnd = GetSafeHwnd();
	pThread = AfxBeginThread(ThreadPacketCapture, hWnd);
	pThread->m_bAutoDelete = FALSE;
}


void CProtocolAnalysisDlg::OnButtonStop()
{
	if (StopThread == TRUE)
	{
		return;
	}
	StopThread = TRUE;
	CButton *p=(CButton*)GetDlgItem (IDC_BUTTON_END);
	p->EnableWindow (FALSE);
	CMenu *pp=(CMenu *)GetMenu();
	pp->EnableMenuItem (MENU_STOP,TRUE);
	CButton *p2=(CButton*)GetDlgItem (IDC_BUTTON_START);
	p2->EnableWindow (TRUE);
	CMenu *pp2=(CMenu *)GetMenu();
	pp2->EnableMenuItem (MENU_START,FALSE);
}


void CProtocolAnalysisDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (m_tab1.GetCurSel() == 0)
	{
		m_list_common.ShowWindow(SW_SHOW);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("数据包信息");
	}
	else if (m_tab1.GetCurSel() == 1)
	{
		m_list_arp.ShowWindow(SW_SHOW);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("ARP协议分析");
	}
	else if (m_tab1.GetCurSel() == 2)
	{
		m_list_ip.ShowWindow(SW_SHOW);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("IP协议分析");
	}
	else if (m_tab1.GetCurSel() == 3)
	{
		m_list_tcp.ShowWindow(SW_SHOW);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("TCP协议分析");
	}
	else if (m_tab1.GetCurSel() == 4)
	{
		m_list_udp.ShowWindow(SW_SHOW);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("UDP协议分析");
	}
	else if (m_tab1.GetCurSel() == 5)
	{
		m_list_icmp.ShowWindow(SW_SHOW);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("ICMP协议分析");
	}
	else if(m_tab1.GetCurSel () == 6 )
	{
		m_list_ethernet.ShowWindow(SW_SHOW);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("以太网协议分析");
	}
	else
	{
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("ICMP协议分析");
	}
	CRect rect1, rect2;
	m_tab1.GetWindowRect(rect1); 
	m_tab1.GetItemRect(0, rect2); 
	ScreenToClient(rect1);
	rect1.left += 2;
	rect1.top += rect2.Height() + 3;
	m_list_common.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_arp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_ip.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_tcp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_udp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_icmp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	*pResult = 0;
}

void CProtocolAnalysisDlg::OnStart()
{
	StopThread = FALSE;
	hWnd = GetSafeHwnd();
	pThread = AfxBeginThread(ThreadPacketCapture, hWnd);
	pThread->m_bAutoDelete = FALSE;
}

void CProtocolAnalysisDlg::OnStop()
{
	if (StopThread == TRUE)
	{
		return;
	}
	StopThread = TRUE;
	CButton *p=(CButton*)GetDlgItem (IDC_BUTTON_END);
	p->EnableWindow (FALSE);
	CMenu *pp=(CMenu *)GetMenu();
	pp->EnableMenuItem (MENU_STOP,TRUE);
	CButton *p2=(CButton*)GetDlgItem (IDC_BUTTON_START);
	p2->EnableWindow (TRUE);
	CMenu *pp2=(CMenu *)GetMenu();
	pp2->EnableMenuItem (MENU_START,FALSE);
}

void CProtocolAnalysisDlg::OnExit()
{
	int result =MessageBox(" \n\n你真的要退出吗？","网络协议分析系统《网络安全编程技术与实例》（作者：刘文涛）",MB_OKCANCEL);
	if(result==IDOK)
		PostQuitMessage(1);
}

void CProtocolAnalysisDlg::OnTStart()
{
	StopThread = FALSE;
	hWnd = GetSafeHwnd();
	pThread = AfxBeginThread(ThreadPacketCapture, hWnd);
	pThread->m_bAutoDelete = FALSE;
}

void CProtocolAnalysisDlg::OnTStop()
{
	if (StopThread == TRUE)
	{
		return;
	}
	StopThread = TRUE;
}
void CProtocolAnalysisDlg::OnTAbout()
{
	helpdialog dia;
	dia.DoModal();
}
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	HICON m_hIcon;	
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CenterWindow();
	return TRUE;
}
void CProtocolAnalysisDlg::OnHelp()
{
	CAboutDlg dlg;
	dlg.DoModal();
}
void CAboutDlg::OnOK()
{
	CDialog::OnOK();
}
void CProtocolAnalysisDlg::OnTExit()
{
	ShowWindow(SW_HIDE);
	int result =MessageBox("\n\n你真的要退出吗？","网络协议分析系统《网络安全编程技术与实例》（作者：刘文涛）",MB_OKCANCEL);
	if(result==IDOK)
		PostQuitMessage(1);
}
void CProtocolAnalysisDlg::OnTuoAbout()
{
	CAboutDlg aboutdlg;
	aboutdlg.DoModal();
}

void CProtocolAnalysisDlg::OnTuoExit()
{
	int result =MessageBox("\n\n你真的要退出吗？","网络协议分析系统《网络安全编程技术与实例》（作者：刘文涛）",MB_OKCANCEL);
	if(result==IDOK)
		PostQuitMessage(1);
}
void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnOK();
	CDialog::OnLButtonDown(nFlags, point);
}
void CProtocolAnalysisDlg::OnSetFileter()
{
	filterdlg dlg;
	int result = dlg.DoModal();
	if (result == IDOK)
	{
		strcpy(PacketFilter, dlg.m_filter);
	}
}
void CProtocolAnalysisDlg::OnTHide()
{
	ShowWindow(SW_HIDE);
}
void CProtocolAnalysisDlg::OnButtonHide()
{
	ShowWindow(SW_HIDE);
}
void CProtocolAnalysisDlg::OnCenter()
{
	int result =MessageBox("\n\n你真的要退出吗？","网络协议分析系统《网络安全编程技术与实例》（作者：刘文涛）",MB_OKCANCEL);
	if(result==IDOK)
		PostQuitMessage(1);
}
void CProtocolAnalysisDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CProtocolAnalysisDlg::OnMeasureItem(int nIDCtl,
										 LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CProtocolAnalysisDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnRButtonUp(nFlags, point);
}

LRESULT CProtocolAnalysisDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	LRESULT lrst = CDialog::DefWindowProc(message, wParam, lParam);
	if (!::IsWindow(m_hWnd))
		return lrst;
	if (message == WM_MOVE ||
		message == WM_PAINT ||
		message == WM_NCPAINT ||
		message == WM_NCACTIVATE ||
		message == WM_NOTIFY)
	{
		CDC* pWinDC = GetWindowDC();
		if (pWinDC)
			DrawTitleBar(pWinDC);
		ReleaseDC(pWinDC);
	}
	return lrst;
}

void CProtocolAnalysisDlg::DrawTitleBar(CDC* pDC)
{
	if (m_hWnd)
	{
		CBitmap* pBitmap = new CBitmap;
		CBitmap* pOldBitmap;
		CDC* pDisplayMemDC = new CDC;
		pDisplayMemDC->CreateCompatibleDC(pDC);
		pBitmap->LoadBitmap(IDB_BITMAP8);
		pOldBitmap = (CBitmap *) pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(300, 5, 500, 100, pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();
		CRect rtWnd, rtTitle, rtButtons;
		GetWindowRect(&rtWnd); 
		rtTitle.left = GetSystemMetrics(SM_CXFRAME);
		rtTitle.top = GetSystemMetrics(SM_CYFRAME);
		rtTitle.right = rtWnd.right -
			rtWnd.left -
			GetSystemMetrics(SM_CXFRAME);
		rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);
		CPoint point;
		CBrush Brush(0x551A8B);
		CBrush* pOldBrush = pDC->SelectObject(&Brush);
		point.x = rtWnd.Width(); 
		point.y = GetSystemMetrics(SM_CYFRAME) + 1;
		pDC->PatBlt(0, rtWnd.Height() - point.y, point.x, point.y, PATCOPY);
		CBrush Brush2(0x00CD00);
		pOldBrush = pDC->SelectObject(&Brush2);
		pDC->PatBlt(0, rtWnd.Height() - 3, rtWnd.Width(), 1, PATCOPY);
		
		ReleaseDC(pDisplayMemDC);
		delete pDisplayMemDC;
		delete pBitmap;
	}
}
void CProtocolAnalysisDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);
}

void CProtocolAnalysisDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
}

void CProtocolAnalysisDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);
}

void CProtocolAnalysisDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	static int i=0;
	if(i==1)
	{
		CRect newrect(rect);
		newrect.top =rect.top +30;
		m_tab1.MoveWindow (newrect);
		CRect rect1, rect2;
		m_tab1.GetWindowRect(rect1); 
		m_tab1.GetItemRect(0, rect2); 
		ScreenToClient(rect1);
		rect1.left += 2;
		rect1.top += rect2.Height() + 3;
		m_list_common.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_arp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_ip.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_tcp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_udp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_icmp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
		m_list_ethernet.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
			rect1.Height() - rect2.Height() - 000, NULL); 
	}
	i++;
	if(i==2)
		i=1;
	InvalidateRect(rect);
	GetWindowRect(&rect);
	InvalidateRect(rect);	
}
void CProtocolAnalysisDlg::OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CRect rect1, rect2;
	m_tab1.GetWindowRect(rect1); 
	m_tab1.GetItemRect(0, rect2); 
	ScreenToClient(rect1);
	rect1.left += 2;
	rect1.top += rect2.Height() + 3;
	m_list_common.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_arp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL);
	m_list_ip.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_tcp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_udp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_icmp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL);
	m_list_ethernet.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	*pResult = 0;
}

void CProtocolAnalysisDlg::ShowTabWindow()
{
	if (m_tab1.GetCurSel() == 0)
	{
		m_list_common.ShowWindow(SW_SHOW);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("数据包信息");
	}
	else if (m_tab1.GetCurSel() == 1)
	{
		m_list_arp.ShowWindow(SW_SHOW);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("ARP协议分析");
	}
	else if (m_tab1.GetCurSel() == 2)
	{
		m_list_ip.ShowWindow(SW_SHOW);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("IP协议分析");
	}
	else if (m_tab1.GetCurSel() == 3)
	{
		m_list_tcp.ShowWindow(SW_SHOW);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("TCP协议分析");
	}
	else if (m_tab1.GetCurSel() == 4)
	{
		m_list_udp.ShowWindow(SW_SHOW);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("UDP协议分析");
	}
	else if (m_tab1.GetCurSel() == 5)
	{
		m_list_icmp.ShowWindow(SW_SHOW);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("ICMP协议分析");
	}
	else if(m_tab1.GetCurSel () == 6 )
	{
		m_list_ethernet.ShowWindow(SW_SHOW);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("以太网协议分析");
	}
	else
	{
		m_list_common.ShowWindow(SW_HIDE);
		m_list_arp.ShowWindow(SW_HIDE);
		m_list_ip.ShowWindow(SW_HIDE);
		m_list_tcp.ShowWindow(SW_HIDE);
		m_list_udp.ShowWindow(SW_HIDE);
		m_list_icmp.ShowWindow(SW_HIDE);
		m_list_ethernet.ShowWindow(SW_HIDE);
		CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
		p->SetWindowText ("ICMP协议分析");
	}
	CRect rect1, rect2;
	m_tab1.GetWindowRect(rect1);
	m_tab1.GetItemRect(0, rect2);
	ScreenToClient(rect1);
	rect1.left += 2;
	rect1.top += rect2.Height() + 3;
	m_list_common.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_arp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_ip.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL);
	m_list_tcp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_udp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_icmp.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL); 
	m_list_ethernet.SetWindowPos(NULL, rect1.left, rect1.top, rect1.Width(),
		rect1.Height() - rect2.Height() - 000, NULL);
}

void CProtocolAnalysisDlg::OnMenuArp() 
{
	m_tab1.SetCurSel (1);
	ShowTabWindow();
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	p->SetWindowText ("ARP协议分析");
}

void CProtocolAnalysisDlg::OnMenuIp() 
{
	m_tab1.SetCurSel (2);
	ShowTabWindow();
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	p->SetWindowText ("IP协议分析");
}

void CProtocolAnalysisDlg::OnMenuTcp() 
{
	m_tab1.SetCurSel (3);
	ShowTabWindow();
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	p->SetWindowText ("TCP协议分析");
}

void CProtocolAnalysisDlg::OnMenuUdp() 
{
	m_tab1.SetCurSel (4);
	ShowTabWindow();
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	p->SetWindowText ("UDP协议分析");
}

void CProtocolAnalysisDlg::OnMenuIcmp() 
{
	m_tab1.SetCurSel (5);
	ShowTabWindow();
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	p->SetWindowText ("ICMP协议分析");
}

void CProtocolAnalysisDlg::OnMenuEthernet() 
{
	m_tab1.SetCurSel (6);
	ShowTabWindow();
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	p->SetWindowText ("以太网协议分析");
}

void CProtocolAnalysisDlg::OnMenuPacket() 
{
	m_tab1.SetCurSel (0);
	ShowTabWindow();	
	CStatic *p=(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	p->SetWindowText ("数据包信息");
}

void CProtocolAnalysisDlg::OnMenuhelp() 
{
	helpdialog dia;
	dia.DoModal();
}
void CProtocolAnalysisDlg::OnMenuOpenDumpFile() 
{
	StopThread = FALSE;
	hWnd = GetSafeHwnd();
	pThread = AfxBeginThread(ThreadReadFile, hWnd);
	pThread->m_bAutoDelete = FALSE;
}
void CProtocolAnalysisDlg::OnOK ()
{	
}
void CProtocolAnalysisDlg::OnCancel ()
{	
}
void CProtocolAnalysisDlg::OnButtonOpenDumpFile() 
{
	StopThread = FALSE;
	hWnd = GetSafeHwnd();
	pThread = AfxBeginThread(ThreadReadFile, hWnd);
	pThread->m_bAutoDelete = FALSE;
}
void CProtocolAnalysisDlg::OnButtonSetFilter() 
{
	filterdlg dlg;
	int result = dlg.DoModal();
	if (result == IDOK)
	{
		strcpy(PacketFilter, dlg.m_filter);
	}
}
void CProtocolAnalysisDlg::OnButtonParameter() 
{
	CParameterDlg dlg;
	dlg.DoModal();
}
void CProtocolAnalysisDlg::OnMenuSetParameter() 
{
	CParameterDlg dlg;
	dlg.DoModal();	
}
