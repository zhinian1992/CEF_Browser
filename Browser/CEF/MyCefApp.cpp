#include "MyCefApp.h"
#include "V8handler.h"

CMyCefApp::CMyCefApp(void)
{

}


CMyCefApp::~CMyCefApp(void)
{

}


void CMyCefApp::OnContextInitialized() {

}

void CMyCefApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) {
	//加载flash插件
	command_line->AppendSwitchWithValue("ppapi-flash-path", "PepperFlash/pepflashplayer32_31_0_0_153.dll");
	command_line->AppendSwitchWithValue("ppapi-flash-version", "31.0.0.153");

	//禁止DPI缩放
	//command_line->AppendSwitchWithValue("--force-device-scale-factor", "1");

	//允许启动摄像头
	command_line->AppendSwitch("allow-running-insecure-content");
	command_line->AppendSwitchWithValue("enable-media-stream", "1");
	command_line->AppendSwitch("allow-insecure-localhost");
}

void CMyCefApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefV8Context> context) {

  //添加C++/JS交互方法
  CefRefPtr<CefV8Value> window = context->GetGlobal();
  CefRefPtr<CefV8Handler> handler = new MyV8Handler(browser->GetMainFrame());
  CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("test", handler);
  window->SetValue("test", func, V8_PROPERTY_ATTRIBUTE_NONE);

}