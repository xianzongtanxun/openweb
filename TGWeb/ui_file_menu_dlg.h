#ifndef _UI_FILE_MENU_H_
#define _UI_FILE_MENU_H_

class CTrayListElementUI;
class CMainFrame;
class CFileMenuDlg : public WindowImplBase {
public:
	CFileMenuDlg(CMainFrame* pMain);
	~CFileMenuDlg();
	void InitWindow();
	HWND Create(HWND hwnd);
	virtual LPCTSTR GetWindowClassName() const { return _T("UI_FILE_MENU_DLG"); }
	void Notify(TNotifyUI& msg);
	CDuiString GetSkinFile() { return _T("menu_file.xml"); }
	CDuiString GetSkinFolder() { return _T("skin"); }
	UILIB_RESOURCETYPE GetResourceType() const {
		return UILIB_FILE;
	}
	virtual void OnFinalMessage(HWND hWnd){
		WindowImplBase::OnFinalMessage(hWnd);
		delete this;
	}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	void AdjustPos(RECT& rc, POINT pt);
	void ShowWnd();
private:
	POINT m_ptPos;
	ShadowHelp m_shadowHelp;
	CTrayListElementUI* m_pBtnOpen;
	CTrayListElementUI*	m_pBtnSave;
	CTrayListElementUI*	m_pBtnSaveAs;
	CTrayListElementUI*	m_pBtnPrint;
	CMainFrame* m_pMainFrame;
};

#endif // !_UI_FILE_MENU_H_
