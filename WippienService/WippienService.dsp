# Microsoft Developer Studio Project File - Name="WippienService" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WippienService - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WippienService.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WippienService.mak" CFG="WippienService - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WippienService - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "WippienService - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WippienService - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "\WeOnlyDo\OpenSSL\x86\inc32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WIPPIENSERVICE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41a /d "_DEBUG"
# ADD RSC /l 0x41a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gdi32.lib user32.lib version.lib wininet.lib comctl32.lib wsock32.lib iphlpapi.lib \WeOnlyDo\wodVPN\Code\Win32LIB\Debug\wodVPN.lib \WeOnlyDo\wodXMPP\Code\Win32LIB\Debug\wodXMPP.lib \WeOnlyDo\wodXMPP\Code\libpng\Debug\libpng.lib \WeOnlyDo\wodXMPP\Code\zlib\Debug\zlib.lib ssleay32.lib libeay32.lib Secur32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"\WeOnlyDo\OpenSSL\x86\out32"
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\WippienService.exe
InputPath=.\Debug\WippienService.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "WippienService - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WippienService___Win32_Release"
# PROP BASE Intermediate_Dir "WippienService___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "\WeOnlyDo\OpenSSL\x86\inc32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "_WIPPIENSERVICE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x41a /d "NDEBUG"
# ADD RSC /l 0x41a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 gdi32.lib user32.lib version.lib wininet.lib comctl32.lib wsock32.lib iphlpapi.lib \WeOnlyDo\wodVPN\Code\Win32LIB\Release\wodVPN.lib \WeOnlyDo\wodXMPP\Code\Win32LIB\Release\wodXMPP.lib \WeOnlyDo\wodXMPP\Code\libpng\Release\libpng.lib \WeOnlyDo\wodXMPP\Code\zlib\Release\zlib.lib ssleay32.lib libeay32.lib Secur32.lib /nologo /subsystem:windows /machine:I386 /libpath:"\WeOnlyDo\OpenSSL\x86\out32"
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\Release\WippienService.exe
InputPath=.\Release\WippienService.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "WippienService - Win32 Debug"
# Name "WippienService - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\MiniVPN\Ethernet.cpp
# End Source File
# Begin Source File

SOURCE=..\MiniVPN\JabberLib.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\MiniVPN\User.cpp
# End Source File
# Begin Source File

SOURCE=.\WippienService.cpp
# End Source File
# Begin Source File

SOURCE=.\WippienService.idl
# ADD MTL /tlb ".\WippienService.tlb" /h "WippienService.h" /iid "WippienService_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\WippienService.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\MiniVPN\Ethernet.h
# End Source File
# Begin Source File

SOURCE=..\MiniVPN\JabberLib.h
# End Source File
# Begin Source File

SOURCE=..\MiniVPN\proto.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\MiniVPN\User.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\WippienService.rgs
# End Source File
# End Group
# End Target
# End Project
