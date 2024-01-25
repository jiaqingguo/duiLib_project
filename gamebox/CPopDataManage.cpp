#include "stdafx.h"
#include "CPopDataManage.h"
#include "UtilTool.h"
#include "EnvironmentData.h"

//vector<string> seleceColumns;
//数据管理界面类实现
DUI_BEGIN_MESSAGE_MAP(CPopDataManage, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CPopDataManage::CPopDataManage(void)
{

}

CPopDataManage::~CPopDataManage(void)
{



}

void CPopDataManage::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}
DWORD StringToDword(string val)
{
	DWORD cur_dword;
	sscanf(val.c_str(), "%ul", &cur_dword);
	return cur_dword;
}

DuiLib::CDuiString CPopDataManage::GetSkinFile()
{
	return _T("DataManagement.xml");
}

LPCTSTR CPopDataManage::GetWindowClassName(void) const
{
	return _T("DataManagement");
}

void CPopDataManage::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}


}

void CPopDataManage::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopDataManage::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopDataManage::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT CPopDataManage::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  CPopDataManage::get_cur_timelogdata()/*获取当前时间字符串*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}

wstring CPopDataManage::stringToWstring(string str)
{

	wstring w_str;
	UtilTool::setWstring(w_str, str.c_str());
	return w_str;


}
//wstring转string
string CPopDataManage::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}

void CPopDataManage::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	
	if (sName.CompareNoCase(_T("bt_Import")) == 0)//导入按钮
	{

		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_DatabaseName")));
		select_combox->RemoveAll();
		m_dataList_XZSJB->RemoveAll();
		m_dataList_DXSJB->RemoveAll();
		m_dataList_WXZHSJB->RemoveAll();
		m_dataList_WXTX->RemoveAll();
		m_dataList_DXGSJB->RemoveAll();
		m_dataList_DQZSJB->RemoveAll();
		m_dataList_DQZKYSJB->RemoveAll();

		m_dataList_DQZBXSB->RemoveAll();
		m_dataList_DQZTX->RemoveAll();
		m_dataList_DMZDSJB->RemoveAll();
		m_dataList_ZDZHSJB->RemoveAll();
		m_dataList_BXSBSJB->RemoveAll();
		m_dataList_ZDTX->RemoveAll();

		InitializeCombo();

	}
	else if (sName.CompareNoCase(_T("bt_Flushed")) == 0)//刷新按钮
	{
		CControlUI* pControl_Start = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_DatabaseName")));
		wstring wstr_start = pControl_Start->GetText();
		string str_dbName = wstringToString(wstr_start);
	/*	bool flag = ConnectMysql::Instance().SetUpDatabase(str_dbName);
		if (flag)
		{
			GettingDatabase();
		}*/
		GettingDatabase();
		//vec_vecData_XZSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_星座数据表_1");//获取的数据库中---星座数据表


		
	}
	//{
	//	m_dataList_DXSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt2")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DXSJB = str;
	//	Filter_DXSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt3")) == 0)//卫星载荷数据表筛选
	//{
	//	m_dataList_WXZHSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt3")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	//string m_ChooseIt_DXSJB;//用户选择筛选的列名称 ---单星数据表
	//	m_ChooseIt_WXZHSJB = str;
	//	Filter_WXZHSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt4")) == 0)//卫星天线数据表筛选
	//{
	//	m_dataList_WXTX->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt4")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_WXTX = str;
	//	Filter_WXTX();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt5")) == 0)//电信港数据表筛选
	//{
	//	m_dataList_DXGSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt5")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DXGSJB = str;
	//	Filter_DXGSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt6")) == 0)//地球站数据表筛选
	//{
	//	m_dataList_DQZSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt6")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZSJB = str;
	//	Filter_DQZSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt7")) == 0)//地球站溃源数据表筛选
	//{
	//	m_dataList_DQZKYSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt7")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZKYSJB = str;
	//	Filter_DQZKYSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt8")) == 0)//地球站波形设备数据表
	//{

	//	m_dataList_DQZBXSB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt8")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZBXSB = str;
	//	Filter_DQZBXSBSJB();

	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt9")) == 0)//地地球站天线筛选
	//{
	//	m_dataList_DQZTX->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt9")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZTX = str;
	//	Filter_DQZTXSJB();

	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt10")) == 0)// 地面终端数据表筛选
	//{
	//	m_dataList_DMZDSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt10")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DMZDSJB = str;
	//	Filter_DMZDSJB();

	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt11")) == 0)//终端载荷数据表筛选
	//{
	//	m_dataList_ZDZHSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt11")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_ZDZHSJB = str;
	//	Filter_ZDZHSJBSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt12")) == 0)//波形设备数据表筛选
	//{
	//	m_dataList_BXSBSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt12")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_BXSBSJB = str;
	//	Filter_BXSBSJBSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt13")) == 0)//终端天线筛选
	//{
	//	m_dataList_ZDTX->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt13")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_ZDTX = str;
	//	Filter_ZDTXSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Import_opt1")) == 0)//星座数据表里面的
	//{
	//	vector<wstring> upload_fileName;
	//	upload_fileName = UtilTool::GetOpenFilePaths_xls();


	//}



}
void CPopDataManage::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// 按钮消息
		OnLClick(msg.pSender);
	}
	else if (msg.sType == _T("selectchanged"))
	{

		if (strControlName.CompareNoCase(_T("opt_DataPage1")) == 0)
		{
			pTabDatabase_0->SelectItem(0);

		}
		else if (strControlName.CompareNoCase(_T("opt_DataPage2")) == 0) {

			pTabDatabase_0->SelectItem(1);
		}

		if (strControlName.CompareNoCase(_T("opt_Tab1")) == 0)
		{
			pTabDatabase_1->SelectItem(0);

		}
		else if (strControlName.CompareNoCase(_T("opt_Tab2")) == 0) {

			pTabDatabase_1->SelectItem(1);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab3")) == 0) {

			pTabDatabase_1->SelectItem(2);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab4")) == 0) {

			pTabDatabase_1->SelectItem(3);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab5")) == 0) {

			pTabDatabase_1->SelectItem(4);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab6")) == 0) {

			pTabDatabase_1->SelectItem(5);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab7")) == 0) {

			pTabDatabase_1->SelectItem(6);
		}

		/************************************************/
		if (strControlName.CompareNoCase(_T("opt_Tab2_0")) == 0)
		{
			pTabDatabase_2->SelectItem(0);

		}
		else if (strControlName.CompareNoCase(_T("opt_Tab2_1")) == 0) {

			pTabDatabase_2->SelectItem(1);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab2_2")) == 0) {

			pTabDatabase_2->SelectItem(2);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab2_3")) == 0) {

			pTabDatabase_2->SelectItem(3);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab2_4")) == 0) {

			pTabDatabase_2->SelectItem(4);
		}
		else if (strControlName.CompareNoCase(_T("opt_Tab2_5")) == 0) {

			pTabDatabase_2->SelectItem(5);
		}

	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt1"))//tab1里面的筛选条件 ---星座数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt1"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender; 
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt1")));
		string XZSJB = FilterArray_1[nsel];//星座名称  所属国家 所属机构 下属卫星名称
		m_selectConditionsXZSJB = XZSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt2"))//tab1里面的筛选条件 ---单星数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt2"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt2")));
		string DXSJB = FilterArray_2[nsel];//星座名称  所属国家 所属机构 下属卫星名称
		m_selectConditions_DXSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt3"))//tab1里面的筛选条件 ---卫星载荷数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt3"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt3")));
		string DXSJB = FilterArray_3[nsel];//星座名称  所属国家 所属机构 下属卫星名称
		m_selectConditions_WXZHSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt4"))//tab1里面的筛选条件 ---卫星天线
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt4"))
		{
			return;
		}
		string ;//选择的卫星天线列名

		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt4")));
		string DXSJB = FilterArray_4[nsel];
		m_selectConditions_WXTX = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt5"))//tab1里面的筛选条件 ---电线港数据表
	{

		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt5"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt5")));
		string DXSJB = FilterArray_5[nsel];//星座名称  所属国家 所属机构 下属卫星名称
		m_selectConditions_DXGSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt6"))//tab1里面的筛选条件 ---地球站数据表
	{

		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt6"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt6")));
		string DXSJB = FilterArray_6[nsel];//星座名称  所属国家 所属机构 下属卫星名称
		m_selectConditions_DQZSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt7"))//tab1里面的筛选条件 ---地球站溃源数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt7"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt7")));
		string DXSJB = FilterArray_7[nsel];//星座名称  所属国家 所属机构 下属卫星名称
		m_selectConditions_DQZKYSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt8"))//tab1里面的筛选条件 ---地球站溃源数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt8"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt8")));
		string DXSJB = FilterArray_8[nsel];//星座名称  所属国家 所属机构 下属卫星名称
		m_selectConditions_DQZBXSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt9"))//tab1里面的筛选条件 ---地球站天线数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt9"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt9")));
		string DXSJB = FilterArray_9[nsel];
		m_selectConditions_DQZTXSJB = DXSJB;

	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt10"))//tab1里面的筛选条件 ---地面终端数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt10"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt10")));
		string DXSJB = FilterArray_10[nsel];
		m_selectConditions_DMZDSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt11"))//tab1里面的筛选条件 ---终端载荷数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt11"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt11")));
		string DXSJB = FilterArray_11[nsel];
		m_selectConditions_DQZDZHSJB = DXSJB;


	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt12"))//tab1里面的筛选条件 ---终端波形设备数据表
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt12"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt12")));
		string DXSJB = FilterArray_12[nsel];
		m_selectConditions_BXSBSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt13"))//tab1里面的筛选条件 ---终端天线
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt13"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt13")));
		string DXSJB = FilterArray_13[nsel];
		m_selectConditions_ZDTXSJB = DXSJB;
	}
	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CPopDataManage::Busys(void* param)
{

	CPopDataManage * pWnd = (CPopDataManage*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}

//初始化
void CPopDataManage::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	
	pTabDatabase_0 = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_database0")));
	pTabDatabase_1 = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_database1")));
	pTabDatabase_2 = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_database2")));

	m_dataList_XZSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_XZSJB")));//绑定list
	m_dataList_DXSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DXSJB")));//单星数据表
	m_dataList_WXZHSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_WXZHAJB")));//卫星载荷数据表的List
	m_dataList_WXTX = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_WXTX")));//卫星天线的List
	m_dataList_DXGSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DXGSJB")));//电信港数据表的List
	m_dataList_DQZSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZSJB")));//地球站数据表的List
	m_dataList_DQZKYSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZKYSJB")));//地球站溃源数据表的List

	m_dataList_DQZBXSB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZBXSBSJB")));//地球站波形设备数据表的List
	m_dataList_DQZTX = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZTX")));//地球站天线的List
	m_dataList_DMZDSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DMZDSJB")));//地面终端数据表的List
	m_dataList_ZDZHSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_ZDZHSJB")));//终端载荷数据表的List
	m_dataList_BXSBSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_ZDBXSBSJB")));//波形设备数据表的List
	m_dataList_ZDTX = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_ZDTX")));//终端天线的List


	
	//list_DataShow_XZSJB
	/*if (!ConnectMysql::Instance().StartConnectMysql())
	{
		cout << "StartConnectMysql Failure!!!" << endl;
		string msg = "数据库连接失败!";

	}
	else
	{
		cout << "StartConnectMysql Successful!!!" << endl;
		string msg = "数据库链接成功!";

	}*/

	//GettingDatabase();

	InitializeMap();
	//string all_data = ConnectMysql::Instance().mytest_QueryDatabase_1("0_星座数据表_1");
	//cout << all_data.c_str() << endl;
	//vector<vector<string>> vec_vecData = ConnectMysql::Instance().mytest_QueryDatabase("0_星座数据表_1");
	//string recv = vec_vecData[0][2];
	//cout << recv.c_str() << endl;

}
void CPopDataManage::InitializeCombo()
{
	vector<string> vec = ConnectMysql::Instance().mytest_QueryDatabase();
	cout << vec.size() << endl;


	ConnectMysql::Instance().getAllScheme(EnvironmentData::m_mapStScheme);
	select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_DatabaseName")));
	if (select_combox)
	{
		for (const auto& stData : EnvironmentData::m_mapStScheme)
		{
			auto& st = stData.second;
			wstring temp;
			CListLabelElementUI* pLabel = new CListLabelElementUI();
			UtilTool::setWstring(temp, st.schemeName.c_str());
			pLabel->SetText(temp.c_str());
			CDuiString strID;
			strID.Format(_T("%d"), st.id); // 将int转换为字符串
			pLabel->SetUserData(strID); // 设置项的用户数据
			pLabel->SetBkColor(0xff0000);
			
		    select_combox->Add(pLabel);
		}
		select_combox->SelectItem(0);
	}
	
	//FilterArray_1[0] = "无筛选";
	//FilterArray_1[1] = "星座名称";
	//FilterArray_1[2] = "所属国家";
	//FilterArray_1[3] = "所属机构";
	//FilterArray_1[4] = "下属卫星名称";
	//int num =  strlen(FilterArray_1);
	//for (int i = 0; i < vec.size(); i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, vec[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetUserData(_T("Item Data")); // 设置项的用户数据
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	// 
	//}
	//select_combox->SelectItem(0);
	

	//string FilterArray_1[4];//星座数据表筛选列名
	//string FilterArray_2[6];//单星数据表筛选列名
	//string FilterArray_3[3];//卫星载荷数据表筛选列名
	//string FilterArray_4[3];//卫星天线数据表筛选列名
	//string FilterArray_5[4];//电信港数据表筛选列名
	//string FilterArray_6[4];//地球站数据表筛选列名
	//string FilterArray_7[4];//地球站溃源数据表筛选列名
	//string FilterArray_8[3];//地球站波形设备数据表筛选列名
	//string FilterArray_9[3];//地球站天线数据表筛选列名
	//string FilterArray_10[2];//地面终端数据表筛选列名
	//string FilterArray_11[4];//终端载荷数据表筛选列名
	//string FilterArray_12[2];//终端波形设备数据表筛选列名
	//string FilterArray_13[3];//终端天线数据表筛选列名

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt1")));
	//FilterArray_1[0] = "无筛选";
	//FilterArray_1[1] = "星座名称";
	//FilterArray_1[2] = "所属国家";
	//FilterArray_1[3] = "所属机构";
	//FilterArray_1[4] = "下属卫星名称";
	////int num =  strlen(FilterArray_1);
	//for (int i = 0; i < 5; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_1[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt2")));
	//FilterArray_2[0] = "无筛选";
	//FilterArray_2[1] = "卫星名称";
	//FilterArray_2[2] = "所属星座";
	//FilterArray_2[3] = "所属国家";
	//FilterArray_2[4] = "所属机构";
	//FilterArray_2[5] = "功能类别";//
	//FilterArray_2[6] = "所属轨道类别";//所属轨道卫星
	//int num = FilterArray_2->size();
	//for (int i = 0; i < 7; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_2[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt3")));
	//
	//FilterArray_3[0] = "无筛选";
	//FilterArray_3[1] = "载荷名称";
	//FilterArray_3[2] = "工作波段";
	//FilterArray_3[3] = "所属机构";

	//for (int i = 0; i < 4; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_3[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt4")));

	//FilterArray_4[0] = "无筛选";
	//FilterArray_4[1] = "天线名称";
	//FilterArray_4[2] = "所属卫星";
	//FilterArray_4[3] = "天线类型";

	//for (int i = 0; i < 4; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_4[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt5")));

	//FilterArray_5[0] = "无筛选";
	//FilterArray_5[1] = "电信港名称";
	//FilterArray_5[2] = "所属国家";
	//FilterArray_5[3] = "所属机构";
	//FilterArray_5[4] = "性质";
	//for (int i = 0; i < 5; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_5[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt6")));

	//FilterArray_6[0] = "无筛选";
	//FilterArray_6[1] = "地球站名称";
	//FilterArray_6[2] = "所属电信港";
	//FilterArray_6[3] = "所属机构";
	//FilterArray_6[4] = "地球站类型";
	//for (int i = 0; i < 5; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_6[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt7")));

	//FilterArray_7[0] = "无筛选";
	//FilterArray_7[1] = "馈源名称";
	//FilterArray_7[2] = "所属地球站";
	//FilterArray_7[3] = "工作波段";
	//FilterArray_7[4] = "所属机构";
	//for (int i = 0; i < 5; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_7[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt8")));

	//FilterArray_8[0] = "无筛选";
	//FilterArray_8[1] = "波形设备名称";
	//FilterArray_8[2] = "所属地球站";
	//FilterArray_8[3] = "波形设备型号";

	//for (int i = 0; i < 4; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_8[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt9")));

	//FilterArray_9[0] = "无筛选";
	//FilterArray_9[1] = "天线名称";
	//FilterArray_9[2] = "所属地球站";
	//FilterArray_9[3] = "天线类型";

	//for (int i = 0; i < 4; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_9[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt10")));

	//FilterArray_10[0] = "无筛选";
	//FilterArray_10[1] = "终端名称";
	//FilterArray_10[2] = "终端类型";


	//for (int i = 0; i < 3; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_10[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt11")));

	//FilterArray_11[0] = "无筛选";
	//FilterArray_11[1] = "载荷名称";
	//FilterArray_11[2] = "所属终端";
	//FilterArray_11[3] = "工作波段";
	//FilterArray_11[4] = "所属机构";
	//for (int i = 0; i < 5; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_11[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt12")));

	//FilterArray_12[0] = "无筛选";
	//FilterArray_12[1] = "波形设备名称";
	//FilterArray_12[2] = "波形设备型号";


	//for (int i = 0; i < 3; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_12[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt13")));

	//FilterArray_13[0] = "无筛选";
	//FilterArray_13[1] = "天线名称";
	//FilterArray_13[2] = "所属终端";
	//FilterArray_13[3] = "天线类型";//天线类型

	//for (int i = 0; i < 4; i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, FilterArray_13[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	//}

	//select_combox->SelectItem(0);
}
void CPopDataManage::GettingDatabase()
{

	 //vec_vecData_XZSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_星座数据表_1");//获取的数据库中---星座数据表
	// ConnectMysql::Instance().CreateNewTable("myCsTable", vec_vecData_XZSJB)


	 //for (vector<vector<string>>::iterator it = vec_vecData_XZSJB.begin(); it != vec_vecData_XZSJB.end(); ++it)
	 //{
		// CListTextElementUI* pItem = new CListTextElementUI();
		// //pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
		// m_dataList_XZSJB->Add(pItem);
		// wstring text_string;
		// for (int i = 0; i < (*it).size(); ++i)
		// {
		//	 //cout << (*it)[i] << " ";
		//	 UtilTool::setWstring(text_string, (*it)[i].c_str());
		//	 pItem->SetText(i, text_string.c_str());
		// }
		// cout << endl;
	 //}

	 //获取星座数据表数据;
	 ConnectMysql::Instance().getAllXzsjbBySchemeID(EnvironmentData::m_mapStXZSJB,1);
	 m_dataList_XZSJB->RemoveAll();
	 wstring text_string;
	 for ( auto & stData : EnvironmentData::m_mapStXZSJB)
	 {
		 auto& stXZSJ = stData.second;
		 CListTextElementUI* pItem = new CListTextElementUI();
		 //pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
		
		 m_dataList_XZSJB->Add(pItem);
		 UtilTool::setWstring(text_string, stXZSJ.constellationName.c_str());
		 pItem->SetText(0, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.constellationNation.c_str());
		 pItem->SetText(1, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.constellationCorp.c_str());
		 pItem->SetText(2, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.constellationYear.c_str());
		 pItem->SetText(3, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.constellationPhase.c_str());
		 pItem->SetText(4, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.satNum.c_str());
		 pItem->SetText(5, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.COL.c_str());
		 pItem->SetText(6, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.satName.c_str());
		 pItem->SetText(7, text_string.c_str());
		 UtilTool::setWstring(text_string, stXZSJ.planeNum.c_str());
		 pItem->SetText(8, text_string.c_str());

	 }

	//单星数据表
	 ConnectMysql::Instance().getAllDxsjbBySchemeID(EnvironmentData::m_mapStDXSJB,1);
	 m_dataList_DXSJB->RemoveAll();
	 for (auto& stData : EnvironmentData::m_mapStDXSJB)
	 {

		 auto& stDXSJ = stData.second;
		 CListTextElementUI* pItem = new CListTextElementUI();
	
		 m_dataList_DXSJB->Add(pItem);
		 UtilTool::setWstring(text_string, stDXSJ.satName.c_str());
		 pItem->SetText(0, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.constellationName.c_str());
		 pItem->SetText(1, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satNation.c_str());
		 pItem->SetText(2, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satCorp.c_str());
		 pItem->SetText(3, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satType.c_str());
		 pItem->SetText(4, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.orbitType.c_str());
		 pItem->SetText(5, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.NORAD.c_str());
		 pItem->SetText(6, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.COPAR.c_str());
		 pItem->SetText(7, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.isPublic.c_str());
		 pItem->SetText(8, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.SOL.c_str());
		 pItem->SetText(9, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satSemiMajor.c_str());
		 pItem->SetText(10, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satEcc.c_str());
		 pItem->SetText(11, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satOblique.c_str());
		 pItem->SetText(12, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satRAAN.c_str());
		 pItem->SetText(13, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satOmega.c_str());
		 pItem->SetText(14, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satTrueAnomaly.c_str());
		 pItem->SetText(15, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satLoadName.c_str());
		 pItem->SetText(16, text_string.c_str());
		 UtilTool::setWstring(text_string, stDXSJ.satAntennaName.c_str());
		 pItem->SetText(17, text_string.c_str());
	 }

	 //获取卫星天线数据表数据;
	 ConnectMysql::Instance().getAllWxtxbBySchemeID(EnvironmentData::m_mapStWXTXB,1);
	 m_dataList_WXTX->RemoveAll();
	 for (auto& st : EnvironmentData::m_mapStWXTXB)
	 {

		 auto& stData = st.second;
		 CListTextElementUI* pItem = new CListTextElementUI();
		
		 m_dataList_WXTX->Add(pItem);
		 UtilTool::setWstring(text_string, stData.satAntennaName.c_str());
		 pItem->SetText(0, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satName.c_str());
		 pItem->SetText(1, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satAntennaType.c_str());
		 pItem->SetText(2, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satAntennaCaliber.c_str());
		 pItem->SetText(3, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satAntennaGain.c_str());
		 pItem->SetText(4, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satAntennaEff.c_str());
		 pItem->SetText(5, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satAntennaPointerLoss.c_str());
		 pItem->SetText(6, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satName.c_str());
		 pItem->SetText(7, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satAntennaPolarLoss.c_str());
		 pItem->SetText(8, text_string.c_str());

	 }
	 //获取卫星载荷数据数据表数据;
	 ConnectMysql::Instance().getAllWxzhsjbBySchemeID(EnvironmentData::m_mapStWXZHSJB,1);
	 m_dataList_WXZHSJB->RemoveAll();
	 for (auto& st : EnvironmentData::m_mapStWXZHSJB)
	 {
		 auto& stData = st.second;
		 CListTextElementUI* pItem = new CListTextElementUI();
		 m_dataList_WXZHSJB->Add(pItem);

		 UtilTool::setWstring(text_string, stData.satLoadName.c_str());
		 pItem->SetText(0, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satLoadWaveBand.c_str());
		 pItem->SetText(1, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satLoadCorp.c_str());
		 pItem->SetText(2, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satLoadSendEIRP.c_str());
		 pItem->SetText(3, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satLoadSendSFD.c_str());
		 pItem->SetText(4, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satLoadRecvGT.c_str());
		 pItem->SetText(5, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.satLoadBandwidth.c_str());
		 pItem->SetText(6, text_string.c_str());
	 }

	 //电信港数据表
	 ConnectMysql::Instance().getAllDxgsjbBySchemeID(EnvironmentData::m_mapStDXGSJB, 1);
	 m_dataList_DXGSJB->RemoveAll();
	 for (auto& st : EnvironmentData::m_mapStDXGSJB)
	 {
		 auto& stData = st.second;
		 CListTextElementUI* pItem = new CListTextElementUI();
		 m_dataList_DXGSJB->Add(pItem);
		 UtilTool::setWstring(text_string, stData.teleportName.c_str());
		 pItem->SetText(0, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.teleportNation.c_str());
		 pItem->SetText(1, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.teleportCorp.c_str());
		 pItem->SetText(2, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.teleportType.c_str());
		 pItem->SetText(3, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.teleportYear.c_str());
		 pItem->SetText(4, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.teleportLon.c_str());
		 pItem->SetText(5, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.teleportLat.c_str());
		 pItem->SetText(6, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationName.c_str());
		 pItem->SetText(7, text_string.c_str());
	 }

	 //地球站数据表
	 ConnectMysql::Instance().getAllDqzsjbBySchemeID(EnvironmentData::m_mapStDQZSJB, 1);
	 m_dataList_DQZSJB->RemoveAll();
	 for (auto& st : EnvironmentData::m_mapStDQZSJB)
	 {
		 auto& stData = st.second;
		 CListTextElementUI* pItem = new CListTextElementUI();
		 m_dataList_DQZSJB->Add(pItem);
		 UtilTool::setWstring(text_string, stData.stationName.c_str());
		 pItem->SetText(0, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.teleportName.c_str());
		 pItem->SetText(1, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationCorp.c_str());
		 pItem->SetText(2, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationType.c_str());
		 pItem->SetText(3, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLon.c_str());
		 pItem->SetText(4, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLat.c_str());
		 pItem->SetText(5, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationAlt.c_str());
		 pItem->SetText(6, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadName.c_str());
		 pItem->SetText(7, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationWaveName.c_str());
		 pItem->SetText(8, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationAntennaName.c_str());
		 pItem->SetText(9, text_string.c_str());

	 }

	 //地球站馈源数据表
	 ConnectMysql::Instance().getAllDqzkysjbBySchemeID(EnvironmentData::m_mapStDQZKYSJB, 1);
	 m_dataList_DQZKYSJB->RemoveAll();
	 for (auto& st : EnvironmentData::m_mapStDQZKYSJB)
	 {
		 auto& stData = st.second;
		 CListTextElementUI* pItem = new CListTextElementUI();
		 m_dataList_DQZKYSJB->Add(pItem);
		 UtilTool::setWstring(text_string, stData.stationLoadName.c_str());
		 pItem->SetText(0, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationName.c_str());
		 pItem->SetText(1, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadWaveBand.c_str());
		 pItem->SetText(2, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadCorp.c_str());
		 pItem->SetText(3, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadSendPower.c_str());
		 pItem->SetText(4, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadSendCarrierFreq.c_str());
		 pItem->SetText(5, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadSendFEC.c_str());
		 pItem->SetText(6, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadUpBPS.c_str());
		 pItem->SetText(7, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadRecvGT.c_str());
		 pItem->SetText(8, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadRecvCarrierFreq.c_str());
		 pItem->SetText(9, text_string.c_str());
		 UtilTool::setWstring(text_string, stData.stationLoadDownBPS.c_str());
		 pItem->SetText(10, text_string.c_str());	

	 }	

	 // 地球站波形设备数据表;
	 ConnectMysql::Instance().getAllDqzbxsbsjbBySchemeID(EnvironmentData::m_mapStDQZBXSBSJB, 1);
	 m_dataList_DQZBXSB->RemoveAll();
	for (auto& st : EnvironmentData::m_mapStDQZBXSBSJB)
	{
		auto& stData = st.second;
		CListTextElementUI* pItem = new CListTextElementUI();
		m_dataList_DQZBXSB->Add(pItem);
		UtilTool::setWstring(text_string, stData.stationWaveName.c_str());
		pItem->SetText(0, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationName.c_str());
		pItem->SetText(1, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationWaveModel.c_str());
		pItem->SetText(2, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationWaveModType.c_str());
		pItem->SetText(3, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationWaveDemoType.c_str());
		pItem->SetText(4, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationWaveCodingType.c_str());
		pItem->SetText(5, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationWaveCarrierType.c_str());
		pItem->SetText(6, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationWaveBandwidth.c_str());
		pItem->SetText(7, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationWaveType.c_str());
		pItem->SetText(8, text_string.c_str());
	}

	//地球站天线
	ConnectMysql::Instance().getAllDqztxbBySchemeID(EnvironmentData::m_mapStDQZTXB, 1);
	m_dataList_DQZTX->RemoveAll();
	for (auto& st : EnvironmentData::m_mapStDQZTXB)
	{
		auto& stData = st.second;
		CListTextElementUI* pItem = new CListTextElementUI();
		m_dataList_DQZTX->Add(pItem);
		UtilTool::setWstring(text_string, stData.stationAntennaName.c_str());
		pItem->SetText(0, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationName.c_str());
		pItem->SetText(1, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationAntennaType.c_str());
		pItem->SetText(2, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationAntennaCaliber.c_str());
		pItem->SetText(3, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationAntennaGain.c_str());
		pItem->SetText(4, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationAntennaEff.c_str());
		pItem->SetText(5, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationAntennaPointerLoss.c_str());
		pItem->SetText(6, text_string.c_str());
		UtilTool::setWstring(text_string, stData.stationAntennaPolarLoss.c_str());
		pItem->SetText(7, text_string.c_str());

	}


	//vec_vecData_DXSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_单星数据表_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_DXSJB.begin(); it != vec_vecData_DXSJB.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_DXSJB->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}

	//vec_vecData_WXZHSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_卫星载荷数据表_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_WXZHSJB.begin(); it != vec_vecData_WXZHSJB.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_WXZHSJB->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}
	//vec_vecData_WXTX = ConnectMysql::Instance().mytest_QueryDatabase("0_卫星天线_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_WXTX.begin(); it != vec_vecData_WXTX.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_WXTX->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}
	//vec_vecData_DXGSJB = ConnectMysql::Instance().mytest_QueryDatabase("1_电信港数据表_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_DXGSJB.begin(); it != vec_vecData_DXGSJB.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_DXGSJB->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}

	//vec_vecData_DQZSJB = myMysql.mytest_QueryDatabase("1_地球站数据表_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_DQZSJB.begin(); it != vec_vecData_DQZSJB.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_DQZSJB->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}

	//vec_vecData_DQZKYSJB = myMysql.mytest_QueryDatabase("1_地球站馈源数据表_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_DQZKYSJB.begin(); it != vec_vecData_DQZKYSJB.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_DQZKYSJB->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}

	//vec_vecData_DQZBXSB = myMysql.mytest_QueryDatabase("1_地球站波形设备数据表_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_DQZBXSB.begin(); it != vec_vecData_DQZBXSB.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_DQZBXSB->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}

	//vec_vecData_DQZTX = myMysql.mytest_QueryDatabase("1_地球站天线_1");//获取的数据库中---单星数据表
	//for (vector<vector<string>>::iterator it = vec_vecData_DQZTX.begin(); it != vec_vecData_DQZTX.end(); ++it)
	//{
	//	CListTextElementUI* pItem = new CListTextElementUI();
	//	//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
	//	m_dataList_DQZTX->Add(pItem);
	//	wstring text_string;
	//	for (int i = 0; i < (*it).size(); ++i)
	//	{
	//		//cout << (*it)[i] << " ";
	//		UtilTool::setWstring(text_string, (*it)[i].c_str());
	//		pItem->SetText(i, text_string.c_str());
	//	}
	//	cout << endl;
	//}

	vec_vecData_DMZDSJB = ConnectMysql::Instance().mytest_QueryDatabase("2_地面终端数据表_1");//获取的数据库中---单星数据表
	for (vector<vector<string>>::iterator it = vec_vecData_DMZDSJB.begin(); it != vec_vecData_DMZDSJB.end(); ++it)
	{
		CListTextElementUI* pItem = new CListTextElementUI();
		//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
		m_dataList_DMZDSJB->Add(pItem);
		wstring text_string;
		for (int i = 0; i < (*it).size(); ++i)
		{
			//cout << (*it)[i] << " ";
			UtilTool::setWstring(text_string, (*it)[i].c_str());
			pItem->SetText(i, text_string.c_str());
		}
		cout << endl;
	}

	vec_vecData_ZDZHSJB = ConnectMysql::Instance().mytest_QueryDatabase("2_终端载荷数据表_1");//获取的数据库中---单星数据表
	for (vector<vector<string>>::iterator it = vec_vecData_ZDZHSJB.begin(); it != vec_vecData_ZDZHSJB.end(); ++it)
	{
		CListTextElementUI* pItem = new CListTextElementUI();
		//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
		m_dataList_ZDZHSJB->Add(pItem);
		wstring text_string;
		for (int i = 0; i < (*it).size(); ++i)
		{
			//cout << (*it)[i] << " ";
			UtilTool::setWstring(text_string, (*it)[i].c_str());
			pItem->SetText(i, text_string.c_str());
		}
		cout << endl;
	}

	vec_vecData_BXSBSJB = ConnectMysql::Instance().mytest_QueryDatabase("2_终端波形设备数据表_1");//获取的数据库中---单星数据表
	for (vector<vector<string>>::iterator it = vec_vecData_BXSBSJB.begin(); it != vec_vecData_BXSBSJB.end(); ++it)
	{
		CListTextElementUI* pItem = new CListTextElementUI();
		//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
		m_dataList_BXSBSJB->Add(pItem);
		wstring text_string;
		for (int i = 0; i < (*it).size(); ++i)
		{
			//cout << (*it)[i] << " ";
			UtilTool::setWstring(text_string, (*it)[i].c_str());
			pItem->SetText(i, text_string.c_str());
		}
		cout << endl;
	}

	vec_vecData_ZDTX = ConnectMysql::Instance().mytest_QueryDatabase("2_终端天线_1");//获取的数据库中---单星数据表
	for (vector<vector<string>>::iterator it = vec_vecData_ZDTX.begin(); it != vec_vecData_ZDTX.end(); ++it)
	{
		CListTextElementUI* pItem = new CListTextElementUI();
		//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
		m_dataList_ZDTX->Add(pItem);
		wstring text_string;
		for (int i = 0; i < (*it).size(); ++i)
		{
			//cout << (*it)[i] << " ";
			UtilTool::setWstring(text_string, (*it)[i].c_str());
			pItem->SetText(i, text_string.c_str());
		}
		cout << endl;
	}


}
void CPopDataManage::InitializeMap()
{
	FilterArray_map1["星座名称"] = "constellationName";
	FilterArray_map1["所属国家"] = "constellationNation";
	FilterArray_map1["所属机构"] = "constellationCorp";
	FilterArray_map1["创建年份"] = "constellationYear";
	FilterArray_map1["工程期数"] = "constellationPhase";
	FilterArray_map1["工作寿命"] = "COL";
	FilterArray_map1["星座卫星总数量"] = "satNum";
	FilterArray_map1["星座轨道面数"] = "planeNum";
	FilterArray_map1["下属卫星名称"] = "satName";


	FilterArray_map2["卫星名称"] = "satName";
	FilterArray_map2["所属星座"] = "constellationName";
	FilterArray_map2["所属国家"] = "satNation";
	FilterArray_map2["所属机构"] = "satCorp";
	FilterArray_map2["功能类别"] = "satType";
	FilterArray_map2["所属轨道类别"] = "orbitType";
	FilterArray_map2["NORAD编号"] = "NORAD";
	FilterArray_map2["COPAR编号"] = "COPAR";
	FilterArray_map2["是否公开信息标识"] = "isPublic";
	FilterArray_map2["卫星寿命"] = "SOL";
	FilterArray_map2["轨道半长轴"] = "satSemiMajor";			
	FilterArray_map2["轨道偏心率"] = "satEcc";
	FilterArray_map2["轨道倾角"] = "satOblique";
	FilterArray_map2["升交点赤经"] = "satRAAN";
	FilterArray_map2["近地点辐角"] = "satOmega";
	FilterArray_map2["真近点角"] = "satTrueAnomaly";
	FilterArray_map2["卫星载荷名称"] = "satLoadName";
	FilterArray_map2["卫星天线名称"] = "satAntennaName";

							
	FilterArray_map3["载荷名称"] = "satLoadName";
	FilterArray_map3["工作波段"] = "satLoadWaveBand";
	FilterArray_map3["所属机构"] = "satLoadCorp";
	FilterArray_map3["转发器发射机饱和EIRP"] = "satLoadSendEIRP";
	FilterArray_map3["转发器发射机饱和通量密度"] = "satLoadSendSFD";
	FilterArray_map3["转发器接收机G/T值"] = "satLoadRecvGT";
	FilterArray_map3["转发器带宽"] = "satLoadBandwidth";


	FilterArray_map4["天线名称"] = "satAntennaName";
	FilterArray_map4["所属卫星"] = "satName";
	FilterArray_map4["天线类型"] = "satAntennaType";
	FilterArray_map4["天线口径"] = "satAntennaCaliber";
	FilterArray_map4["天线增益"] = "userAntennaGain";
	FilterArray_map4["天线效率"] = "satAntennaEff";
	FilterArray_map4["天线指向误差"] = "satAntennaPointerLoss";
	FilterArray_map4["天线极化误差"] = "satAntennaPolarLoss";

	FilterArray_map5["电信港名称"] = "teleportName";
	FilterArray_map5["所属国家"] = "teleportNation";
	FilterArray_map5["所属机构"] = "teleportCorp";
	FilterArray_map5["性质"] = "teleportType";
	FilterArray_map5["创建年份"] = "teleportYear";
	FilterArray_map5["经度"] = "teleportLon";
	FilterArray_map5["纬度"] = "teleportLat";
	FilterArray_map5["下属地球站名称"] = "stationName";

										
											
	FilterArray_map6["地球站名称"] = "stationName";
	FilterArray_map6["所属电信港"] = "teleportName";
	FilterArray_map6["所属机构"] = "stationCorp";
	FilterArray_map6["地球站类型"] = "stationType";
	FilterArray_map6["经度"] = "stationLon";
	FilterArray_map6["纬度"] = "stationLat";
	FilterArray_map6["高度"] = "stationAlt";
	FilterArray_map6["地球站馈源名称"] = "stationLoadName";
	FilterArray_map6["地球站波形设备名称"] = "stationWaveName";
	FilterArray_map6["地球站天线名称"] = "stationAntennaName";
												
												
	FilterArray_map7["馈源名称"] = "stationLoadName";
	FilterArray_map7["所属地球站"] = "stationName";
	FilterArray_map7["工作波段"] = "stationLoadWaveBand";
	FilterArray_map7["所属机构"] = "stationLoadCorp";
	FilterArray_map7["发射机功率"] = "stationLoadSendPower";
	FilterArray_map7["发射载波频率"] = "stationLoadSendCarrierFreq";
	FilterArray_map7["前向纠错编码率"] = "stationLoadSendFEC";
	FilterArray_map7["上行信息速率"] = "stationLoadUpBPS";
	FilterArray_map7["接收机G/T值"] = "stationLoadRecvGT";
	FilterArray_map7["接收机载波频率"] = "stationLoadRecvCarrierFreq";
	FilterArray_map7["下行信息速率"] = "stationLoadDownBPS";
												
	FilterArray_map8["波形设备名称"] = "stationWaveName";//存放地球站波形设备数据表的map
	FilterArray_map8["所属地球站"] = "stationName";
	FilterArray_map8["波形设备型号"] = "stationWaveModel";
	FilterArray_map8["调制方式"] = "stationWaveModType";
	FilterArray_map8["解调方式"] = "stationWaveDemoType";
	FilterArray_map8["编码方式"] = "stationWaveCodingType";
	FilterArray_map8["载波类型"] = "stationWaveCarrierType";
	FilterArray_map8["带宽(GHz)"] = "stationWaveBandwidth";
	FilterArray_map8["设备功能"] = "stationWaveType";


	FilterArray_map9["天线名称"] = "stationAntennaName";//存放地球站天线的map
	FilterArray_map9["所属地球站"] = "stationName";
	FilterArray_map9["天线类型"] = "stationAntennaType";
	FilterArray_map9["天线口径"] = "stationAntennaCaliber";
	FilterArray_map9["天线增益"] = "userAntennaGain";
	FilterArray_map9["天线效率"] = "stationAntennaEff";
	FilterArray_map9["天线指向误差"] = "stationAntennaPointerLoss";
	FilterArray_map9["天线极化误差"] = "stationAntennaPolarLoss";


	FilterArray_map10["终端名称"] = "userName";//存放地面终端数据表的map
	FilterArray_map10["终端类型"] = "userType";
	FilterArray_map10["终端载荷名称"] = "userLoadName";
	FilterArray_map10["终端波形设备名称"] = "userWaveName";
	FilterArray_map10["终端天线名称"] = "userAntennaName";									

	FilterArray_map11["载荷名称"] = "userLoadName";//存放终端载荷数据表的map
	FilterArray_map11["所属终端"] = "userName";
	FilterArray_map11["工作波段"] = "userLoadWaveBand";
	FilterArray_map11["所属机构"] = "userLoadCorp";
	FilterArray_map11["发射机功率"] = "userLoadSendPower";
	FilterArray_map11["发射载波频率"] = "userLoadSendCarrierFreq";
	FilterArray_map11["前向纠错编码率"] = "userLoadSendFEC";
	FilterArray_map11["上行信息速率"] = "userLoadUpBPS";
	FilterArray_map11["接收机G/T值"] = "userLoadRecvGT";
	FilterArray_map11["接收机载波频率"] = "userLoadRecvCarrierFreq";
	FilterArray_map11["下行信息速率"] = "userLoadDownBPS";
	
	FilterArray_map12["波形设备名称"] = "userWaveName";//存放终端波形设备数据表的map
	FilterArray_map12["波形设备型号"] = "userWaveModel";
	FilterArray_map12["调制方式"] = "userWaveModType";
	FilterArray_map12["解调方式"] = "userWaveDemoType";
	FilterArray_map12["编码方式"] = "userWaveCodingType";
	FilterArray_map12["载波类型"] = "userWaveCarrierType";
	FilterArray_map12["带宽"] = "userWaveBandwidth";
	FilterArray_map12["设备功能"] = "userWaveType";	

	FilterArray_map13["天线名称"] = "userAntennaName";//存放终端天线的map
	FilterArray_map13["所属终端"] = "userName";
	FilterArray_map13["天线类型"] = "userAntennaType";
	FilterArray_map13["天线口径"] = "userAntennaCaliber";
	FilterArray_map13["天线增益"] = "userAntennaGain";
	FilterArray_map13["天线效率"] = "userAntennaEff";
	FilterArray_map13["天线指向误差"] = "userAntennaPointerLoss";
	FilterArray_map13["天线极化误差"] = "userAntennaPolarLoss";
}

