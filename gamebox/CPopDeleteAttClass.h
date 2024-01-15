#pragma once

#include <Windows.h>
#include <iostream>
#include "Util/UtilTool.h"
//#include "UtilTool.h"


class CPopDeleteAttClass : public WindowImplBase
{
public:
	static int MessageBox(HWND hParent, vector<string> vec)
	{
		CPopDeleteAttClass* pWnd = new CPopDeleteAttClass();
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW, 0, 0, 400, 250);
		pWnd->CenterWindow();
		SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//将界面这个界面显示在最上层

		pWnd->SetOptical_fiber(vec);
		//pWnd->SettingCues(lpstrTitle);
		//pWnd->SetTestName(Test_Name);
		return pWnd->ShowModal();
	}

	CPopDeleteAttClass();
	~CPopDeleteAttClass(void);

	string UTF8_To_string(const std::string & str);
	string string_To_UTF8(const std::string & str);
	vector<string> GetDeleteName();//获取要移动到的节点名称

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	void OnLClick(CControlUI *pControl);
	virtual void InitWindow();

	void SetOptical_fiber(vector<string> vec);//初始化给节点赋值
	void SettingCues(LPCTSTR lpstrTitle);
	void SettingCues();
	virtual string get_cur_timelogdata();

	DUI_DECLARE_MESSAGE_MAP()
		virtual void OnClick(TNotifyUI& msg);
	virtual void OnSelectChanged(TNotifyUI &msg);
	virtual void OnItemSelect(TNotifyUI &msg);

	static vector<string> m_DeleteName;//选择的节点名称


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

	string StoreOptions[300];//存放option控件的数组
	vector<string> m_option;//显示出来的option
	vector<string> m_optionValue;//显示出来的所有option的内容

public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

};
