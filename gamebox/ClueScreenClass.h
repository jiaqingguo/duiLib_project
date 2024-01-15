#pragma once



#include <iostream>
#include "UtilTool.h"

//网络管理界面
class ClueScreenClass : public WindowImplBase
{
public:
	ClueScreenClass(LPCTSTR lpstrTitle);
	~ClueScreenClass(void);

	//static int MessageBox(HWND hParent, LPCTSTR lpstrTitle)
	//{
	//	ClueScreenClass* pWnd = new ClueScreenClass();
	//	pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	//	//pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	//	pWnd->CenterWindow();
	//	pWnd->ShowModal();
	//	//ClueScreenClass* pWnd = new ClueScreenClass();
	//	//pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	//	////pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW,100,100,0,0);
	//	////pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	//	////pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	//	////pWnd->CenterWindow();
	//	////SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//将界面这个界面显示在最上层
	//	//SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//将界面这个界面显示在最上层
	//	//pWnd->SettingCues(lpstrTitle);
	//	//pWnd->ShowModal();
	//	//return 0;
	//}

	//if (WAIT_OBJECT_0 == ::WaitForSingleObject(g_hEventActive, 0))
	//{
	//	::ResetEvent(g_hEventActive);
	//	ShowWindow(); ///< 先显示出来

	//	/// 要求前置, 又不能是最顶层窗口
	//	::SetWindowPos(this->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//	::SetWindowPos(this->GetHWND(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//}

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	void OnLClick(CControlUI *pControl);
	virtual void InitWindow();

	void SettingCues(LPCTSTR lpstrTitle);
	void SettingCues();
	virtual string get_cur_timelogdata();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemSelect(TNotifyUI &msg);



	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
	{
		if (uMsg == WM_KEYDOWN)
		{
			switch (wParam)
			{
			case VK_RETURN:
			case VK_ESCAPE:
				return ResponseDefaultKeyEvent(wParam);
			default:
				break;
			}
		}
		return FALSE;
	}


private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pMenuBtn;
	CTabLayoutUI* pTabDatabase;
	CLabelUI* m_CLabelUI;
	
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

};



