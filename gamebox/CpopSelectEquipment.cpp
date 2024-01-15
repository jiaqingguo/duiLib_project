#include "stdafx.h"
#include "CpopSelectEquipment.h"

#include <sstream>

#pragma execution_character_set("utf-8") ;
LPCTSTR m_ReceivedPrompts;//接收到的提示语

vector<string> CpopSelectEquipment::m_selectName;
//vector<string> seleceColumns;
//每种类型可以是159个，总共可以弄10种类型
DUI_BEGIN_MESSAGE_MAP(CpopSelectEquipment, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CpopSelectEquipment::CpopSelectEquipment()
{
	//m_ReceivedPrompts = lpstrTitle;
	////m_newName = "";
}

CpopSelectEquipment::~CpopSelectEquipment(void)
{
	//m_newName = "";


}

void CpopSelectEquipment::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


DuiLib::CDuiString CpopSelectEquipment::GetSkinFile()
{
	return _T("SelectEquipment_1.xml");
}

wstring  CpopSelectEquipment::stringToWstring(string str)
{

	wstring w_str;
	UtilTool::setWstring(w_str, str.c_str());
	return w_str;


}

string CpopSelectEquipment::UTF8_To_string(const std::string & str)
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
//wstring转string
string  CpopSelectEquipment::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}

