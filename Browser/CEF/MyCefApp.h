#ifndef _MYCEFAPP_H_
#define _MYCEFAPP_H_

#include "include\cef_app.h"

class CMyCefApp: public CefApp,
	public CefBrowserProcessHandler,
	public CefRenderProcessHandler
{

public:
	CMyCefApp(void);
	~CMyCefApp(void);

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
		OVERRIDE { return this; }

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() 
		OVERRIDE { return this; }

	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() OVERRIDE;

	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CMyCefApp);

protected:
	bool GetExtendFuncNameValue(std::vector<std::string> &funcList);
};


#endif//_MYCEFAPP_H_
