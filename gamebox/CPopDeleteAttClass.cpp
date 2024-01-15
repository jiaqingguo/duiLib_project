#include "stdafx.h"
#include "CPopDeleteAttClass.h"

#include <sstream>

LPCTSTR m_ReceivedPrompts;//接收到的提示语
#pragma execution_character_set("utf-8") ;
vector<string> CPopDeleteAttClass::m_DeleteName;
//vector<string> seleceColumns;
//网络管理界面类实现
DUI_BEGIN_MESSAGE_MAP(CPopDeleteAttClass, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CPopDeleteAttClass::CPopDeleteAttClass()
{
	//m_ReceivedPrompts = lpstrTitle;
	////m_newName = "";
}

CPopDeleteAttClass::~CPopDeleteAttClass(void)
{
	//m_newName = "";


}

string CPopDeleteAttClass::UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;

}
string CPopDeleteAttClass::string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

void CPopDeleteAttClass::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


DuiLib::CDuiString CPopDeleteAttClass::GetSkinFile()
{
	return _T("DeleteAttribute.xml");
}

LPCTSTR CPopDeleteAttClass::GetWindowClassName(void) const
{
	return _T("SelectLevel1");
}
void CPopDeleteAttClass::SetOptical_fiber(vector<string> vec)
{
	if (vec.size() <= 0) return;

	for (int i = 0; i < vec.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions[i] = "optical_fiber" + streamsssss.str();
		UtilTool::setWstring(w, UTF8_To_string(vec[i]).c_str());
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
}
void CPopDeleteAttClass::OnClick(TNotifyUI &msg)
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
				//m_NodeName = m_optionValue[i];
				m_DeleteName.push_back(m_optionValue[i]);
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

void CPopDeleteAttClass::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopDeleteAttClass::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopDeleteAttClass::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT CPopDeleteAttClass::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  CPopDeleteAttClass::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void CPopDeleteAttClass::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void CPopDeleteAttClass::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CPopDeleteAttClass::Busys(void* param)
{

	CPopDeleteAttClass * pWnd = (CPopDeleteAttClass*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


void CPopDeleteAttClass::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_newPage")));
	m_CEditUI = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name")));

	m_DeleteName.clear();
	//SettingCues();
	//SettingCues();

}

void CPopDeleteAttClass::SettingCues(LPCTSTR lpstrTitle)
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(lpstrTitle);
	m_CEditUI->SetText(lpstrTitle);
}
///
void CPopDeleteAttClass::SettingCues()
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(m_ReceivedPrompts);
	//m_CEditUI->SetText(m_ReceivedPrompts);

}
//获取修改后的名称
vector<string> CPopDeleteAttClass::GetDeleteName()
{
	return m_DeleteName;
}
