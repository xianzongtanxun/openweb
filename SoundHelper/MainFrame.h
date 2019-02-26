#ifndef _MAIN_DLG_H_
#define _MAIN_DLG_H_

#include "simple_handler.h"
#define  WM_MSG_SHOW  3800

static HWND g_hwnd;
class CMainFrame : public WindowImplBase
{
public:
	CMainFrame();
	~CMainFrame();
	static LPCTSTR GetCaption(){ return _T("MainSound"); }
	HWND Create(HWND hwndParent, LPCTSTR pstrName);
	static HWND g_HWnd;
public:
	DuiLib::CDuiString GetSkinFile();
    void InitWindow();
	UILIB_RESOURCETYPE GetResourceType() const;
	CDuiString GetSkinFolder();
	UINT GetClassStyle() const;
	virtual LPCTSTR GetWindowClassName(void)const{
		return _T("MainSound");
	};
protected:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(DuiLib::TNotifyUI& msg);
private:
	ShadowHelp m_ShaowHelp;
	CEditUI* m_edtFilePath;
	CButtonUI *m_pBtnBackup;
	CButtonUI *m_pBtnForword;
	CButtonUI *m_pBtnGo;
	CButtonUI *m_pBtnChangePath;
	CButtonUI *m_pBtnClose;
	CWebBrowserUI* m_pVlyFrame1;
	CVerticalLayoutUI* m_pVlyFrame;
	CefRefPtr<SimpleHandler> m_handler;
};
#endif // !_MAIN_DLG_H_