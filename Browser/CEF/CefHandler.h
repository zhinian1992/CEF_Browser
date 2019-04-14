#pragma once
#include "include\cef_client.h"
#include <list>
#include <string>


class CCefHandler :
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler,
	public CefKeyboardHandler,
	public CefContextMenuHandler
{
public:

	CCefHandler();

	~CCefHandler(void);

	//自定义方法
	void CreateBrowser(HWND hParentWnd, const RECT& rect, CefString &url);

	CefRefPtr<CefBrowser> GetBrowser() {
		return m_browser;
	}
	CefRefPtr<CefFrame>	GetMainFram() {
		return m_browser.get() ? m_browser->GetMainFrame() : NULL;
	}
	HWND GetBrowserHostWnd() {
		return m_browser.get() ? m_browser->GetHost()->GetWindowHandle() : NULL;
	}

	std::wstring GetLoadingUrl();

	void reload();

	void goBack();

	void goForward();

	bool canGoForward();

	bool canGoBack();

	// CefClient methods
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE {
		return this;
	}

	// CefKeyboardHandler methods:
	virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event) OVERRIDE;

	// CefLoadHandler methods:
	virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) OVERRIDE;

	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;

	virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;

	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser);

	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;

	// CefLifeSpanHandler methods:
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& target_url,
		const CefString& target_frame_name,
		CefLifeSpanHandler::WindowOpenDisposition target_disposition,
		bool user_gesture,
		const CefPopupFeatures& popupFeatures,
		CefWindowInfo& windowInfo,
		CefRefPtr<CefClient>& client,
		CefBrowserSettings& settings,
		bool* no_javascript_access) OVERRIDE;

	// Request that all existing browser windows close.

	virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request) {
		return false;
	}

	//菜单处理
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, \
		CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;

	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, \
		CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) OVERRIDE;

private:
	HWND m_hWnd; //接收消息的句柄

	CefRefPtr<CefBrowser> m_browser;
	
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CCefHandler);

};
