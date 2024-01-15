#include "stdafx.h"
#include "CpopSelectLevel1.h"

#include <sstream>

LPCTSTR m_ReceivedPrompts;//接收到的提示语

string CpopSelectLevel1::m_NodeName;
//vector<string> seleceColumns;
//网络管理界面类实现
DUI_BEGIN_MESSAGE_MAP(CpopSelectLevel1, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CpopSelectLevel1::CpopSelectLevel1()
{
	//m_ReceivedPrompts = lpstrTitle;
	////m_newName = "";
}

CpopSelectLevel1::~CpopSelectLevel1(void)
{
	//m_newName = "";


}

void CpopSelectLevel1::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


DuiLib::CDuiString CpopSelectLevel1::GetSkinFile()
{
	return _T("SelectLevel1.xml");
}

LPCTSTR CpopSelectLevel1::GetWindowClassName(void) const
{
	return _T("SelectLevel1");
}
void CpopSelectLevel1::SetOptical_fiber(vector<string> vec)
{
	if (vec.size() <= 0) return;

	for (int i = 0; i < vec.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions[i] = "optical_fiber" + streamsssss.str();
		UtilTool::setWstring(w, vec[i].c_str());
		UtilTool::setWstring(kj, StoreOptions[i].c_str());
		//StoreOptions[]
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetText(w.c_str());
		pControl->SetVisible(TRUE);
		m_option.push_back(StoreOptions[i]);
		m_optionValue.push_back(vec[i]);

		streamsssss.clear(); //clear flags.当多次转换时，必须先调这个clear()
		streamsssss.str("");
	}

	//for(int i = 0;i < vec.size(); i++)
	//{
	//	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(StoreOptions[i].c_str()));
	//}
	//string str = "optical_fiber2";

	//UtilTool::setWstring(w,vec[0].c_str());
	//UtilTool::setWstring(kj,str.c_str());
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
	////CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageTitle")));
	//if( pControl )pControl->SetText(w.c_str());
	//pControl->SetVisible(TRUE);
	//if( pControl ) pControl->SetText(lpstrMsg);
}
void CpopSelectLevel1::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{

		for (int i = 0; i < m_option.size(); i++)
		{
			wstring kj;

			UtilTool::setWstring(kj, StoreOptions[i].c_str());
			//StoreOptions[]
			COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(kj.c_str()));
			if (pControl->IsSelected())
			{
				m_NodeName = m_optionValue[i];
			}
		}


		Close(1);
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close(0);
	}
	else if (sName.CompareNoCase(_T("Close")) == 0)
	{
		Close(1);
	}


}

void CpopSelectLevel1::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CpopSelectLevel1::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CpopSelectLevel1::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT CpopSelectLevel1::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  CpopSelectLevel1::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void CpopSelectLevel1::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void CpopSelectLevel1::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CpopSelectLevel1::Busys(void* param)
{

	CpopSelectLevel1 * pWnd = (CpopSelectLevel1*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


void CpopSelectLevel1::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_newPage")));
	m_CEditUI = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name")));

	m_NodeName = "";
	//SettingCues();
	//SettingCues();

}

void CpopSelectLevel1::SettingCues(LPCTSTR lpstrTitle)
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(lpstrTitle);
	m_CEditUI->SetText(lpstrTitle);
}
///
void CpopSelectLevel1::SettingCues()
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(m_ReceivedPrompts);
	//m_CEditUI->SetText(m_ReceivedPrompts);

}
//获取修改后的名称
string CpopSelectLevel1::GetNodeName()
{
	return m_NodeName;
}

