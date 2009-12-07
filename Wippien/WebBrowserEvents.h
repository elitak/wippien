#ifndef __WEBBROWSEREVENTS
#define __WEBBROWSEREVENTS

extern _ATL_FUNC_INFO WebBrowserEventsInfo;

template <class T>
class CWebBrowserEvents : public IDispEventSimpleImpl<1, CWebBrowserEvents<T>, &DIID_DWebBrowserEvents2>
{
public:
    CWebBrowserEvents (T * Owner, IWebBrowser2 *pWebBrowser)
    {
        m_pWebBrowser = pWebBrowser;
        DispEventAdvise ( (IUnknown*)m_pWebBrowser);
//		m_Initial = TRUE;
		m_Owner = Owner;
//		m_Cancel = FALSE;
    }
//	BOOL m_Cancel;
	T* m_Owner;

//	static _ATL_FUNC_INFO SkinDlgBeforeNavigate2Info;// = {CC_STDCALL, VT_EMPTY, 7, {VT_DISPATCH,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_VARIANT,VT_BYREF|VT_BOOL}};


    virtual ~CWebBrowserEvents ()
    {
        DispEventUnadvise ( (IUnknown*)m_pWebBrowser);
//        m_pWebBrowser->Release();
    }

	void __stdcall __StatusTextChange(/*[in]*/ BSTR bstrText)
	{
	}

	void __stdcall __ProgressChange(/*[in]*/ long progress, /*[in]*/ long progressMax)
	{
	}

	void __stdcall __CommandStateChange(/*[in]*/ long Command, /*[in]*/ VARIANT_BOOL Enable)
	{
	}

	void __stdcall __DownloadBegin()
	{
	}

	void __stdcall __DownloadComplete()
	{
	}
	
	void __stdcall __TitleChange(/*[in]*/ BSTR bstrText)
	{
	}
	
	void __stdcall __NavigateComplete2(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL)
	{
	}
	
	void __stdcall __BeforeNavigate2(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL, /*[in]*/ VARIANT* Flags, 
		/*[in]*/ VARIANT* TargetFrameName, /*[in]*/ VARIANT* PostData, /*[in]*/ VARIANT* Headers, 
		/*[out]*/ VARIANT_BOOL* Cancel)
	{
		m_Owner->__BeforeNavigate2(pDisp, URL, Flags, TargetFrameName, PostData, Headers, Cancel);
	}
	void __stdcall __PropertyChange(/*[in]*/ BSTR bstrProperty)
	{
	}
	
	void __stdcall __NewWindow2(/*[out]*/ IDispatch** ppDisp, /*[out]*/ VARIANT_BOOL* Cancel)
	{
	}
	
	void __stdcall __DocumentComplete(/*[in]*/ IDispatch* pDisp, /*[in]*/ VARIANT* URL)
	{
	}
	
	void __stdcall __OnQuit()
	{
	}
	
	void __stdcall __OnVisible(/*[in]*/ VARIANT_BOOL Visible)
	{
	}

	void __stdcall __OnToolBar(/*[in]*/ VARIANT_BOOL ToolBar)
	{
	}

	void __stdcall __OnMenuBar(/*[in]*/ VARIANT_BOOL MenuBar)
	{
	}
	
	void __stdcall __OnStatusBar(/*[in]*/ VARIANT_BOOL StatusBar)
	{
	}

	void __stdcall __OnFullScreen(/*[in]*/ VARIANT_BOOL FullScreen)
	{
	}

	void __stdcall __OnTheaterMode(/*[in]*/ VARIANT_BOOL TheaterMode)
	{
	}
    BEGIN_SINK_MAP (CWebBrowserEvents)
		SINK_ENTRY_INFO(1, DIID_DWebBrowserEvents2, 250, __BeforeNavigate2, &WebBrowserEventsInfo)
	END_SINK_MAP ()
private:
    IWebBrowser2 *m_pWebBrowser;
	//BOOL m_Initial;
};


#endif