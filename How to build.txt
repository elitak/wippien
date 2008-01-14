How to build.txt

You must have VC6 and WTL installed on your system. We have provided source code for everything
we were allowed to do so. Some of the features or packages used by Wippien have been licensed
for binary use only, so they don't contain the source. 



----------------------------------------------------------------------------------------------
Prerequirements: download and install

* WeOnlyDo wodVPN Component - used for P2P
http://www.weonlydo.com/index.asp?showform=VPN

* WeOnlyDo wodXMPP Component - used for XMPP/Jabber protocol
http://www.weonlydo.com/index.asp?showform=XMPP

* WeOnlyDo wodAppUpdate Component - used for auto-update
http://www.weonlydo.com/index.asp?showform=AppUpdate

* Skinmagic - for skins
http://www.appspeed.com/

If you have purchased above components, then in Wippien\Stdafx.h file enter your license keys
so they are used with your "branded!!" copy of Wippien. Otherwise, DEMO version of above 
components will do just fine for your initial tests.


----------------------------------------------------------------------------------------------
Installation steps:

1. Open CxImage project, go to Build>Batch Build->Rebuild All
2. Open EmoticonsSmall project, go to Build>Batch Build->Rebuild All
3. Open EmoticonsBig project, go to Build>Batch Build->Rebuild All
4. Open Wippien, select 'Win32 Debug' and Build->Rebuild All

That's it. In Bin folder you should see Wippien.exe ready to run!
