#include "StdAfx.h"
#include "PopWnds.h"
#include <ShellAPI.h>
#include <process.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "UtilTool.h"
#include "direct.h"

//////////////////////////////////////////////////////////////////////////
///
//vector<string> PreplyData;
//vector<string> PreplyDataTime;
bool data_save;
//int data_save_en;

DUI_BEGIN_MESSAGE_MAP(CPopWnds, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT,OnItemSelect)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
	DUI_END_MESSAGE_MAP()

	CPopWnds::CPopWnds(void)
{
}

CPopWnds::~CPopWnds(void)
{
}

void CPopWnds::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CPopWnds::GetSkinFile()
{
	return _T("Loading.xml");
}

LPCTSTR CPopWnds::GetWindowClassName( void ) const
{
	return _T("PopWnds");
}

void CPopWnds::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if( msg.pSender == m_pCloseBtn ) {
		Close(0);
		return; 
	}
	else if( msg.pSender == m_pMinBtn ) { 
		//CMsgWnd::MessageBox(m_hWnd, NULL, _T("子子窗口"));

		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
	else if( msg.pSender == m_pMaxBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
	else if( msg.pSender == m_pRestoreBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
	else if( msg.pSender == m_pMenuBtn ) {
	}
	else if(sName.CompareNoCase(_T("homepage_btn")) == 0)
	{
		ShellExecute(NULL, _T("open"), _T("https://github.com/qdtroy"), NULL, NULL, SW_SHOW);
	}
}

void CPopWnds::OnSelectChanged( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopWnds::OnItemSelect( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopWnds::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CPopWnds::Notify( TNotifyUI &msg )
{
	return WindowImplBase::Notify(msg);
}

LRESULT CPopWnds::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if( ::IsZoomed(*this) != bZoomed ) {
		if( !bZoomed ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return lRes;
}



string  CPopWnds::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100]={0};
	GetLocalTime(&stime);
	sprintf(buf,"%04d.%02d.%02d.%02d.%02d.%02d",stime.wYear,stime.wMonth,stime.wDay,stime.wHour,stime.wMinute,stime.wSecond);
	time_string+=buf;
	return time_string;
}

unsigned __stdcall CPopWnds:: Busyss( void* param )  
{  
	
	FILE * lOG;
	string logdata,path1,path2;
	vector<string> replyDatas,datatime;
	string newname,tempstr,tempstr1;
	CPopWnds * pWnd = (CPopWnds*)param;
	while(1)
	{
		if (data_save==false)
		{
			::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
		Sleep(6);
	}
	
	
	
	return 0;
}  

void CPopWnds::InitWindow()
{
	
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
	m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
	m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
	m_pMenuBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("menubtn")));
	
	CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Busyss, this,0,NULL);

	//_beginthread(Busys, 0, m_hWnd);
}