//星座数据表
void CPopDataManage::Filter_XZSJB()
{
	if (m_selectConditionsXZSJB == "无筛选")
	{
		//vec_vecData_XZSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_星座数据表_1");//获取的数据库中---星座数据表

		for (vector<vector<string>>::iterator it = vec_vecData_XZSJB.begin(); it != vec_vecData_XZSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_XZSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map1.find(m_selectConditionsXZSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_星座数据表_1", FilterArray_map1.find(m_selectConditionsXZSJB)->second, m_ChooseIt_XZSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_XZSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//单星数据表筛选
void CPopDataManage::Filter_DXSJB()
{
	if (m_selectConditions_DXSJB == "无筛选")
	{
		//vec_vecData_DXSJB = myMysql.mytest_QueryDatabase("0_单星数据表_1");//获取的数据库中---单星数据表
		for (vector<vector<string>>::iterator it = vec_vecData_DXSJB.begin(); it != vec_vecData_DXSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DXSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{

		string column = FilterArray_map2.find(m_selectConditions_DXSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_单星数据表_1", FilterArray_map2.find(m_selectConditions_DXSJB)->second, m_ChooseIt_DXSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DXSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//卫星载荷数据表筛选
void CPopDataManage::Filter_WXZHSJB()
{
	if (m_selectConditions_WXZHSJB == "无筛选")
	{
		//vec_vecData_WXZHSJB = myMysql.mytest_QueryDatabase("0_卫星载荷数据表_1");//获取的数据库中---单星数据表
		for (vector<vector<string>>::iterator it = vec_vecData_WXZHSJB.begin(); it != vec_vecData_WXZHSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_WXZHSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map3.find(m_selectConditions_WXZHSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_卫星载荷数据表_1", FilterArray_map3.find(m_selectConditions_WXZHSJB)->second, m_ChooseIt_WXZHSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_WXZHSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//卫星天线筛选
void CPopDataManage::Filter_WXTX()
{
	if (m_selectConditions_WXTX == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_WXTX.begin(); it != vec_vecData_WXTX.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_WXTX->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map4.find(m_selectConditions_WXTX)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_卫星天线_1", FilterArray_map4.find(m_selectConditions_WXTX)->second, m_ChooseIt_WXTX);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_WXTX->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//电信港数据表筛选
void CPopDataManage::Filter_DXGSJB()
{
	if (m_selectConditions_DXGSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_DXGSJB.begin(); it != vec_vecData_DXGSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DXGSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map5.find(m_selectConditions_DXGSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_电信港数据表_1", FilterArray_map5.find(m_selectConditions_DXGSJB)->second, m_ChooseIt_DXGSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DXGSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//地球站数据表筛选
void CPopDataManage::Filter_DQZSJB()
{
	if (m_selectConditions_DQZSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_DQZSJB.begin(); it != vec_vecData_DQZSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map6.find(m_selectConditions_DQZSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_地球站数据表_1", FilterArray_map6.find(m_selectConditions_DQZSJB)->second, m_ChooseIt_DQZSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//地球站溃源数据表筛选
void CPopDataManage::Filter_DQZKYSJB()
{
	if (m_selectConditions_DQZKYSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_DQZKYSJB.begin(); it != vec_vecData_DQZKYSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZKYSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map7.find(m_selectConditions_DQZKYSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_地球站馈源数据表_1", FilterArray_map7.find(m_selectConditions_DQZKYSJB)->second, m_ChooseIt_DQZKYSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZKYSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//地球站波形设备数据表筛选
void CPopDataManage::Filter_DQZBXSBSJB()
{
	if (m_selectConditions_DQZBXSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_DQZBXSB.begin(); it != vec_vecData_DQZBXSB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZBXSB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map8.find(m_selectConditions_DQZBXSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_地球站波形设备数据表_1", FilterArray_map8.find(m_selectConditions_DQZBXSJB)->second, m_ChooseIt_DQZBXSB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZBXSB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//地球站天线筛选
void CPopDataManage::Filter_DQZTXSJB()
{
	if (m_selectConditions_DQZTXSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_DQZTX.begin(); it != vec_vecData_DQZTX.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZTX->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map9.find(m_selectConditions_DQZTXSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_地球站天线_1", FilterArray_map9.find(m_selectConditions_DQZTXSJB)->second, m_ChooseIt_DQZTX);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DQZTX->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//地面终端数据表筛选
void CPopDataManage::Filter_DMZDSJB()
{
	if (m_selectConditions_DMZDSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_DMZDSJB.begin(); it != vec_vecData_DMZDSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DMZDSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map10.find(m_selectConditions_DMZDSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_地面终端数据表_1", FilterArray_map10.find(m_selectConditions_DMZDSJB)->second, m_ChooseIt_DMZDSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_DMZDSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//终端载荷数据表筛选
void CPopDataManage::Filter_ZDZHSJBSJB()
{
	if (m_selectConditions_DQZDZHSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_ZDZHSJB.begin(); it != vec_vecData_ZDZHSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_ZDZHSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map11.find(m_selectConditions_DQZDZHSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_终端载荷数据表_1", FilterArray_map11.find(m_selectConditions_DQZDZHSJB)->second, m_ChooseIt_ZDZHSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_ZDZHSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//波形设备数据表筛选
void CPopDataManage::Filter_BXSBSJBSJB()
{
	if (m_selectConditions_BXSBSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_BXSBSJB.begin(); it != vec_vecData_BXSBSJB.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_BXSBSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map12.find(m_selectConditions_BXSBSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_终端波形设备数据表_1", FilterArray_map12.find(m_selectConditions_BXSBSJB)->second, m_ChooseIt_BXSBSJB);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_BXSBSJB->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}
//终端天线筛选
void CPopDataManage::Filter_ZDTXSJB()
{
	if (m_selectConditions_ZDTXSJB == "无筛选")
	{
		for (vector<vector<string>>::iterator it = vec_vecData_ZDTX.begin(); it != vec_vecData_ZDTX.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_ZDTX->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}
	else
	{
		string column = FilterArray_map13.find(m_selectConditions_ZDTXSJB)->second;
		cout << column << endl;
		vector<vector<string>> vecVec;
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_终端天线_1", FilterArray_map13.find(m_selectConditions_ZDTXSJB)->second, m_ChooseIt_ZDTX);
		for (vector<vector<string>>::iterator it = vecVec.begin(); it != vecVec.end(); ++it)
		{
			CListTextElementUI* pItem = new CListTextElementUI();
			//pItem->ApplyAttributeList(_T("textcolor=\"#FF000000\""));
			m_dataList_ZDTX->Add(pItem);
			wstring text_string;
			for (int i = 0; i < (*it).size(); ++i)
			{
				//cout << (*it)[i] << " ";
				UtilTool::setWstring(text_string, (*it)[i].c_str());
				pItem->SetText(i, text_string.c_str());
			}
			cout << endl;
		}
	}

}