#include "stdafx.h"
#include "DrawingInterfaceClass.h"



#include <sstream>



//vector<string> seleceColumns;
//网络管理界面类实现
DUI_BEGIN_MESSAGE_MAP(DrawingInterfaceClass, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
DrawingInterfaceClass::DrawingInterfaceClass()
{
	//m_ReceivedPrompts = lpstrTitle;
	////m_newName = "";
}

DrawingInterfaceClass::~DrawingInterfaceClass(void)
{
	//m_newName = "";


}
wstring DrawingInterfaceClass::stringToWstring(string str)
{

	wstring w_str;
	UtilTool::setWstring(w_str, str.c_str());
	return w_str;


}
//wstring转string
string DrawingInterfaceClass::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}
void DrawingInterfaceClass::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}


DuiLib::CDuiString DrawingInterfaceClass::GetSkinFile()
{
	return _T("DrawingInterfaceClass.xml");//DrawingInterfaceClass_perLine.xml  DrawingInterfaceClass_group.xml
}

LPCTSTR DrawingInterfaceClass::GetWindowClassName(void) const
{
	return _T("SelectLevel1");
}
void DrawingInterfaceClass::SetOptical_fiber(vector<string> vec)
{
	if (vec.size() <= 0) return;

	//for (int i = 0; i < vec.size(); i++)
	//{
	//	wstring w, kj;
	//	std::ostringstream streamsssss;
	//	streamsssss << (i + 1);
	//	StoreOptions[i] = "optical_fiber" + streamsssss.str();
	//	UtilTool::setWstring(w, vec[i].c_str());
	//	UtilTool::setWstring(kj, StoreOptions[i].c_str());
	//	//StoreOptions[]
	//	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(kj.c_str()));
	//	pControl->SetText(w.c_str());
	//	pControl->SetVisible(TRUE);
	//	m_option.push_back(StoreOptions[i]);
	//	m_optionValue.push_back(vec[i]);

	//	streamsssss.clear(); //clear flags.当多次转换时，必须先调这个clear()
	//	streamsssss.str("");
	//}

	
}
void DrawingInterfaceClass::OnClick(TNotifyUI &msg)
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
	else if (sName.CompareNoCase(_T("Close")) == 0)
	{
		Close(1);
	}


}

void DrawingInterfaceClass::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void DrawingInterfaceClass::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT DrawingInterfaceClass::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT DrawingInterfaceClass::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  DrawingInterfaceClass::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void DrawingInterfaceClass::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void DrawingInterfaceClass::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	return WindowImplBase::Notify(msg);
}
unsigned __stdcall DrawingInterfaceClass::Busys(void* param)
{

	DrawingInterfaceClass * pWnd = (DrawingInterfaceClass*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}

//初始化
void DrawingInterfaceClass::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	pTabDatabase = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_newPage")));
	m_CEditUI = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_name")));
	/*****************************一个组******************************/
	//CHorizontalLayoutUI * m_pMainWndContent = dynamic_cast<CHorizontalLayoutUI*>(m_pm.FindControl(stringToWstring("horizontal_WX").c_str()));
	//vector<COptionUI * > vec_COption;

	//CVerticalLayoutUI * m_pMainVer = new CVerticalLayoutUI();//VerticalLayout
	//m_pMainVer->SetFixedWidth(50);
	//m_pMainVer->SetVisible(true);

	//CButtonUI * button1 = new CButtonUI();
	//button1->SetFixedHeight(50);
	//button1->SetFixedWidth(50);
	//button1->SetTextColor(0xFF000000);
	//button1->SetForeImage(_T("ui/卫星_白色.png"));
	//m_pMainVer->Add(button1);

	//CLabelUI * lable = new CLabelUI();
	//lable->SetFixedHeight(30);
	//lable->SetFixedWidth(50);
	//lable->SetTextColor(0xFFFFFFFF);
	//lable->SetAttribute(_T("wordbreak"),_T("true"));
	//lable->SetText(_T("卫星1-----1"));

	//m_pMainVer->Add(lable);

	//CButtonUI * button2 = new CButtonUI();
	//button2->SetFixedHeight(50);
	//button2->SetFixedWidth(50);
	//button2->SetTextColor(0xFF000000);
	//button2->SetForeImage(_T("ui/上下箭头灭_白色.png"));
	//m_pMainVer->Add(button2);

	//m_pMainWndContent->Add(m_pMainVer);

	/***********************依附于已有Horizontal之上 创建了两个button 中间有空隙*****************************/

	//CHorizontalLayoutUI * m_pMainWndContent = dynamic_cast<CHorizontalLayoutUI*>(m_pm.FindControl(stringToWstring("horizontal_WX").c_str()));
	//vector<COptionUI * > vec_COption;

	//CButtonUI * button1 = new CButtonUI();
	//button1->SetFixedHeight(50);
	//button1->SetFixedWidth(50);
	//button1->SetTextColor(0xFF000000);
	//button1->SetForeImage(_T("ui/卫星_白色.png"));
	//m_pMainWndContent->Add(button1);

	//CHorizontalLayoutUI * m_pMainInterval = new CHorizontalLayoutUI();//间隙
	//m_pMainInterval->SetFixedWidth(100);
	//m_pMainWndContent->Add(m_pMainInterval);

	//CButtonUI * button2 = new CButtonUI();
	//button2->SetFixedHeight(50);
	//button2->SetFixedWidth(50);
	//button2->SetTextColor(0xFF000000);
	//button2->SetForeImage(_T("ui/卫星_白色.png"));
	//m_pMainWndContent->Add(button2);
}

