# Microsoft Developer Studio Project File - Name="Wippien" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=WIPPIEN - WIN32 LIBRELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Wippien.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Wippien.mak" CFG="WIPPIEN - WIN32 LIBRELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Wippien - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Wippien - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Wippien - Win32 LibDebug" (based on "Win32 (x86) Application")
!MESSAGE "Wippien - Win32 LibRelease" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Wippien - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "HAVESSL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41a /d "_DEBUG"
# ADD RSC /l 0x41a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib Shell32.lib Comdlg32.lib winmm.lib ..\Lib\Debug\cximage.lib ..\Lib\Debug\Jpeg.lib ..\Lib\Debug\png.lib ..\Lib\Debug\Tiff.lib ..\Lib\Debug\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib version.lib iphlpapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/Wippien.exe" /pdbtype:sept /FIXED:NO
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Wippien - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "HAVESSL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x41a /d "NDEBUG"
# ADD RSC /l 0x41a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 wsock32.lib Shell32.lib Comdlg32.lib winmm.lib Iphlpapi.lib ..\Lib\cximage.lib ..\Lib\Jpeg.lib ..\Lib\png.lib ..\Lib\Tiff.lib ..\Lib\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib version.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/Wippien.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "Wippien - Win32 LibDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "HAVESSL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "HAVESSL" /D "_WODVPNLIB" /D "_APPUPDLIB" /D "_WODXMPPLIB" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x41a /d "_DEBUG"
# ADD RSC /l 0x41a /d "_DEBUG" /d "_APPUPDLIB"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib Shell32.lib Comdlg32.lib winmm.lib ..\Lib\Debug\cximage.lib ..\Lib\Debug\Jpeg.lib ..\Lib\Debug\png.lib ..\Lib\Debug\Tiff.lib ..\Lib\Debug\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib version.lib iphlpapi.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/Wippien.exe" /pdbtype:sept /FIXED:NO
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wsock32.lib Shell32.lib Comdlg32.lib winmm.lib ..\Lib\Debug\cximage.lib ..\Lib\Debug\Jpeg.lib ..\Lib\Debug\png.lib ..\Lib\Debug\Tiff.lib ..\Lib\Debug\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib version.lib iphlpapi.lib ..\..\..\WeOnlyDo\wodVPN\Code\Win32LIB\Debug\wodVPN.lib ..\..\..\WeOnlyDo\wodAppUpdate\Code\Win32LIB\Debug\wodAppUp.lib ..\..\..\WeOnlyDo\wodXMPP\Code\Win32LIB\Debug\wodXMPP.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/Wippien.exe" /pdbtype:sept /FIXED:NO
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Wippien - Win32 LibRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "HAVESSL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_STATIC_REGISTRY" /D "HAVESSL" /D "_WODVPNLIB" /D "_APPUPDLIB" /D "_WODXMPPLIB" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x41a /d "NDEBUG"
# ADD RSC /l 0x41a /d "NDEBUG" /d "_APPUPDLIB"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wsock32.lib Shell32.lib Comdlg32.lib winmm.lib Iphlpapi.lib ..\Lib\cximage.lib ..\Lib\Jpeg.lib ..\Lib\png.lib ..\Lib\Tiff.lib ..\Lib\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib version.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/Wippien.exe"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 wsock32.lib Shell32.lib Comdlg32.lib winmm.lib Iphlpapi.lib ..\Lib\cximage.lib ..\Lib\Jpeg.lib ..\Lib\png.lib ..\Lib\Tiff.lib ..\Lib\zlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib version.lib ..\..\..\WeOnlyDo\wodVPN\Code\Win32LIB\Release\wodVPN.lib ..\..\..\WeOnlyDo\wodAppUpdate\Code\Win32LIB\Release\wodAppUp.lib ..\..\..\WeOnlyDo\wodXMPP\Code\Win32LIB\Release\wodXMPP.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/Wippien.exe"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "Wippien - Win32 Debug"
# Name "Wippien - Win32 Release"
# Name "Wippien - Win32 LibDebug"
# Name "Wippien - Win32 LibRelease"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BaloonHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\BaloonTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatRoom.cpp
# End Source File
# Begin Source File

SOURCE=.\ComBSTR2.cpp
# End Source File
# Begin Source File

