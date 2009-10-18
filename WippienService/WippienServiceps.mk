
WippienServiceps.dll: dlldata.obj WippienService_p.obj WippienService_i.obj
	link /dll /out:WippienServiceps.dll /def:WippienServiceps.def /entry:DllMain dlldata.obj WippienService_p.obj WippienService_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del WippienServiceps.dll
	@del WippienServiceps.lib
	@del WippienServiceps.exp
	@del dlldata.obj
	@del WippienService_p.obj
	@del WippienService_i.obj
