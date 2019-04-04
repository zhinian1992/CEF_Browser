#include "CefHandler.h"
#include "util.h"
#include "include/cef_app.h"
#include "include/cef_runnable.h"


CCefHandler::CCefHandler()
	:_is_DevTools_open(false)
{
 
}
 
 
CCefHandler::~CCefHandler() 
{
 
}
 
void CCefHandler::CreateBrowser(HWND hParentWnd, const RECT& rect, CefString &url)
{
	CefWindowInfo info;
	CefBrowserSettings settings;
	static wchar_t* pCharset = (wchar_t*)L"GB2312";
	settings.default_encoding.str = pCharset;
	settings.default_encoding.length = wcslen(pCharset);
	info.SetAsChild(hParentWnd, rect);
	CefBrowserHost::CreateBrowser(info, this, url.c_str(), settings, NULL);
}

std::wstring CCefHandler::GetLoadingUrl()
{
	CefRefPtr<CefFrame> pMainFram = GetMainFram();
	return pMainFram.get() ? pMainFram->GetURL() : L"";
}
 
void CCefHandler::reload()
{
	m_browser->Reload();
}

void CCefHandler::goBack()
{
	if (m_browser->CanGoBack())
		m_browser->GoBack();
}

void CCefHandler::goForward()
{
	if (m_browser->CanGoForward())
		m_browser->GoForward();
}

bool CCefHandler::canGoForward()
{
	return m_browser->CanGoForward();
}

bool CCefHandler::canGoBack()
{
	return m_browser->CanGoBack();
}

//****************************************************
//������Ϣ��ȡ�ӿ�
bool CCefHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event)
{
	if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 122)
	{
		return true;
	}
	else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 123)
	{
		if (_is_DevTools_open)
		{
			_is_DevTools_open = false;
			browser->GetHost()->CloseDevTools();
			return true;
		}
		_is_DevTools_open = true;
		CefWindowInfo windowInfo;
		CefBrowserSettings settings;
		RECT rc = { 0,0,800,600 };
		windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
		browser->GetHost()->ShowDevTools(windowInfo, this, settings, CefPoint());
		return true;
	}
	else if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 116)
	{
		browser->Reload();
		return true;
	}
	return CefKeyboardHandler::OnKeyEvent(browser, event, os_event);
}

//****************************************************
//��ҳ����״̬�ص��ӿ�
void CCefHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
	return __super::OnLoadStart(browser, frame);
}

void CCefHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{

}

void CCefHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, \
	ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl)
{
	if (errorCode == ERR_ABORTED)
		return;

	// Display a load error message.
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL " << std::string(failedUrl) <<
		" with error " << std::string(errorText) << " (" << errorCode <<
		").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}

void CCefHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	m_browser = browser;
}

//****************************************************
//�˵����ؽӿ�
void CCefHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, \
	CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
	cef_context_menu_type_flags_t flag = params->GetTypeFlags();
	if (flag & CM_TYPEFLAG_PAGE)
	{//��ͨҳ����Ҽ���Ϣ
		model->SetLabel(MENU_ID_BACK, L"����");
		model->SetLabel(MENU_ID_FORWARD, L"ǰ��");
		model->SetLabel(MENU_ID_VIEW_SOURCE, L"�鿴Դ����");
		model->SetLabel(MENU_ID_PRINT, L"��ӡ");
		model->SetLabel(MENU_ID_RELOAD, L"ˢ��");
		model->SetLabel(MENU_ID_RELOAD_NOCACHE, L"ǿ��ˢ��");
		model->SetLabel(MENU_ID_STOPLOAD, L"ֹͣ����");
		model->SetLabel(MENU_ID_REDO, L"�ظ�");
	}
	if (flag & CM_TYPEFLAG_EDITABLE)
	{//�༭����Ҽ���Ϣ
		model->SetLabel(MENU_ID_UNDO, L"����");
		model->SetLabel(MENU_ID_REDO, L"����");
		model->SetLabel(MENU_ID_CUT, L"����");
		model->SetLabel(MENU_ID_COPY, L"����");
		model->SetLabel(MENU_ID_PASTE, L"ճ��");
		model->SetLabel(MENU_ID_DELETE, L"ɾ��");
		model->SetLabel(MENU_ID_SELECT_ALL, L"ȫѡ");
	}
}

bool CCefHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, \
	CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
{
	return false;
}

//****************************************************
//״̬�ı�ص��ӿ�
void CCefHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	
}

//****************************************************
//��ҳ�������ڻص��ӿ�
bool CCefHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	CefLifeSpanHandler::WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	//����ʹ��Ĭ�����������ҳ������CEF���´�������
	switch (target_disposition)
	{
	case WOD_NEW_FOREGROUND_TAB:
	case WOD_NEW_BACKGROUND_TAB:
	case WOD_NEW_POPUP:
	case WOD_NEW_WINDOW:
		browser->GetMainFrame()->LoadURL(target_url);
		return true; //cancel create
	}
	return false;
}

