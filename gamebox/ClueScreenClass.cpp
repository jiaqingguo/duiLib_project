#include "stdafx.h"
#include "ClueScreenClass.h"


#include <sstream>

LPCTSTR m_ReceivedPrompts;//接收到的提示语
//vector<string> seleceColumns;
//网络管理界面类实现
DUI_BEGIN_MESSAGE_MAP(ClueScreenClass, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
ClueScreenClass::ClueScreenClass(LPCTSTR lpstrTitle)
{
	m_ReceivedPrompts = lpstrTitle;

}

ClueScreenClass::~ClueScreenClass(void)
{



}

void ClueScreenClass::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


DuiLib::CDuiString ClueScreenClass::GetSkinFile()
{
	return _T("ClueScreen.xml");
}

LPCTSTR ClueScreenClass::GetWindowClassName(void) const
{
	return _T("ClueScreen");
}

void ClueScreenClass::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{

		Close(1);
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close(0);
	}



}

void ClueScreenClass::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void ClueScreenClass::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT ClueScreenClass::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT ClueScreenClass::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if (wParam == SC_CLOSE) {
		::PostQuitMessage(0L);
		bHandled = TRUE;
		return 0;
	}
	BOOL bZoomed = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(*this) != bZoomed) {
		if (!bZoomed) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(true);
		}
		else {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if (pControl) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if (pControl) pControl->SetVisible(false);
		}
	}
	return lRes;
}


string  ClueScreenClass::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void ClueScreenClass::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void ClueScreenClass::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	return WindowImplBase::Notify(msg);
}
unsigned __stdcall ClueScreenClass::Busys(void* param)
{

	ClueScreenClass * pWnd = (ClueScreenClass*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


void ClueScreenClass::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_newPage")));
	m_CLabelUI = static_cast<CLabelUI*>(m_pm.FindControl(_T("lable_Statement")));
	//SettingCues();
	SettingCues();

}

void ClueScreenClass::SettingCues(LPCTSTR lpstrTitle)
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	m_CLabelUI->SetText(lpstrTitle);
	
}
///
void ClueScreenClass::SettingCues()
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	m_CLabelUI->SetText(m_ReceivedPrompts);

}