SOURCE=.\ContactAuthDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\crypto_aes.cpp
# End Source File
# Begin Source File

SOURCE=.\CwodWinSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadSkinDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Ethernet.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtWndShadow.cpp
# End Source File
# Begin Source File

SOURCE=.\Jabber.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgWin.cpp
# End Source File
# Begin Source File

SOURCE=.\Notify.cpp
# End Source File
# Begin Source File

SOURCE=.\Ping.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SDKMessageLink.cpp
# End Source File
# Begin Source File

SOURCE=.\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UpdateHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\User.cpp
# End Source File
# Begin Source File

SOURCE=.\UserList.cpp
# End Source File
# Begin Source File

SOURCE=.\VividTree.cpp
# End Source File
# Begin Source File

SOURCE=.\wildmat.cpp
# End Source File
# Begin Source File

SOURCE=.\Wippien.cpp
# End Source File
# Begin Source File

SOURCE=.\Wippien.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\BalloonDialog.h
# End Source File
# Begin Source File

SOURCE=.\BaloonHelp.h
# End Source File
# Begin Source File

SOURCE=.\BaloonTip.h
# End Source File
# Begin Source File

SOURCE=.\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\ChatRoom.h
# End Source File
# Begin Source File

SOURCE=.\ComBSTR2.h
# End Source File
# Begin Source File

SOURCE=.\ContactAuthDlg.h
# End Source File
# Begin Source File

SOURCE=.\crypto_aes.h
# End Source File
# Begin Source File

SOURCE=.\CwodWinSocket.h
# End Source File
# Begin Source File

SOURCE=.\DownloadSkinDlg.h
# End Source File
# Begin Source File

SOURCE=.\Ethernet.h
# End Source File
# Begin Source File

SOURCE=.\ExtWndShadow.h
# End Source File
# Begin Source File

SOURCE=.\HideContact.h
# End Source File
# Begin Source File

SOURCE=.\Jabber.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# Begin Source File

SOURCE=.\MenuXP.h
# End Source File
# Begin Source File

SOURCE=.\MsgWin.h
# End Source File
# Begin Source File

SOURCE=.\Notify.h
# End Source File
# Begin Source File

SOURCE=.\Ping.h
# End Source File
# Begin Source File

SOURCE=.\PNGButton.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\proto.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ScrollerCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SDKMessageLink.h
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\SettingsDlg.h
# End Source File
# Begin Source File

SOURCE=.\SimpleHttpRequest.h
# End Source File
# Begin Source File

SOURCE=.\SimpleXmlParser.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\UpdateHandler.h
# End Source File
# Begin Source File

SOURCE=.\User.h
# End Source File
# Begin Source File

SOURCE=.\UserList.h
# End Source File
# Begin Source File

SOURCE=.\versionno.h
# End Source File
# Begin Source File

SOURCE=.\VividTree.h
# End Source File
# Begin Source File

SOURCE=.\WebBrowserEvents.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\Gfx\away.ico
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=..\Gfx\do_not_disturb.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\dragdrop_16.bmp
# End Source File
# Begin Source File

SOURCE=.\dragdrop_16.bmp
# End Source File
# Begin Source File

SOURCE=..\Gfx\extended_away.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\free_for_chat.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\icon4.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\invisible.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\no.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\offline.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\online.ico
# End Source File
# Begin Source File

SOURCE=..\Gfx\splash2.jpg
# End Source File
# Begin Source File

SOURCE=..\Gfx\test1.bmp
# End Source File
# Begin Source File

SOURCE=.\Wippien.rgs
# End Source File
# Begin Source File

SOURCE=..\Gfx\WizardImage.bmp
# End Source File
# Begin Source File

SOURCE=..\Gfx\yes.ico
# End Source File
# End Group
# Begin Source File

SOURCE="..\Gfx\ac0038-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\ac0038-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ac0052-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\ac0052-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ac0052-24.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ac0057-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\ac0057-48.png"
# End Source File
# Begin Source File

SOURCE=".\ac0057-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ac0058-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\ac0058-48.png"
# End Source File
# Begin Source File

SOURCE=".\ac0058-48.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\aim.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\ALL CAPS!!!.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\ALL CAPS!!!.png"
# End Source File
# Begin Source File

