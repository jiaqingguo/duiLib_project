#include "stdafx.h"
#include "CPopNewAttributeClass.h"

#include <sstream>

LPCTSTR m_ReceivedPrompts;//接收到的提示语

vector<string> CPopNewAttributeClass::m_vecNewName;
//vector<string> seleceColumns;
//网络管理界面类实现
DUI_BEGIN_MESSAGE_MAP(CPopNewAttributeClass, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CPopNewAttributeClass::CPopNewAttributeClass()
{
	//m_ReceivedPrompts = lpstrTitle;
	////m_newName = "";
}

CPopNewAttributeClass::~CPopNewAttributeClass(void)
{
	//m_newName = "";


}

void CPopNewAttributeClass::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


DuiLib::CDuiString CPopNewAttributeClass::GetSkinFile()
{
	return _T("NewAttribute.xml");
}

LPCTSTR CPopNewAttributeClass::GetWindowClassName(void) const
{
	return _T("ClueScreen");
}

void CPopNewAttributeClass::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		CDuiString folderName = m_pm.FindControl(L"edit_name")->GetText();
		string str;
		UtilTool::setString(str, folderName.GetData());
		m_vecNewName.push_back(str);
		CDuiString Value = m_pm.FindControl(L"edit_value")->GetText();
		string str_value;
		UtilTool::setString(str_value, Value.GetData());
		m_vecNewName.push_back(str_value);
		//m_newName = str;

		Close(1);
	}
	else if (sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close(0);
	}
	else if (sName.CompareNoCase(_T("Close")) == 0)
	{
		Close(0);
	}



}

void CPopNewAttributeClass::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopNewAttributeClass::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopNewAttributeClass::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT CPopNewAttributeClass::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  CPopNewAttributeClass::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void CPopNewAttributeClass::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void CPopNewAttributeClass::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CPopNewAttributeClass::Busys(void* param)
{

	CPopNewAttributeClass * pWnd = (CPopNewAttributeClass*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


void CPopNewAttributeClass::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_newPage")));
	m_CEditUI = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name")));

	m_vecNewName.clear();
	//SettingCues();
	//SettingCues();

}

void CPopNewAttributeClass::SettingCues(LPCTSTR lpstrTitle)
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(lpstrTitle);
	m_CEditUI->SetText(lpstrTitle);
}
///
void CPopNewAttributeClass::SettingCues()
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(m_ReceivedPrompts);
	//m_CEditUI->SetText(m_ReceivedPrompts);

}
//获取修改后的名称
vector<string> CPopNewAttributeClass::GetNewName()
{
	return m_vecNewName;
}
void CPopNewAttributeClass::SetNewNameEmpty()
{
	m_vecNewName.clear();
}