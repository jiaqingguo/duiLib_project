#pragma once

#include <Windows.h>
#include <iostream>
#include "Util/UtilTool.h"
//#include "UtilTool.h"


class DrawingInterfaceClass : public WindowImplBase
{
public:
	static int MessageBox(HWND hParent, vector<string> vec_wx, vector<string> vec_dqz, vector<string> vec_dmzd,vector<string> vec_wxNet, vector<string> vec_dqzNet, vector<string> vec_dmzdNet)
	{
		DrawingInterfaceClass* pWnd = new DrawingInterfaceClass();
		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW, 0, 0, 400, 250);
		pWnd->CenterWindow();
		SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//将界面这个界面显示在最上层

		pWnd->DrawTheInterface_All(vec_wx, vec_dqz, vec_dmzd);//将所有的卫星、地球站、地面终端绘制在界面上
		pWnd->LightUpTheDevice(vec_wxNet, vec_dqzNet, vec_dmzdNet);//将链路中的卫星、地球站、地面终端点亮

		//pWnd->SetOptical_fiber(vec);
		//pWnd->SettingCues(lpstrTitle);
		//pWnd->SetTestName(Test_Name);
		return pWnd->ShowModal();
	}

	DrawingInterfaceClass();
	~DrawingInterfaceClass(void);
	wstring stringToWstring(string str);
	string wstringToString(wstring wstr);

public:
	virtual void OnFinalMessage(HWND);
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void Notify(TNotifyUI &msg);
	void OnLClick(CControlUI *pControl);
	virtual void InitWindow();

	void SetOptical_fiber(vector<string> vec);//初始化给节点赋值
	void SettingCues(LPCTSTR lpstrTitle);

	void DrawTheInterface_All(vector<string> vec_wx, vector<string> vec_dqz, vector<string> vec_dmzd);//将所有的卫星、地球站、地面终端绘制在界面上
	void LightUpTheDevice(vector<string> vec_wx, vector<string> vec_dqz, vector<string> vec_dmzd);//将链路中的卫星、地球站、地面终端点亮
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
	CEditUI * m_CEditUI;

	vector<CButtonUI * > vec_wxIcon;//卫星图标
	vector<CButtonUI * > vec_wxArrowBelow;//卫星下面的箭头

	vector<CButtonUI * > vec_dqzArrowAbove;//地球站上面的箭头
	vector<CButtonUI * > vec_dqzIcon;//地球站图标
	vector<CButtonUI * > vec_dqzArrowBelow;//地球站下面的箭头

	vector<CButtonUI * > vec_dmzdArrowAbove;//地面终端上面的箭头
	vector<CButtonUI * > vec_dmzdIcon;//地面终端图标

	vector<string> vec_wxName;//卫星名称
	vector<string> vec_dqzName;//地球站名称
	vector<string> vec_dmzdName;//地面终端名称
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

};


