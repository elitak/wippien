
Wippienps.dll: dlldata.obj Wippien_p.obj Wippien_i.obj
	link /dll /out:Wippienps.dll /def:Wippienps.def /entry:DllMain dlldata.obj Wippien_p.obj Wippien_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del Wippienps.dll
	@del Wippienps.lib
	@del Wippienps.exp
	@del dlldata.obj
	@del Wippien_p.obj
	@del Wippien_i.obj
