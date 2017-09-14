//--------------------------------------------------------------------//
//                作者：刘文涛                                        //
//                书籍：《网络安全编程技术与实例》                    //
//--------------------------------------------------------------------//

// ProtocolAnalysis.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Protocolanalysis.h"
#include "ProtocolanalysisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJiemianApp
char Global_FilePath[_MAX_PATH];
char lwtGlobal_FileDirectory[MAX_PATH];
//by liu wentao

BEGIN_MESSAGE_MAP(CProtocolAnalysisApp, CWinApp)
//{{AFX_MSG_MAP(CProtocolAnalysisApp)
//}}AFX_MSG_MAP
ON_COMMAND(ID_HELP, CWinApp::OnHelp)END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProtocolAnalysisApp construction

CProtocolAnalysisApp::CProtocolAnalysisApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CProtocolAnalysisApp object

CProtocolAnalysisApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CProtocolAnalysisApp initialization
CString lwtProgramName = "Intrusion deteciton system by liuwentao";
HANDLE ProgramValue = (HANDLE) 1;
BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
	HANDLE h = GetProp(hwnd, lwtProgramName);
	if (h == ProgramValue)
	{
		*(HWND *) lParam = hwnd;
		return false;
	}
	
	return true;
}
BOOL CProtocolAnalysisApp::InitInstance()
{
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc, (LPARAM) & oldHWnd); 
	if (oldHWnd != NULL)
	{
		m_pMainWnd->MessageBox ("本程序已经在运行了！","网络入侵检测系统《网络安全编程技术与实例》（作者：刘文涛）");
		::ShowWindow(oldHWnd, SW_SHOWNORMAL); 
		::SetForegroundWindow(oldHWnd); 
		return false; 
	}
	GetModuleFileName(NULL, Global_FilePath, _MAX_PATH);
	//////////////////////////////////////////////////////////////////////////////////
	int Length;
	Length=strlen(Global_FilePath);
	for(int i=Length-1;i>=0;i--)
	{
		char temp;
		temp=Global_FilePath[i];
		if(temp!='\\')
		{
			continue ;
		}
		else
		{
			break ;
		}
	}
	for(int j=0;j<i;j++)
	{
		lwtGlobal_FileDirectory[j]=Global_FilePath[j];
	}
	/////////////////////////////////////////////////////////////////////////////	
	AfxEnableControlContainer();
	
	CoInitialize(NULL);
#ifdef _AFXDLL
	Enable3dControls(); // Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic(); // Call this when linking to MFC statically
#endif				  
	CAboutDlg dlg1;
	dlg1.DoModal();
	CProtocolAnalysisDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
int CProtocolAnalysisApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	return CWinApp::ExitInstance();
}
