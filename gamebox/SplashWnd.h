#pragma once
//#include "Chart.h"
#include "WndUI.h"
//#include "MysqlAdmin.h"
#include "AddTest.h"
#include "io.h"
#include "direct.h"
#include <iostream>  
#include <vector>  

#include <memory>
//#include "Structural.h"
//#include "UDP.h"
//#include "UdpRecvClass.h"

#include "CPopLinkManage.h"
#include "CPopNetworkManage.h"
#include "CPopDataManage.h"
#include "CPopEditorialClass.h"
//////////////////////////////////////////////////////////////////////////
///
extern CDuiString ZTest_Name;
extern CDuiString ZDLX_Name;
extern bool option_zhT, option_dlx;
class ConnectMysql;


class CSplashWnd : public WindowImplBase, public CWebBrowserEventHandler
{
public:
	static int MessageBox(HWND hParent)
	{
		CSplashWnd* pWnd = new CSplashWnd();

		pWnd->Create(hParent, _T("msgwnd"), WS_POPUP | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
		//pWnd->Create(hParent, _T("msgwnd"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
		pWnd->CenterWindow();
		return pWnd->ShowModal();
	}
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("Wnd")) == 0)
		{
			CWndUI  *ppUI = new CWndUI;
			//CChartWnd * m_ChartWnd = new CChartWnd();
			//ppUI->SetChartWnd(m_ChartWnd);
			HWND hWnd = CreateWindow(_T("Button"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 0, 0, 0, m_pm.GetPaintWindow(), NULL, NULL, NULL);
			ppUI->Attach(hWnd);
			//m_ChartWnd->Attach(hWnd);

			//CChart * chart = m_ChartWnd->GetChart();
			double pX[100], pY[100];

			for (int i = 0; i < 100; i++) {
				pX[i] = i;
				pY[i] = i;

			}
			//chart->AddCurve(pX, pY, 100);


			return ppUI;
		}
		return NULL;
	}

	//void recDataFun(unsigned char *rec, int len);
	//void DataJX();
public:
	CSplashWnd(void);
	~CSplashWnd(void);
	
public:

	CTabLayoutUI* pTabDatabase;//pTabSwitchs
	CControlUI* pTabPanels;
	CComboBoxUI  * select_combox;	
	CButtonUI * ZLState;
	CEditUI * serialInput;


	HANDLE m_recDataThread;



	bool DATASAVE;

	string parameters[100];
	vector<string> Readtxtdata;
	//MysqlPool mps;
	FILE * Cardata;
	CListUI* pList;


	virtual void OnFinalMessage( HWND );
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName( void ) const;
	virtual void InitWindow();

	

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);



	DUI_DECLARE_MESSAGE_MAP()
	/*virtual void OnClick(TNotifyUI& msg);*/
	void Notify(TNotifyUI& msg);
	void OnLClick(CControlUI *pControl);



	
	void getFiles(string path, vector<string>& files);
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void StringtoHex(BYTE *GB, int glen, BYTE* SendB, int* slen);
	static unsigned __stdcall ThreadSaveData(void* arglist);
	void appendMsg(const char* msg);//添加消息

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void onError(const char* msg);

private:


	CListUI*	m_state;//界面上显示状态的控件
	CListUI*	m_msg;//界面上显示消息的控件

	unsigned char unsgndChar_SCJSL[10];//陀螺仪通信协议一 输出角速率

	//bool m_isData;//串口接收数据标志

	int SelectListLine;//选择List的某一行


	CPopLinkManage* m_CPopLinkManage;
	CPopNetworkManage* m_CPopNetworkManage;
	CPopDataManage* m_CPopDataManage;
	CPopEditorialClass* m_CPopEditorialClass;
};
