#include "stdafx.h"
#include "ui_list_element.h"
//#include "CommonFunction.h"
//#include "ui_image_review.h"
#include "MainFrame.h"

#define B2b(val) ((val) == 0?false:true)
#define b2B(val) ((val)?TRUE:FALSE)
CTrayListElementUI::CTrayListElementUI()
	:m_pText(NULL),
		m_pLine(NULL),
		m_pHor(NULL),
		m_bEnable(true)
{
	m_dwTrayTextColor = 0xFF333333;
	m_dwTrayTextHotColor = 0xFF484848;
	OnInit();
}

void  CTrayListElementUI::OnInit(void)
{
	ASSERT(NULL == m_pText);
	m_pText = new CLabelUI;
	ASSERT(m_pText);
	Add(m_pText);
	m_pText->SetTextColor(m_dwTrayTextColor);
}

LPCTSTR CTrayListElementUI::GetClass() const
{
	return  DUI_CTR_LISTELEM;
}

LPVOID CTrayListElementUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, DUI_CTR_LISTELEM) == 0 ) return static_cast<CTrayListElementUI*>(this);
	return CListContainerElementUI::GetInterface(pstrName);
}

void CTrayListElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("traytextcolor")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTrayTextColor(clrColor);
		if(m_pText)
			m_pText->SetTextColor(m_dwTrayTextHotColor);
		return;
	}
	else if( _tcscmp(pstrName, _T("traytexthotcolor")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTrayTextHotColor(clrColor);
		return;
	}
	else if( _tcscmp(pstrName, _T("traydisabledcolor")) == 0 ) {
		while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetTrayTextDisabledColor(clrColor);
		return;
	}
	else if( _tcscmp(pstrName, _T("traytext")) == 0 ) {
		SetTrayText(pstrValue);
		return;
	}
	else if( _tcscmp(pstrName, _T("traynormalimage")) == 0 ) {
		SetTrayNormalImage(pstrValue);
		return;
	}
	else if( _tcscmp(pstrName, _T("trayhotimage")) == 0 ) {
		SetTrayHotImage(pstrValue);
		return;
	}
	else if( _tcscmp(pstrName, _T("traydisabledimage")) == 0 ) {
		SetTrayDisabledImage(pstrValue);
		return;
	}
	else if( _tcscmp(pstrName, _T("traytextpadding")) == 0 ){
		RECT rcPadding = { 0 };
		LPTSTR pstr = NULL;
		rcPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
		rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
		rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
		SetTrayTextPadding(rcPadding);
		return;
	}
	
	CListContainerElementUI::SetAttribute(pstrName,pstrValue);
}

void CTrayListElementUI::DoEvent(TEventUI& event)
{
	if( !OnEvent(&event))
	   return;

	if( event.Type == UIEVENT_MOUSEENTER )   //鼠标进入事件处理
	{
		if(!m_bEnable)
			return;
		m_pText->SetBkImage(m_trayHotImage);
		m_pText->SetTextColor(m_dwTrayTextHotColor);
	}
	else if( event.Type == UIEVENT_MOUSELEAVE )  //鼠标离开事件处理
	{
		if(!m_bEnable)
			return;
		m_pText->SetBkImage(m_trayNormalImage);
		m_pText->SetTextColor(m_dwTrayTextColor);
	}
	__super::DoEvent(event);
}

void CTrayListElementUI::SetTrayTextPadding(RECT rect)
{
	m_trayTextPadding = rect;
	if(m_pText)
		m_pText->SetTextPadding(rect);
}

RECT CTrayListElementUI::GetTrayTextPadding()
{
	return m_trayTextPadding;
}

LPCTSTR CTrayListElementUI::GetTrayNormalImage()
{
	return m_trayNormalImage;
}

void CTrayListElementUI::SetTrayNormalImage(LPCTSTR pStrImage)
{
	m_trayNormalImage = pStrImage;
	if(m_pText)
		m_pText->SetBkImage(pStrImage);
}

LPCTSTR CTrayListElementUI::GetTrayHotImage()
{
	return m_trayHotImage;
}

void CTrayListElementUI::SetTrayHotImage(LPCTSTR pStrImage)
{
	m_trayHotImage = pStrImage;
}

LPCTSTR CTrayListElementUI::GetTrayDisabledImage()
{
	return m_trayDisabledImage;
}

void CTrayListElementUI::SetTrayDisabledImage(LPCTSTR pStrImage)
{
	m_trayDisabledImage = pStrImage;
	/*if(m_pText)
		m_pText->SetDis(dwColor);*/
}

void CTrayListElementUI::SetTrayText(LPCTSTR pstrName)
{
	m_trayText = pstrName;
	if(m_pText)
	{
		m_pText->SetText(pstrName);
		//m_pText->SetResourceText(pstrName);
	}
}

LPCTSTR CTrayListElementUI::GetTrayText()
{
	return m_trayText;
}

void CTrayListElementUI::SetTrayTextColor(DWORD dwColor)
{
	m_dwTrayTextColor = dwColor;
}

DWORD CTrayListElementUI::GetTrayTextColor() const
{
	return m_dwTrayTextColor;
}

void CTrayListElementUI::SetTrayTextHotColor(DWORD dwColor)
{
	m_dwTrayTextHotColor = dwColor;
}
	
DWORD CTrayListElementUI::GetTrayTextHotColor() const
{
	return m_dwTrayTextHotColor;
}

void CTrayListElementUI::SetTrayTextDisabledColor(DWORD dwColor)
{
	m_dwTrayTextDisabledColor = dwColor;
	if(m_pText)
		m_pText->SetDisabledTextColor(dwColor);
}

DWORD CTrayListElementUI::GetTrayTextDisabledColor() const
{
	return m_dwTrayTextDisabledColor;
}

void CTrayListElementUI::SetEnableElement(BOOL _bEnable)
{
	if(_bEnable)
	{
		if(m_pText)
			m_pText->SetBkImage(m_trayNormalImage);
	}
	else
	{
		if(m_pText)
			m_pText->SetBkImage(m_trayDisabledImage);
	}
	if(m_pText)
		m_pText->SetEnabled(B2b(_bEnable));

	m_bEnable = _bEnable;
	SetEnabled(B2b(_bEnable));
}