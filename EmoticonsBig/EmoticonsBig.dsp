# Microsoft Developer Studio Project File - Name="EmoticonsBig" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EmoticonsBig - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EmoticonsBig.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EmoticonsBig.mak" CFG="EmoticonsBig - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EmoticonsBig - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Objs"
# PROP Intermediate_Dir "Objs"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMOTICONS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "EMOTICONS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41a /d "NDEBUG"
# ADD RSC /l 0x41a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /machine:I386 /out:"../Bin/Emotico2.dll"
# Begin Target

# Name "EmoticonsBig - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Wippien\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\em.def
# End Source File
# Begin Source File

SOURCE=.\Emoticons.cpp
# End Source File
# Begin Source File

SOURCE=.\Emoticons.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Wippien\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\GFX\acute.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\aggressive.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\aikido.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\air_kiss.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\alcoholic.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\alien.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\angry2.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\artist.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\baby.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\bad.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\badgoalie.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\bag.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\balloon.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\band.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\bann.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\banned.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\bataita.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\bb.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\beach.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\bear_yes.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\beee.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\beer.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\big_boss.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\biggrin.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\biggrin2.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\biker.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\blink.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\blow.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\blum.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\blushing.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\book.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\boredom.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\boxing.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\brunette.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\butcher.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\bye.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cat.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cat2.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cheer.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cheers.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\chef.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\chris.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\clap.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\clapping.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\close_tema.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\clover.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\clown.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\comando.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cool.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cow.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cowboy.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\cray.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\crazy.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\crazy_pilot.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\dance.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\dance2.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\dance3.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\dance4.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\derisive.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\devil.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\dirol.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\doctor.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\doh.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\donatello.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\download.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\drag.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\drinks.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\drunk.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\dry.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\dwarf.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\eat.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\eek.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\elf.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\ermm.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\eusa_think.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\excl.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\feminist_en.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\fingers.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\first_move.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\fish.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\flex.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\flirt.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\flowers1.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\focus.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\fool.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\friends.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\frusty.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\gathering.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\giggle.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\girl_devil.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\girl_werewolf.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\give_heart.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\give_rose.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\good.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\grin.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\haha.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\happy.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\headspin.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\help.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\hi.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\homestar.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\hug.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\huglove.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\huh.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\hunter.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\ill.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\imao.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\innocent.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\jester.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\juggle.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\king.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\king2.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\kiss.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\kisses.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\laugh.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\lazy.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\Lighten.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\mad.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\mamba.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\man_in_love.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\mda.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\meeting.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\mellow.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\moil.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\nea.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\neo.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\nep.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\nhl.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\no.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\not_i.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\notworthy.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\nuke.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\nyam.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\offtopic.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\ohmy.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\ok.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\online2long.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\oops.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\orc.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\paladin.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pardon.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\party.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\partytime2.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pash.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\patsak.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pepsi.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\ph34r.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\phiu.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\phone.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\photo.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pilot.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pimp.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pinch.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pioneer.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pizza.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\plane.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pleasantry.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pogranichnik.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pokey.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\pope.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\protest.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\queen.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\rabbi.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\rap.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\read.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\resent.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\ro.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\rock.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\rofl.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\rolleyes.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\rotflmao.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\rtfm.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\russian.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\sad.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\salut.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\santa.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\santa2.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\scare.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\scenic.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\sclerosis.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\scooter.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\scout_en.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\search.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\secret.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\serenade.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\shades.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\shaun.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\shifty.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\shipwrecked.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\shok.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\shout.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\shuriken.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\skull.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\sleeping.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\slow_en.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\smartass.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\smile.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\snegurochka.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\snooks.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\sorcerer.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\sorry.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\sp_ike.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\spam.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\spiteful.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\spruce_up.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\stars.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\stink.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\stinker.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\stop.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\stretcher.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\stuart.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\suicide.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\superman.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\superstition.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\surrender.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\sweatingbullets.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\swoon.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\swordfight.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\take_example.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\taunt.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\tease.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\telephone.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\this.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\threaten.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\thumbsup.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\to_become_senile.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\to_keep_order.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\tomato.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\tongue.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\tooth.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\triniti.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\turned.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\umnik.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\unknw.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\unsure.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\vampire.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\vava.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\victory.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\vinsent.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\w00t.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\wacko.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\wardellipse.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\whack.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\whistling.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\wink.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\wizard.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\wub.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\yahoo.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\yawn.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\yes.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\yu.gif
# End Source File
# Begin Source File

SOURCE=.\GFX\zorro.gif
# End Source File
# End Group
# Begin Source File

SOURCE=.\GFX\mellow.png
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\SND\smile.wav
# End Source File
# End Target
# End Project