void DrawingInterfaceClass::SettingCues(LPCTSTR lpstrTitle)
{
	//CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("lable_Statement")));
	//if( pControl )pControl->SetText(lpstrTitle);
	//if (pControl)pControl->SetText(lpstrTitle);

	//m_CLabelUI->SetText(lpstrTitle);
	m_CEditUI->SetText(lpstrTitle);
}
//将所有的卫星、地球站、地面终端绘制在界面上
void DrawingInterfaceClass::DrawTheInterface_All(vector<string> vec_wx, vector<string> vec_dqz, vector<string> vec_dmzd)
{
	/*****************************卫星****************************/
//创建卫星那一行的组
	CHorizontalLayoutUI * m_pMainWndContent = dynamic_cast<CHorizontalLayoutUI*>(m_pm.FindControl(stringToWstring("horizontal_WX").c_str()));
	for (int i = 0; i < vec_wx.size(); i++)
	{

		CVerticalLayoutUI * m_pMainVer = new CVerticalLayoutUI();//VerticalLayout
		m_pMainVer->SetFixedWidth(50);
		m_pMainVer->SetVisible(true);

		CButtonUI * button1 = new CButtonUI();
		button1->SetFixedHeight(50);
		button1->SetFixedWidth(50);
		button1->SetTextColor(0xFF000000);
		button1->SetForeImage(_T("ui/卫星_白色.png"));
		m_pMainVer->Add(button1);
		vec_wxIcon.push_back(button1);

		CLabelUI * lable = new CLabelUI();
		lable->SetFixedHeight(30);
		lable->SetFixedWidth(50);
		lable->SetTextColor(0xFFFFFFFF);
		lable->SetAttribute(_T("wordbreak"), _T("true"));
		lable->SetText(stringToWstring(vec_wx[i]).c_str());

		m_pMainVer->Add(lable);

		CButtonUI * button2 = new CButtonUI();
		button2->SetFixedHeight(50);
		button2->SetFixedWidth(50);
		button2->SetTextColor(0xFF000000);
		button2->SetForeImage(_T("ui/上下箭头灭_白色.png"));
		m_pMainVer->Add(button2);
		vec_wxArrowBelow.push_back(button2);
		m_pMainWndContent->Add(m_pMainVer);
		vec_wxName.push_back(vec_wx[i]);

	}

	/*****************************地球站****************************/
	CHorizontalLayoutUI * m_pMainWndDQZh = dynamic_cast<CHorizontalLayoutUI*>(m_pm.FindControl(stringToWstring("horizontal_DQZ").c_str()));
	for (int i = 0; i < vec_dqz.size(); i++)
	{

		CVerticalLayoutUI * m_pMainVer = new CVerticalLayoutUI();//VerticalLayout
		m_pMainVer->SetFixedWidth(50);
		m_pMainVer->SetVisible(true);

		CButtonUI * button0 = new CButtonUI();
		button0->SetFixedHeight(50);
		button0->SetFixedWidth(50);
		button0->SetTextColor(0xFF000000);
		button0->SetForeImage(_T("ui/上下箭头灭_白色.png"));
		m_pMainVer->Add(button0);
		vec_dqzArrowAbove.push_back(button0);

		CButtonUI * button1 = new CButtonUI();
		button1->SetFixedHeight(50);
		button1->SetFixedWidth(50);
		button1->SetTextColor(0xFF000000);
		button1->SetForeImage(_T("ui/设备_白色.png"));
		m_pMainVer->Add(button1);
		vec_dqzIcon.push_back(button1);

		CLabelUI * lable = new CLabelUI();
		lable->SetFixedHeight(30);
		lable->SetFixedWidth(50);
		lable->SetTextColor(0xFFFFFFFF);
		lable->SetAttribute(_T("wordbreak"), _T("true"));
		lable->SetText(stringToWstring(vec_dqz[i]).c_str());

		m_pMainVer->Add(lable);

		CButtonUI * button2 = new CButtonUI();
		button2->SetFixedHeight(50);
		button2->SetFixedWidth(50);
		button2->SetTextColor(0xFF000000);
		button2->SetForeImage(_T("ui/上下箭头灭_白色.png"));
		m_pMainVer->Add(button2);
		vec_dqzArrowBelow.push_back(button2);
		m_pMainWndDQZh->Add(m_pMainVer);

		vec_dqzName.push_back(vec_dqz[i]);

	}

	///*****************************地面终端****************************/
	//创建卫星那一行的组
	CHorizontalLayoutUI * m_pMainWndDMZD = dynamic_cast<CHorizontalLayoutUI*>(m_pm.FindControl(stringToWstring("horizontal_DMZD").c_str()));
	vector<CButtonUI * > vec_CButtonUIDMZD1;
	vector<CButtonUI * > vec_CButtonUIDMZD2;
	for (int i = 0; i < vec_dmzd.size(); i++)
	{

		CVerticalLayoutUI * m_pMainVer = new CVerticalLayoutUI();//VerticalLayout
		m_pMainVer->SetFixedWidth(50);
		m_pMainVer->SetVisible(true);

		CButtonUI * button1 = new CButtonUI();
		button1->SetFixedHeight(50);
		button1->SetFixedWidth(50);
		button1->SetTextColor(0xFF000000);
		button1->SetForeImage(_T("ui/上下箭头灭_白色.png"));
		m_pMainVer->Add(button1);
		vec_dmzdArrowAbove.push_back(button1);

		CButtonUI * button2 = new CButtonUI();
		button2->SetFixedHeight(50);
		button2->SetFixedWidth(50);
		button2->SetTextColor(0xFF000000);
		button2->SetForeImage(_T("ui/设备_白色.png"));
		m_pMainVer->Add(button2);
		vec_dmzdIcon.push_back(button2);

		CLabelUI * lable = new CLabelUI();
		lable->SetFixedHeight(30);
		lable->SetFixedWidth(50);
		lable->SetTextColor(0xFFFFFFFF);
		lable->SetAttribute(_T("wordbreak"), _T("true"));
		lable->SetText(stringToWstring(vec_dmzd[i]).c_str());
		m_pMainVer->Add(lable);

		m_pMainWndDMZD->Add(m_pMainVer);

		vec_dmzdName.push_back(vec_dmzd[i]);
	}

}
//将链路中的卫星、地球站、地面终端点亮
void DrawingInterfaceClass::LightUpTheDevice(vector<string> vec_wx, vector<string> vec_dqz, vector<string> vec_dmzd)
{

	for (int i = 0; i < vec_wx.size(); i++)
	{
		for (int j = 0; j < vec_wxName.size(); j++)
		{
			if (vec_wxName[j] == vec_wx[i])
			{

				vec_wxIcon[j]->SetForeImage(_T("ui/卫星.png"));//卫星图标
				vec_wxArrowBelow[j]->SetForeImage(_T("ui/上下箭头亮.png"));;//卫星下面的箭头
			}
		}
	}

	for (int i = 0; i < vec_dqz.size(); i++)
	{
		for (int j = 0; j < vec_dqzName.size(); j++)
		{
			if (vec_dqzName[j] == vec_dqz[i])
			{

				vec_dqzArrowAbove[j]->SetForeImage(_T("ui/上下箭头亮.png"));;//卫星下面的箭头
				vec_dqzIcon[j]->SetForeImage(_T("ui/设备.png"));//卫星图标
				vec_dqzArrowBelow[j]->SetForeImage(_T("ui/上下箭头亮.png"));;//卫星下面的箭头
			}
		}
	}

	for (int i = 0; i < vec_dmzd.size(); i++)
	{
		for (int j = 0; j < vec_dmzdName.size(); j++)
		{
			if (vec_dmzdName[j] == vec_dmzd[i])
			{
				vec_dmzdArrowAbove[j]->SetForeImage(_T("ui/上下箭头亮.png"));;//卫星下面的箭头
				vec_dmzdIcon[j]->SetForeImage(_T("ui/设备.png"));//卫星图标

			}
		}
	}

}


