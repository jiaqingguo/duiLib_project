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
		SetWindowPos(pWnd->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//���������������ʾ�����ϲ�

		pWnd->DrawTheInterface_All(vec_wx, vec_dqz, vec_dmzd);//�����е����ǡ�����վ�������ն˻����ڽ�����
		pWnd->LightUpTheDevice(vec_wxNet, vec_dqzNet, vec_dmzdNet);//����·�е����ǡ�����վ�������ն˵���

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

	void SetOptical_fiber(vector<string> vec);//��ʼ�����ڵ㸳ֵ
	void SettingCues(LPCTSTR lpstrTitle);

	void DrawTheInterface_All(vector<string> vec_wx, vector<string> vec_dqz, vector<string> vec_dmzd);//�����е����ǡ�����վ�������ն˻����ڽ�����
	void LightUpTheDevice(vector<string> vec_wx, vector<string> vec_dqz, vector<string> vec_dmzd);//����·�е����ǡ�����վ�������ն˵���
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

	vector<CButtonUI * > vec_wxIcon;//����ͼ��
	vector<CButtonUI * > vec_wxArrowBelow;//��������ļ�ͷ

	vector<CButtonUI * > vec_dqzArrowAbove;//����վ����ļ�ͷ
	vector<CButtonUI * > vec_dqzIcon;//����վͼ��
	vector<CButtonUI * > vec_dqzArrowBelow;//����վ����ļ�ͷ

	vector<CButtonUI * > vec_dmzdArrowAbove;//�����ն�����ļ�ͷ
	vector<CButtonUI * > vec_dmzdIcon;//�����ն�ͼ��

	vector<string> vec_wxName;//��������
	vector<string> vec_dqzName;//����վ����
	vector<string> vec_dmzdName;//�����ն�����
public:
	vector<string> Ptempdata;
	int PSatelliteNum;
	static unsigned __stdcall Busys(void* arglist);

};


