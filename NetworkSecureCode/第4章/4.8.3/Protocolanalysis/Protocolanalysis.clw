; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProtocolAnalysisDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "protocolanalysis.h"
LastPage=0

ClassCount=7
Class1=CDeviceDialog
Class2=filterdlg
Class3=helpdialog
Class4=CParameterDlg
Class5=CProtocolAnalysisApp
Class6=CAboutDlg
Class7=CProtocolAnalysisDlg

ResourceCount=7
Resource1=IDD_ProtocolAnalysis_DIALOG
Resource2=IDD_FILTER_DLG
Resource3=IDD_DIALOG_PARAMETER
Resource4=IDD_DIALOG_DEVICE
Resource5=IDD_DIALOG_F1
Resource6=IDD_ABOUTBOX
Resource7=IDR_MENU1

[CLS:CDeviceDialog]
Type=0
BaseClass=CDialog
HeaderFile=DeviceDialog.h
ImplementationFile=DeviceDialog.cpp

[CLS:filterdlg]
Type=0
BaseClass=CDialog
HeaderFile=filterdlg.h
ImplementationFile=filterdlg.cpp

[CLS:helpdialog]
Type=0
BaseClass=CDialog
HeaderFile=helpdialog.h
ImplementationFile=helpdialog.cpp

[CLS:CParameterDlg]
Type=0
BaseClass=CDialog
HeaderFile=ParameterDlg.h
ImplementationFile=ParameterDlg.cpp

[CLS:CProtocolAnalysisApp]
Type=0
BaseClass=CWinApp
HeaderFile=Protocolanalysis.h
ImplementationFile=Protocolanalysis.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ProtocolanalysisDlg.h
ImplementationFile=ProtocolanalysisDlg.cpp

[CLS:CProtocolAnalysisDlg]
Type=0
BaseClass=CDialog
HeaderFile=ProtocolanalysisDlg.h
ImplementationFile=ProtocolanalysisDlg.cpp
LastObject=CProtocolAnalysisDlg

[DLG:IDD_DIALOG_DEVICE]
Type=1
Class=CDeviceDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_DEVICE,listbox,1353777411
Control4=IDC_STATIC,button,1342210311

[DLG:IDD_FILTER_DLG]
Type=1
Class=filterdlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_FILTER,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG_F1]
Type=1
Class=helpdialog
ControlCount=5
Control1=IDOK,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342177283
Control4=IDC_ZUOZHE,static,1342308352
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_INTRUSION_RULE]
Type=1
Class=CIntrusionRuleDialog

[DLG:IDD_DIALOG_PARAMETER]
Type=1
Class=CParameterDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_XUANXIANG,button,1342177287
Control4=IDC_CHECK_SAVE_DUMP_FILE,button,1342242819
Control5=IDC_CHECK_SHOW_PACKET_DATA,button,1342242819

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=1
Control1=IDC_STATIC,static,1342177294

[DLG:IDD_ProtocolAnalysis_DIALOG]
Type=1
Class=CProtocolAnalysisDlg
ControlCount=18
Control1=IDC_TAB1,SysTabControl32,1342177280
Control2=IDC_BUTTON_PARAMETER,button,1342275584
Control3=IDC_BUTTON_SET_FILTER,button,1342275584
Control4=IDC_BUTTON_OPEN_DUMP_FILE,button,1342275584
Control5=IDC_BUTTON_END,button,1342275584
Control6=IDC_BUTTON_START,button,1342275584
Control7=IDC_BUTTON_HIDE,button,1342275584
Control8=IDC_BUTTON_ABOUT,button,1342275584
Control9=IDC_CENTER,button,1342275584
Control10=IDC_LIST2,SysListView32,1350631425
Control11=IDC_LIST3,SysListView32,1350631425
Control12=IDC_LIST_IP,SysListView32,1350631425
Control13=IDC_LIST_TCP,SysListView32,1350631425
Control14=IDC_LIST_UDP,SysListView32,1350631425
Control15=IDC_LIST_ICMP,SysListView32,1350631425
Control16=IDC_STATIC_STATUS,static,1342308864
Control17=IDC_LIST_PPPOE,SysListView32,1350631425
Control18=IDC_STATIC_PACKET_COUNT,static,1342308352

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=ID_MENU_OPEN_DUMP_FILE
Command2=MENU_EXIT
Command3=MENU_START
Command4=MENU_STOP
Command5=MENU_SET_FILETER
Command6=ID_MENU_SET_PARAMETER
Command7=ID_MENU_ARP
Command8=ID_MENU_IP
Command9=ID_MENU_TCP
Command10=ID_MENU_UDP
Command11=ID_MENU_ICMP
Command12=ID_MENU_Ethernet
Command13=ID_MENU_PACKET
Command14=IDD_menu_Help
Command15=MENU_HELP
CommandCount=15

