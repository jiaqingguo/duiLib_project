#pragma once

#include <Windows.h>
#include <iostream>
#include "Util/UtilTool.h"
//#include "UtilTool.h"


class CPopNewAttributeClass : public WindowImplBase
{
public:
	static int MessageBox(HWND hParent)
	{
		CPopNewAttributeClass* pWnd = new CPopNewAttributeClass();
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW, 0, 0, 400, 250);
		pWnd->CenterWindow();
		SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//将界面这个界面显示在最上层
		//pWnd->SettingCues(lpstrTitle);
		//pWnd->SetTestName(Test_Name);
		return pWnd->ShowModal();
	}

	CPopNewAttributeClass();
	~CPopNewAttributeClass(void);

	vector<string> GetNewName();//获取修改后的名称
	void SetNewNameEmpty();//将m_newName设置为空
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

	static vector<string> m_vecNewName;//修改后的节点名称


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
	CEditUI * m_CEditUI;
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

};


