# Microsoft Developer Studio Project File - Name="IDS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **


# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=IDS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "intrusiondetectionsystem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "intrusiondetectionsystem.mak" CFG="IDS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IDS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "IDS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IDS - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 wpcap.lib ws2_32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "IDS - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wpcap.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "IDS - Win32 Release"
# Name "IDS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\arpprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ethernetprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\filterdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\icmpprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\intrusiondetect.cpp
# End Source File
# Begin Source File

SOURCE=.\intrusiondetectionsystem.rc
# End Source File
# Begin Source File

SOURCE=.\ipprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\mydialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ParameterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Protocolanalysis.cpp
# End Source File
# Begin Source File

SOURCE=.\ProtocolanalysisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\sniffer.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\tcpprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\udpprotocol.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\arpprotocol.h
# End Source File
# Begin Source File

SOURCE=.\DeviceDialog.h
# End Source File
# Begin Source File

SOURCE=.\ethernetprotocol.h
# End Source File
# Begin Source File

SOURCE=.\filterdlg.h
# End Source File
# Begin Source File

SOURCE=.\icmpprotocol.h
# End Source File
# Begin Source File

SOURCE=.\intrusiondetect.h
# End Source File
# Begin Source File

SOURCE=.\IntrusionRuleDialog.h
# End Source File
# Begin Source File

SOURCE=.\ipprotocol.h
# End Source File
# Begin Source File

SOURCE=.\mydialog.h
# End Source File
# Begin Source File

SOURCE=.\ParameterDlg.h
# End Source File
# Begin Source File

SOURCE=.\Protocolanalysis.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolanalysisDlg.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\sniffer.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\tcpprotocol.h
# End Source File
# Begin Source File

SOURCE=.\udpprotocol.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\about.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon8.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section IDS : {D27CDB6C-AE6D-11CF-96B8-444553540000}
# 	2:5:Class:CShockwaveFlash
# 	2:10:HeaderFile:shockwaveflash.h
# 	2:8:ImplFile:shockwaveflash.cpp
# End Section
# Section IDS : {D27CDB6E-AE6D-11CF-96B8-444553540000}
# 	2:21:DefaultSinkHeaderFile:shockwaveflash.h
# 	2:16:DefaultSinkClass:CShockwaveFlash
# End Section
