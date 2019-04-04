#pragma once
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_v8.h"

class MyV8Handler : public CefV8Handler {
public:
	
	MyV8Handler(CefRefPtr<CefFrame> frame); 

	~MyV8Handler(); 

	virtual bool Execute(const CefString& name,
		CefRefPtr<CefV8Value> object,
		const CefV8ValueList& arguments,
		CefRefPtr<CefV8Value>& retval,
		CefString& exception) OVERRIDE;

	// Provide the reference counting implementation for this class.
	IMPLEMENT_REFCOUNTING(MyV8Handler);

private:

	//pos print extend
	HMODULE hdll_Printer;

};