LPCTSTR CpopSelectEquipment::GetWindowClassName(void) const
{
	return _T("SelectLevel1");
}
void CpopSelectEquipment::SetOptical_fiber(vector<vector<string>> vec_vecName)
{
	if (vec_vecName.size() <= 0) return;

	for (int i = 0; i < vec_vecName.size(); i++)
	{
		
		std::ostringstream num_Hori;
		num_Hori << i+1;
		string str_Hori = "Verti_" + num_Hori.str();
		CVerticalLayoutUI * m_pMainWndContent = dynamic_cast<CVerticalLayoutUI*>(m_pm.FindControl(stringToWstring(str_Hori).c_str()));
		vector<COptionUI * > vec_COption;
		for (int j = 0; j < vec_vecName[i].size(); j++)
		{

			COptionUI * option1 = new COptionUI();

			option1->SetFixedHeight(30);
			option1->SetFixedWidth(150);
			option1->SetBkColor(0xFFFFFFFF);
			option1->SetTextColor(0xFF000000);
			option1->SetText(stringToWstring(vec_vecName[i][j]).c_str());
			option1->SetNormalImage(_T("res = 'common/cb_normal.png'  dest='0,0,16.16'"));
			option1->SetHotImage(_T("res = 'common/cb_hot.png' dest = '0,0,16.16'"));
			option1->SetPushedImage(_T("res = 'res = 'common/cb_pushed.png' dest = '0,0,16.16'"));
			option1->SetSelectedImage(_T("res = 'common/cb_selected.png' dest = '0,0,16,16'"));
			m_pMainWndContent->Add(option1);
			vec_COption.push_back(option1);
		}
		m_optionUI.push_back(vec_COption);
		num_Hori.clear(); //clear flags.当多次转换时，必须先调这个clear()
		num_Hori.str("");

	}
}
void CpopSelectEquipment::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}
	else if (sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		m_selectName.clear();

		for (int i = 0; i < m_optionUI.size(); i++)
		{
			for (int j = 0; j < m_optionUI[i].size(); j++)
			{
				COptionUI* pControl = m_optionUI[i][j];
				if (pControl->IsSelected())
				{
					wstring wstr;
					string str;
					wstr = pControl->GetText().GetData();
					UtilTool::setString(str, wstr.c_str());
					m_selectName.push_back(str);

				}
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
	//


}

void CpopSelectEquipment::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CpopSelectEquipment::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CpopSelectEquipment::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT CpopSelectEquipment::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  CpopSelectEquipment::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void CpopSelectEquipment::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void CpopSelectEquipment::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CpopSelectEquipment::Busys(void* param)
{

	CpopSelectEquipment * pWnd = (CpopSelectEquipment*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


void CpopSelectEquipment::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_newPage")));
	m_CEditUI_1 = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name1")));
	m_CEditUI_2 = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name2")));
	m_CEditUI_3 = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name3")));
	m_CEditUI_4 = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name4")));
	m_CEditUI_5 = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name5")));

	m_selectName.clear();

	//CHorizontalLayoutUI * m_pMainWndContent = dynamic_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("hori_1")));

	//COptionUI * option2[100];
	//for (int i = 0; i < 100; i++)
	//{
	//	option2[i] = new COptionUI();

	//	option2[i]->SetFixedHeight(30);
	//	option2[i]->SetFixedWidth(100);
	//	option2[i]->SetBkColor(0xFFFFFFFF);
	//	option2[i]->SetTextColor(0xFF000000);
	//	option2[i]->SetText(_T("llllll"));
	//	option2[i]->SetNormalImage(_T("res = 'common/cb_normal.png'  dest='0,0,16.16'"));
	//	option2[i]->SetHotImage(_T("res = 'common/cb_hot.png' dest = '0,0,16.16'"));
	//	option2[i]->SetPushedImage(_T("res = 'res = 'common/cb_pushed.png' dest = '0,0,16.16'"));
	//	option2[i]->SetSelectedImage(_T("res = 'common/cb_selected.png' dest = '0,0,16,16'"));
	//	m_pMainWndContent->Add(option2[i]);
	//}

	//CHorizontalLayoutUI * m_pMainWndContent_2 = dynamic_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("hori_2")));

	//COptionUI * option3[100];
	//for (int i = 0; i < 100; i++)
	//{
	//	option3[i] = new COptionUI();

	//	option3[i]->SetFixedHeight(30);
	//	option3[i]->SetFixedWidth(100);
	//	option3[i]->SetBkColor(0xFFFFFFFF);
	//	option3[i]->SetTextColor(0xFF000000);
	//	option3[i]->SetText(_T("22222"));
	//	option3[i]->SetNormalImage(_T("res = 'common/cb_normal.png'  dest='0,0,16.16'"));
	//	option3[i]->SetHotImage(_T("res = 'common/cb_hot.png' dest = '0,0,16.16'"));
	//	option3[i]->SetPushedImage(_T("res = 'res = 'common/cb_pushed.png' dest = '0,0,16.16'"));
	//	option3[i]->SetSelectedImage(_T("res = 'common/cb_selected.png' dest = '0,0,16,16'"));
	//	m_pMainWndContent_2->Add(option3[i]);
	//}




	//SettingCues();
	//SettingCues();

}

void CpopSelectEquipment::SettingCues(vector<string> vec_type)
{
	string StoreOptions_1[300];
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(lpstrTitle);
	for (int i = 0; i < vec_type.size(); i++)
	{
		wstring w, kj;
		std::ostringstream streamsssss;
		streamsssss << (i + 1);
		StoreOptions[i] = "edit_name" + streamsssss.str();
		UtilTool::setWstring(w, UTF8_To_string(vec_type[i]).c_str());
		//UtilTool::setWstring(w, vec_type[i].c_str());
		UtilTool::setWstring(kj, StoreOptions[i].c_str());
		//StoreOptions[]
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
		pControl->SetText(w.c_str());
		pControl->SetVisible(TRUE);

		wstring w1, kj1;
		StoreOptions_1[i] = "label" + streamsssss.str();
		UtilTool::setWstring(kj1, StoreOptions_1[i].c_str());
		CControlUI* pControl_1 = static_cast<CControlUI*>(m_pm.FindControl(kj1.c_str()));
		pControl_1->SetVisible(TRUE);

		streamsssss.clear(); //clear flags.当多次转换时，必须先调这个clear()
		streamsssss.str("");
	}

	//m_CEditUI_1->SetText(lpstrTitle);
}
///
void CpopSelectEquipment::SettingCues()
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(m_ReceivedPrompts);
	//m_CEditUI->SetText(m_ReceivedPrompts);

}
//获取修改后的名称
vector<string> CpopSelectEquipment::GetNodeName()
{
	return m_selectName;
}

