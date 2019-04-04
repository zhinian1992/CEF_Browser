#include "V8handler.h"
#include <string>
using namespace std;


MyV8Handler::MyV8Handler(CefRefPtr<CefFrame> frame)
{
}

MyV8Handler::~MyV8Handler()
{

}

bool MyV8Handler::Execute(const CefString& name,
	CefRefPtr<CefV8Value> object,
	const CefV8ValueList& arguments,
	CefRefPtr<CefV8Value>& retval,
	CefString& exception){

	if (name == "test") {
		retval = CefV8Value::CreateString("test success!");
		return true;
	}
	return false;
}
