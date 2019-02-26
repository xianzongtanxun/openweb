#include "stdafx.h"
#include "CCEFV8HandlerEx.h"
#include <string>
#include <strsafe.h>
CCEFV8HandlerEx::CCEFV8HandlerEx()
{

}


CCEFV8HandlerEx::~CCEFV8HandlerEx(void)
{
	if (!callback_map_.empty())
	{
		CallbackMap::iterator it = callback_map_.begin();
		for (; it != callback_map_.end();)
		{
			if (it->second.first->IsSame(it->second.first))
			{
				callback_map_.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
}

//name:javascript调用C++方法名称
//object:javascript调用者对象
//arguments：js传递的参数
//retval：返回给js的值设置给这个对象
//exception：通知异常信息给js
bool CCEFV8HandlerEx::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	if (name == _T("addFunction"))
	{
		int nSum = 0;
		for (size_t i = 0;i<arguments.size();++i)
		{
			if (!arguments[i]->IsInt())
			{
				return false;
			}
			nSum += arguments[i]->GetIntValue();
		}
		retval = CefV8Value::CreateInt(nSum);
		return true;
	}
	else if (name == _T("hehe"))
	{
		retval = CefV8Value::CreateString(_T("hehe hehe!"));
		return true;
	}
	else if (name == _T("myfunc"))
	{
		retval = CefV8Value::CreateString("My Value!");
		return true;
	}
	else if (name == _T("register"))
	{
		/*if (arguments.size() == 1 && arguments[0]->IsFunction())
		{
			callback_func_ = arguments[0];
			callback_context_ = CefV8Context::GetCurrentContext();
			return true;
		}*/
		MessageBox(NULL, _T("C++ register方法"), _T("提示"), MB_OKCANCEL);
		return false;
	}
	else
	{
		return false;
	}
	//if (name == "NativeLogin")
	//{
	//	//Window Binding
	//	{
	//		CefString strUser = arguments.at(0)->GetStringValue();
	//		CefString strPassword = arguments.at(1)->GetStringValue();

	//		CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("login_msg");
	//		CefRefPtr<CefListValue> args = msg->GetArgumentList();
	//		args->SetSize(2);
	//		args->SetString(0, strUser);
	//		args->SetString(1, strPassword);

	//		//send the process message to the browser process
	//		CefV8Context::GetCurrentContext()->GetBrowser()->SendProcessMessage(PID_BROWSER, msg);
	//		//retval = CefV8Value::CreateInt();//函数的返回值，我们可以拿这个返回值做判断或者其他操作
	//	}
	//	/*else
	//	{
	//		retval = CefV8Value::CreateInt();
	//	}
	//	return true;*/
	//}
	//else if (name == "GetId")
	//{
	//	//Js Extensions
	//	CefRefPtr<CefFrame> frame = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();
	//	frame->ExecuteJavaScript("alert('Get login reply from browser process')",frame->GetURL(),0);

	//	//return to js
	//	//retval = CefV8Value::CreateString('');
	//	return true;
	//}
	return false;
}
