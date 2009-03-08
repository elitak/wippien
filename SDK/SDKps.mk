
SDKps.dll: dlldata.obj SDK_p.obj SDK_i.obj
	link /dll /out:SDKps.dll /def:SDKps.def /entry:DllMain dlldata.obj SDK_p.obj SDK_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del SDKps.dll
	@del SDKps.lib
	@del SDKps.exp
	@del dlldata.obj
	@del SDK_p.obj
	@del SDK_i.obj
