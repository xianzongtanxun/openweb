#include "stdafx.h"
#include "CCefV8HandlerEx.h"
#include <string>
#include <strsafe.h>
#include "MainFrame.h"

CCefV8HandlerEx::CCefV8HandlerEx(CMainFrame* pMainFrame, HWND hwnd):m_pMainFrame(pMainFrame),m_hwnd(hwnd)
{

}


CCefV8HandlerEx::~CCefV8HandlerEx(void)
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

void CCefV8HandlerEx::SetMainFrame(CMainFrame* m_MainFrame)
{
	m_pMainFrame = m_MainFrame;
}

//name:javascript����C++��������
//object:javascript�����߶���
//arguments��js���ݵĲ���
//retval�����ظ�js��ֵ���ø��������
//exception��֪ͨ�쳣��Ϣ��js
bool CCefV8HandlerEx::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
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
		MessageBox(NULL, _T("C++ register����"), _T("��ʾ"), MB_OKCANCEL);
		return false;
	}
	else if (name == _T("clientStart"))//��ʼ��ǩ
	{
		OutputDebugStringA("\n");
		OutputDebugStringA("clientStart");
		OutputDebugStringA("\n");
		if (arguments.size() == 4 && arguments[0]->IsString() && arguments[1]->IsString() && arguments[2]->IsString() && arguments[3]->IsString())
		{
			USES_CONVERSION;
			CefString strCapture = arguments.at(0)->GetStringValue();
			CefString strRender = arguments.at(1)->GetStringValue();
			CefString strToken = arguments.at(2)->GetStringValue();
			CefString strObjectName = arguments.at(3)->GetStringValue();
			
			wstring strData = name;
			strData.append(_T("$"));
			strData.append(strCapture);
			strData.append(_T("$"));
			strData.append(strRender);
			strData.append(_T("$"));
			strData.append(strToken);
			strData.append(_T("$"));
			strData.append(strObjectName);
			COPYDATASTRUCT cds;
			const wchar_t* lpData = strData.c_str();
			cds.lpData = (LPVOID)lpData;
			cds.cbData = (wcslen(lpData) + 1) * sizeof(WCHAR);
			m_hwnd = ::FindWindow(_T("MainSuzuki"), _T("Suzuki"));
			if (m_hwnd && IsWindow(m_hwnd))
			{
				::SendMessage(m_hwnd, WM_COPYDATA, NULL, (LPARAM)&cds);
			}
			//if (m_pMainFrame){
				//m_pMainFrame->StartFromJsWithFileName(W2A(strCapture.c_str()), W2A(strRender.c_str()), W2A(strToken.c_str()), W2A(strObjectName.c_str()));
			//}
			return true;
		}
	}
	else if (name == _T("clientStop"))//������ǩ
	{
		OutputDebugStringA("\n");
		OutputDebugStringA("clientStop");
		OutputDebugStringA("\n");
		
		COPYDATASTRUCT cds;
		const wchar_t* lpData = name.c_str();
		cds.lpData = (LPVOID)lpData;
		cds.cbData = (wcslen(lpData) + 1) * sizeof(WCHAR);
		m_hwnd = ::FindWindow(_T("MainSuzuki"), _T("Suzuki"));
		if (m_hwnd && IsWindow(m_hwnd))
		{
			::SendMessage(m_hwnd, WM_COPYDATA, NULL, (LPARAM)&cds);
		}
		return true;
	}
	else
	{
		return false;
	}
	return false;
}