SOURCE=".\ALL CAPS!!!.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\base_avatar.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\base_skin.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Beatnik.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Beatnik.png
# End Source File
# Begin Source File

SOURCE=.\Beatnik.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\bell.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\bold.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\bold.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\BushBaby.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\BushBaby.png
# End Source File
# Begin Source File

SOURCE=.\BushBaby.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\chatrooms.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\clear_history.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Clueless.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Clueless.png
# End Source File
# Begin Source File

SOURCE=.\Clueless.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\co0001-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\co0001-48.png"
# End Source File
# Begin Source File

SOURCE=".\co0001-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\co0009-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\co0017-16-prekrizeno.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\co0017-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\co0017-24-prekrizeno.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\co0017-24.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\Copy.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\RightClick\Copy.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\ec0054-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ec0054-48.png"
# End Source File
# Begin Source File

SOURCE=".\ec0054-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ei0029-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\ei0029-16.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\emoticons.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\fontbackcolor.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\fontbackcolor.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\fontforecolor.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\fontforecolor.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\fontsizedown.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\fontsizedown.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\fontsizenormal.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\fontsizenormal.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\fontsizeup.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\fontsizeup.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Forum Flirt.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Forum Flirt.png"
# End Source File
# Begin Source File

SOURCE=".\Forum Flirt.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Gamer.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Gamer.png
# End Source File
# Begin Source File

SOURCE=.\Gamer.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\google.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\group_down.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\group_up.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\HopelesslyAddicted.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\HopelesslyAddicted.png
# End Source File
# Begin Source File

SOURCE=.\HopelesslyAddicted.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\hyperlink.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\hyperlink.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\icq.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\italic.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\italic.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Jekyll and Hyde.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Jekyll and Hyde.png"
# End Source File
# Begin Source File

SOURCE=".\Jekyll and Hyde.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Joker.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Joker.png
# End Source File
# Begin Source File

SOURCE=.\Joker.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Linux Zealot.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Linux Zealot.png"
# End Source File
# Begin Source File

SOURCE=".\Linux Zealot.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\lock.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Lurker.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Lurker.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\MacZealot.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MacZealot.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\mainwnd_banner_2back.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\mainwnd_banner_back.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Moderator.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Moderator.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\msgin.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\Sounds\msgin.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\msgout.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\Sounds\msgout.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\msn.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\nd0009-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\nd0009-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\nd0012-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\nd0012-16.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Newbie.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Newbie.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\ni0070-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ni0073-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\ni0073-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\ni0091-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\No Dissent.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\No Dissent.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\no_preview.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\nono.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\Sounds\nono.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\off.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\Sounds\off.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\on.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\Sounds\on.wav
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\PCWeenie.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\PCWeenie.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Post Padder.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Post Padder.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Push My Button.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Push My Button.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Ray of Sunshine.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Ray of Sunshine.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\rename-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\rename-16.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Ringmaster.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Ringmaster.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Robot.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Robot.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Rumor Junkie.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Rumor Junkie.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Sozzled Surfer.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Sozzled Surfer.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Spks in Tungz.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Spks in Tungz.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Statistician.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Statistician.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Tech Support.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Tech Support.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\The Referee.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\The Referee.png"
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\TheGuru.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\TheGuru.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\tree_closed.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\tree_opened.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\MsgWin\underline.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\underline.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Avatars\Uptight.png
# End Source File
# Begin Source File

SOURCE=..\Gfx\Uptight.png
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\wi0041-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\wi0041-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\wi0041-24.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\RightClick\wi0102-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\wi0102-16.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\wi0152-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\wi0152-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\wi0153-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\wi0153-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\wi0155-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\wi0155-48.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Avatars\Windows Zealot.png"
# End Source File
# Begin Source File

SOURCE="..\Gfx\Windows Zealot.png"
# End Source File
# Begin Source File

SOURCE=".\Windows Zealot.png"
# End Source File
# Begin Source File

SOURCE=..\WipUpd\Release\WipUpd.exe
# End Source File
# Begin Source File

SOURCE=..\Gfx\yah.png
# End Source File
# Begin Source File

SOURCE=..\Lib\ssleay32.lib
# End Source File
# Begin Source File

SOURCE=..\Lib\libeay32.lib
# End Source File
# Begin Source File

SOURCE=..\Lib\SkinMagic.lib
# End Source File
# End Target
# End Project
