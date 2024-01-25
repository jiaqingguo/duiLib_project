#include "stdafx.h"
#include "CPopDataManage.h"
#include "UtilTool.h"
#include "EnvironmentData.h"

//vector<string> seleceColumns;
//���ݹ��������ʵ��
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
	// ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
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


string  CPopDataManage::get_cur_timelogdata()/*��ȡ��ǰʱ���ַ���*/
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
//wstringתstring
string CPopDataManage::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}

void CPopDataManage::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	
	if (sName.CompareNoCase(_T("bt_Import")) == 0)//���밴ť
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
	else if (sName.CompareNoCase(_T("bt_Flushed")) == 0)//ˢ�°�ť
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
		//vec_vecData_XZSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_�������ݱ�_1");//��ȡ�����ݿ���---�������ݱ�


		
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
	//else if (sName.CompareNoCase(_T("bt_Filter_opt3")) == 0)//�����غ����ݱ�ɸѡ
	//{
	//	m_dataList_WXZHSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt3")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	//string m_ChooseIt_DXSJB;//�û�ѡ��ɸѡ�������� ---�������ݱ�
	//	m_ChooseIt_WXZHSJB = str;
	//	Filter_WXZHSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt4")) == 0)//�����������ݱ�ɸѡ
	//{
	//	m_dataList_WXTX->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt4")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_WXTX = str;
	//	Filter_WXTX();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt5")) == 0)//���Ÿ����ݱ�ɸѡ
	//{
	//	m_dataList_DXGSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt5")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DXGSJB = str;
	//	Filter_DXGSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt6")) == 0)//����վ���ݱ�ɸѡ
	//{
	//	m_dataList_DQZSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt6")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZSJB = str;
	//	Filter_DQZSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt7")) == 0)//����վ��Դ���ݱ�ɸѡ
	//{
	//	m_dataList_DQZKYSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt7")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZKYSJB = str;
	//	Filter_DQZKYSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt8")) == 0)//����վ�����豸���ݱ�
	//{

	//	m_dataList_DQZBXSB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt8")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZBXSB = str;
	//	Filter_DQZBXSBSJB();

	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt9")) == 0)//�ص���վ����ɸѡ
	//{
	//	m_dataList_DQZTX->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt9")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DQZTX = str;
	//	Filter_DQZTXSJB();

	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt10")) == 0)// �����ն����ݱ�ɸѡ
	//{
	//	m_dataList_DMZDSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt10")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_DMZDSJB = str;
	//	Filter_DMZDSJB();

	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt11")) == 0)//�ն��غ����ݱ�ɸѡ
	//{
	//	m_dataList_ZDZHSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt11")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_ZDZHSJB = str;
	//	Filter_ZDZHSJBSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt12")) == 0)//�����豸���ݱ�ɸѡ
	//{
	//	m_dataList_BXSBSJB->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt12")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_BXSBSJB = str;
	//	Filter_BXSBSJBSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Filter_opt13")) == 0)//�ն�����ɸѡ
	//{
	//	m_dataList_ZDTX->RemoveAll();
	//	CDuiString Test_Name;
	//	Test_Name = m_pm.FindControl(L"edit_Conditions_opt13")->GetText();
	//	string str;
	//	UtilTool::setString(str, Test_Name.GetData());

	//	m_ChooseIt_ZDTX = str;
	//	Filter_ZDTXSJB();
	//}
	//else if (sName.CompareNoCase(_T("bt_Import_opt1")) == 0)//�������ݱ������
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

		// ��ť��Ϣ
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
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt1"))//tab1�����ɸѡ���� ---�������ݱ�
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt1"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender; 
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt1")));
		string XZSJB = FilterArray_1[nsel];//��������  �������� �������� ������������
		m_selectConditionsXZSJB = XZSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt2"))//tab1�����ɸѡ���� ---�������ݱ�
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt2"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt2")));
		string DXSJB = FilterArray_2[nsel];//��������  �������� �������� ������������
		m_selectConditions_DXSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt3"))//tab1�����ɸѡ���� ---�����غ����ݱ�
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt3"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt3")));
		string DXSJB = FilterArray_3[nsel];//��������  �������� �������� ������������
		m_selectConditions_WXZHSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt4"))//tab1�����ɸѡ���� ---��������
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt4"))
		{
			return;
		}
		string ;//ѡ���������������

		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt4")));
		string DXSJB = FilterArray_4[nsel];
		m_selectConditions_WXTX = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt5"))//tab1�����ɸѡ���� ---���߸����ݱ�
	{

		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt5"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt5")));
		string DXSJB = FilterArray_5[nsel];//��������  �������� �������� ������������
		m_selectConditions_DXGSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt6"))//tab1�����ɸѡ���� ---����վ���ݱ�
	{

		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt6"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt6")));
		string DXSJB = FilterArray_6[nsel];//��������  �������� �������� ������������
		m_selectConditions_DQZSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt7"))//tab1�����ɸѡ���� ---����վ��Դ���ݱ�
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt7"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt7")));
		string DXSJB = FilterArray_7[nsel];//��������  �������� �������� ������������
		m_selectConditions_DQZKYSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt8"))//tab1�����ɸѡ���� ---����վ��Դ���ݱ�
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria_opt8"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt8")));
		string DXSJB = FilterArray_8[nsel];//��������  �������� �������� ������������
		m_selectConditions_DQZBXSJB = DXSJB;
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt9"))//tab1�����ɸѡ���� ---����վ�������ݱ�
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
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt10"))//tab1�����ɸѡ���� ---�����ն����ݱ�
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
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt11"))//tab1�����ɸѡ���� ---�ն��غ����ݱ�
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
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt12"))//tab1�����ɸѡ���� ---�ն˲����豸���ݱ�
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
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria_opt13"))//tab1�����ɸѡ���� ---�ն�����
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

//��ʼ��
void CPopDataManage::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	
	pTabDatabase_0 = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_database0")));
	pTabDatabase_1 = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_database1")));
	pTabDatabase_2 = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("tab_database2")));

	m_dataList_XZSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_XZSJB")));//��list
	m_dataList_DXSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DXSJB")));//�������ݱ�
	m_dataList_WXZHSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_WXZHAJB")));//�����غ����ݱ��List
	m_dataList_WXTX = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_WXTX")));//�������ߵ�List
	m_dataList_DXGSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DXGSJB")));//���Ÿ����ݱ��List
	m_dataList_DQZSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZSJB")));//����վ���ݱ��List
	m_dataList_DQZKYSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZKYSJB")));//����վ��Դ���ݱ��List

	m_dataList_DQZBXSB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZBXSBSJB")));//����վ�����豸���ݱ��List
	m_dataList_DQZTX = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DQZTX")));//����վ���ߵ�List
	m_dataList_DMZDSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_DMZDSJB")));//�����ն����ݱ��List
	m_dataList_ZDZHSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_ZDZHSJB")));//�ն��غ����ݱ��List
	m_dataList_BXSBSJB = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_ZDBXSBSJB")));//�����豸���ݱ��List
	m_dataList_ZDTX = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow_ZDTX")));//�ն����ߵ�List


	
	//list_DataShow_XZSJB
	/*if (!ConnectMysql::Instance().StartConnectMysql())
	{
		cout << "StartConnectMysql Failure!!!" << endl;
		string msg = "���ݿ�����ʧ��!";

	}
	else
	{
		cout << "StartConnectMysql Successful!!!" << endl;
		string msg = "���ݿ����ӳɹ�!";

	}*/

	//GettingDatabase();

	InitializeMap();
	//string all_data = ConnectMysql::Instance().mytest_QueryDatabase_1("0_�������ݱ�_1");
	//cout << all_data.c_str() << endl;
	//vector<vector<string>> vec_vecData = ConnectMysql::Instance().mytest_QueryDatabase("0_�������ݱ�_1");
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
			strID.Format(_T("%d"), st.id); // ��intת��Ϊ�ַ���
			pLabel->SetUserData(strID); // ��������û�����
			pLabel->SetBkColor(0xff0000);
			
		    select_combox->Add(pLabel);
		}
		select_combox->SelectItem(0);
	}
	
	//FilterArray_1[0] = "��ɸѡ";
	//FilterArray_1[1] = "��������";
	//FilterArray_1[2] = "��������";
	//FilterArray_1[3] = "��������";
	//FilterArray_1[4] = "������������";
	//int num =  strlen(FilterArray_1);
	//for (int i = 0; i < vec.size(); i++)
	//{
	//	wstring temp;
	//	CListLabelElementUI * pLabel = new CListLabelElementUI();
	//	UtilTool::setWstring(temp, vec[i].c_str());
	//	pLabel->SetText(temp.c_str());
	//	pLabel->SetUserData(_T("Item Data")); // ��������û�����
	//	pLabel->SetBkColor(0xff0000);
	//	if (select_combox)
	//		select_combox->AddAt(pLabel, i);
	// 
	//}
	//select_combox->SelectItem(0);
	

	//string FilterArray_1[4];//�������ݱ�ɸѡ����
	//string FilterArray_2[6];//�������ݱ�ɸѡ����
	//string FilterArray_3[3];//�����غ����ݱ�ɸѡ����
	//string FilterArray_4[3];//�����������ݱ�ɸѡ����
	//string FilterArray_5[4];//���Ÿ����ݱ�ɸѡ����
	//string FilterArray_6[4];//����վ���ݱ�ɸѡ����
	//string FilterArray_7[4];//����վ��Դ���ݱ�ɸѡ����
	//string FilterArray_8[3];//����վ�����豸���ݱ�ɸѡ����
	//string FilterArray_9[3];//����վ�������ݱ�ɸѡ����
	//string FilterArray_10[2];//�����ն����ݱ�ɸѡ����
	//string FilterArray_11[4];//�ն��غ����ݱ�ɸѡ����
	//string FilterArray_12[2];//�ն˲����豸���ݱ�ɸѡ����
	//string FilterArray_13[3];//�ն��������ݱ�ɸѡ����

	//select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria_opt1")));
	//FilterArray_1[0] = "��ɸѡ";
	//FilterArray_1[1] = "��������";
	//FilterArray_1[2] = "��������";
	//FilterArray_1[3] = "��������";
	//FilterArray_1[4] = "������������";
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
	//FilterArray_2[0] = "��ɸѡ";
	//FilterArray_2[1] = "��������";
	//FilterArray_2[2] = "��������";
	//FilterArray_2[3] = "��������";
	//FilterArray_2[4] = "��������";
	//FilterArray_2[5] = "�������";//
	//FilterArray_2[6] = "����������";//�����������
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
	//FilterArray_3[0] = "��ɸѡ";
	//FilterArray_3[1] = "�غ�����";
	//FilterArray_3[2] = "��������";
	//FilterArray_3[3] = "��������";

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

	//FilterArray_4[0] = "��ɸѡ";
	//FilterArray_4[1] = "��������";
	//FilterArray_4[2] = "��������";
	//FilterArray_4[3] = "��������";

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

	//FilterArray_5[0] = "��ɸѡ";
	//FilterArray_5[1] = "���Ÿ�����";
	//FilterArray_5[2] = "��������";
	//FilterArray_5[3] = "��������";
	//FilterArray_5[4] = "����";
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

	//FilterArray_6[0] = "��ɸѡ";
	//FilterArray_6[1] = "����վ����";
	//FilterArray_6[2] = "�������Ÿ�";
	//FilterArray_6[3] = "��������";
	//FilterArray_6[4] = "����վ����";
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

	//FilterArray_7[0] = "��ɸѡ";
	//FilterArray_7[1] = "��Դ����";
	//FilterArray_7[2] = "��������վ";
	//FilterArray_7[3] = "��������";
	//FilterArray_7[4] = "��������";
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

	//FilterArray_8[0] = "��ɸѡ";
	//FilterArray_8[1] = "�����豸����";
	//FilterArray_8[2] = "��������վ";
	//FilterArray_8[3] = "�����豸�ͺ�";

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

	//FilterArray_9[0] = "��ɸѡ";
	//FilterArray_9[1] = "��������";
	//FilterArray_9[2] = "��������վ";
	//FilterArray_9[3] = "��������";

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

	//FilterArray_10[0] = "��ɸѡ";
	//FilterArray_10[1] = "�ն�����";
	//FilterArray_10[2] = "�ն�����";


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

	//FilterArray_11[0] = "��ɸѡ";
	//FilterArray_11[1] = "�غ�����";
	//FilterArray_11[2] = "�����ն�";
	//FilterArray_11[3] = "��������";
	//FilterArray_11[4] = "��������";
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

	//FilterArray_12[0] = "��ɸѡ";
	//FilterArray_12[1] = "�����豸����";
	//FilterArray_12[2] = "�����豸�ͺ�";


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

	//FilterArray_13[0] = "��ɸѡ";
	//FilterArray_13[1] = "��������";
	//FilterArray_13[2] = "�����ն�";
	//FilterArray_13[3] = "��������";//��������

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

	 //vec_vecData_XZSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_�������ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	 //��ȡ�������ݱ�����;
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

	//�������ݱ�
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

	 //��ȡ�����������ݱ�����;
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
	 //��ȡ�����غ��������ݱ�����;
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

	 //���Ÿ����ݱ�
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

	 //����վ���ݱ�
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

	 //����վ��Դ���ݱ�
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

	 // ����վ�����豸���ݱ�;
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

	//����վ����
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


	//vec_vecData_DXSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_�������ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	//vec_vecData_WXZHSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_�����غ����ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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
	//vec_vecData_WXTX = ConnectMysql::Instance().mytest_QueryDatabase("0_��������_1");//��ȡ�����ݿ���---�������ݱ�
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
	//vec_vecData_DXGSJB = ConnectMysql::Instance().mytest_QueryDatabase("1_���Ÿ����ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	//vec_vecData_DQZSJB = myMysql.mytest_QueryDatabase("1_����վ���ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	//vec_vecData_DQZKYSJB = myMysql.mytest_QueryDatabase("1_����վ��Դ���ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	//vec_vecData_DQZBXSB = myMysql.mytest_QueryDatabase("1_����վ�����豸���ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	//vec_vecData_DQZTX = myMysql.mytest_QueryDatabase("1_����վ����_1");//��ȡ�����ݿ���---�������ݱ�
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

	vec_vecData_DMZDSJB = ConnectMysql::Instance().mytest_QueryDatabase("2_�����ն����ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	vec_vecData_ZDZHSJB = ConnectMysql::Instance().mytest_QueryDatabase("2_�ն��غ����ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	vec_vecData_BXSBSJB = ConnectMysql::Instance().mytest_QueryDatabase("2_�ն˲����豸���ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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

	vec_vecData_ZDTX = ConnectMysql::Instance().mytest_QueryDatabase("2_�ն�����_1");//��ȡ�����ݿ���---�������ݱ�
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
	FilterArray_map1["��������"] = "constellationName";
	FilterArray_map1["��������"] = "constellationNation";
	FilterArray_map1["��������"] = "constellationCorp";
	FilterArray_map1["�������"] = "constellationYear";
	FilterArray_map1["��������"] = "constellationPhase";
	FilterArray_map1["��������"] = "COL";
	FilterArray_map1["��������������"] = "satNum";
	FilterArray_map1["�����������"] = "planeNum";
	FilterArray_map1["������������"] = "satName";


	FilterArray_map2["��������"] = "satName";
	FilterArray_map2["��������"] = "constellationName";
	FilterArray_map2["��������"] = "satNation";
	FilterArray_map2["��������"] = "satCorp";
	FilterArray_map2["�������"] = "satType";
	FilterArray_map2["����������"] = "orbitType";
	FilterArray_map2["NORAD���"] = "NORAD";
	FilterArray_map2["COPAR���"] = "COPAR";
	FilterArray_map2["�Ƿ񹫿���Ϣ��ʶ"] = "isPublic";
	FilterArray_map2["��������"] = "SOL";
	FilterArray_map2["����볤��"] = "satSemiMajor";			
	FilterArray_map2["���ƫ����"] = "satEcc";
	FilterArray_map2["������"] = "satOblique";
	FilterArray_map2["������ྭ"] = "satRAAN";
	FilterArray_map2["���ص����"] = "satOmega";
	FilterArray_map2["������"] = "satTrueAnomaly";
	FilterArray_map2["�����غ�����"] = "satLoadName";
	FilterArray_map2["������������"] = "satAntennaName";

							
	FilterArray_map3["�غ�����"] = "satLoadName";
	FilterArray_map3["��������"] = "satLoadWaveBand";
	FilterArray_map3["��������"] = "satLoadCorp";
	FilterArray_map3["ת�������������EIRP"] = "satLoadSendEIRP";
	FilterArray_map3["ת�������������ͨ���ܶ�"] = "satLoadSendSFD";
	FilterArray_map3["ת�������ջ�G/Tֵ"] = "satLoadRecvGT";
	FilterArray_map3["ת��������"] = "satLoadBandwidth";


	FilterArray_map4["��������"] = "satAntennaName";
	FilterArray_map4["��������"] = "satName";
	FilterArray_map4["��������"] = "satAntennaType";
	FilterArray_map4["���߿ھ�"] = "satAntennaCaliber";
	FilterArray_map4["��������"] = "userAntennaGain";
	FilterArray_map4["����Ч��"] = "satAntennaEff";
	FilterArray_map4["����ָ�����"] = "satAntennaPointerLoss";
	FilterArray_map4["���߼������"] = "satAntennaPolarLoss";

	FilterArray_map5["���Ÿ�����"] = "teleportName";
	FilterArray_map5["��������"] = "teleportNation";
	FilterArray_map5["��������"] = "teleportCorp";
	FilterArray_map5["����"] = "teleportType";
	FilterArray_map5["�������"] = "teleportYear";
	FilterArray_map5["����"] = "teleportLon";
	FilterArray_map5["γ��"] = "teleportLat";
	FilterArray_map5["��������վ����"] = "stationName";

										
											
	FilterArray_map6["����վ����"] = "stationName";
	FilterArray_map6["�������Ÿ�"] = "teleportName";
	FilterArray_map6["��������"] = "stationCorp";
	FilterArray_map6["����վ����"] = "stationType";
	FilterArray_map6["����"] = "stationLon";
	FilterArray_map6["γ��"] = "stationLat";
	FilterArray_map6["�߶�"] = "stationAlt";
	FilterArray_map6["����վ��Դ����"] = "stationLoadName";
	FilterArray_map6["����վ�����豸����"] = "stationWaveName";
	FilterArray_map6["����վ��������"] = "stationAntennaName";
												
												
	FilterArray_map7["��Դ����"] = "stationLoadName";
	FilterArray_map7["��������վ"] = "stationName";
	FilterArray_map7["��������"] = "stationLoadWaveBand";
	FilterArray_map7["��������"] = "stationLoadCorp";
	FilterArray_map7["���������"] = "stationLoadSendPower";
	FilterArray_map7["�����ز�Ƶ��"] = "stationLoadSendCarrierFreq";
	FilterArray_map7["ǰ����������"] = "stationLoadSendFEC";
	FilterArray_map7["������Ϣ����"] = "stationLoadUpBPS";
	FilterArray_map7["���ջ�G/Tֵ"] = "stationLoadRecvGT";
	FilterArray_map7["���ջ��ز�Ƶ��"] = "stationLoadRecvCarrierFreq";
	FilterArray_map7["������Ϣ����"] = "stationLoadDownBPS";
												
	FilterArray_map8["�����豸����"] = "stationWaveName";//��ŵ���վ�����豸���ݱ��map
	FilterArray_map8["��������վ"] = "stationName";
	FilterArray_map8["�����豸�ͺ�"] = "stationWaveModel";
	FilterArray_map8["���Ʒ�ʽ"] = "stationWaveModType";
	FilterArray_map8["�����ʽ"] = "stationWaveDemoType";
	FilterArray_map8["���뷽ʽ"] = "stationWaveCodingType";
	FilterArray_map8["�ز�����"] = "stationWaveCarrierType";
	FilterArray_map8["����(GHz)"] = "stationWaveBandwidth";
	FilterArray_map8["�豸����"] = "stationWaveType";


	FilterArray_map9["��������"] = "stationAntennaName";//��ŵ���վ���ߵ�map
	FilterArray_map9["��������վ"] = "stationName";
	FilterArray_map9["��������"] = "stationAntennaType";
	FilterArray_map9["���߿ھ�"] = "stationAntennaCaliber";
	FilterArray_map9["��������"] = "userAntennaGain";
	FilterArray_map9["����Ч��"] = "stationAntennaEff";
	FilterArray_map9["����ָ�����"] = "stationAntennaPointerLoss";
	FilterArray_map9["���߼������"] = "stationAntennaPolarLoss";


	FilterArray_map10["�ն�����"] = "userName";//��ŵ����ն����ݱ��map
	FilterArray_map10["�ն�����"] = "userType";
	FilterArray_map10["�ն��غ�����"] = "userLoadName";
	FilterArray_map10["�ն˲����豸����"] = "userWaveName";
	FilterArray_map10["�ն���������"] = "userAntennaName";									

	FilterArray_map11["�غ�����"] = "userLoadName";//����ն��غ����ݱ��map
	FilterArray_map11["�����ն�"] = "userName";
	FilterArray_map11["��������"] = "userLoadWaveBand";
	FilterArray_map11["��������"] = "userLoadCorp";
	FilterArray_map11["���������"] = "userLoadSendPower";
	FilterArray_map11["�����ز�Ƶ��"] = "userLoadSendCarrierFreq";
	FilterArray_map11["ǰ����������"] = "userLoadSendFEC";
	FilterArray_map11["������Ϣ����"] = "userLoadUpBPS";
	FilterArray_map11["���ջ�G/Tֵ"] = "userLoadRecvGT";
	FilterArray_map11["���ջ��ز�Ƶ��"] = "userLoadRecvCarrierFreq";
	FilterArray_map11["������Ϣ����"] = "userLoadDownBPS";
	
	FilterArray_map12["�����豸����"] = "userWaveName";//����ն˲����豸���ݱ��map
	FilterArray_map12["�����豸�ͺ�"] = "userWaveModel";
	FilterArray_map12["���Ʒ�ʽ"] = "userWaveModType";
	FilterArray_map12["�����ʽ"] = "userWaveDemoType";
	FilterArray_map12["���뷽ʽ"] = "userWaveCodingType";
	FilterArray_map12["�ز�����"] = "userWaveCarrierType";
	FilterArray_map12["����"] = "userWaveBandwidth";
	FilterArray_map12["�豸����"] = "userWaveType";	

	FilterArray_map13["��������"] = "userAntennaName";//����ն����ߵ�map
	FilterArray_map13["�����ն�"] = "userName";
	FilterArray_map13["��������"] = "userAntennaType";
	FilterArray_map13["���߿ھ�"] = "userAntennaCaliber";
	FilterArray_map13["��������"] = "userAntennaGain";
	FilterArray_map13["����Ч��"] = "userAntennaEff";
	FilterArray_map13["����ָ�����"] = "userAntennaPointerLoss";
	FilterArray_map13["���߼������"] = "userAntennaPolarLoss";
}

//�������ݱ�
void CPopDataManage::Filter_XZSJB()
{
	if (m_selectConditionsXZSJB == "��ɸѡ")
	{
		//vec_vecData_XZSJB = ConnectMysql::Instance().mytest_QueryDatabase("0_�������ݱ�_1");//��ȡ�����ݿ���---�������ݱ�

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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_�������ݱ�_1", FilterArray_map1.find(m_selectConditionsXZSJB)->second, m_ChooseIt_XZSJB);
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
//�������ݱ�ɸѡ
void CPopDataManage::Filter_DXSJB()
{
	if (m_selectConditions_DXSJB == "��ɸѡ")
	{
		//vec_vecData_DXSJB = myMysql.mytest_QueryDatabase("0_�������ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_�������ݱ�_1", FilterArray_map2.find(m_selectConditions_DXSJB)->second, m_ChooseIt_DXSJB);
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
//�����غ����ݱ�ɸѡ
void CPopDataManage::Filter_WXZHSJB()
{
	if (m_selectConditions_WXZHSJB == "��ɸѡ")
	{
		//vec_vecData_WXZHSJB = myMysql.mytest_QueryDatabase("0_�����غ����ݱ�_1");//��ȡ�����ݿ���---�������ݱ�
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_�����غ����ݱ�_1", FilterArray_map3.find(m_selectConditions_WXZHSJB)->second, m_ChooseIt_WXZHSJB);
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
//��������ɸѡ
void CPopDataManage::Filter_WXTX()
{
	if (m_selectConditions_WXTX == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("0_��������_1", FilterArray_map4.find(m_selectConditions_WXTX)->second, m_ChooseIt_WXTX);
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
//���Ÿ����ݱ�ɸѡ
void CPopDataManage::Filter_DXGSJB()
{
	if (m_selectConditions_DXGSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_���Ÿ����ݱ�_1", FilterArray_map5.find(m_selectConditions_DXGSJB)->second, m_ChooseIt_DXGSJB);
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
//����վ���ݱ�ɸѡ
void CPopDataManage::Filter_DQZSJB()
{
	if (m_selectConditions_DQZSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_����վ���ݱ�_1", FilterArray_map6.find(m_selectConditions_DQZSJB)->second, m_ChooseIt_DQZSJB);
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
//����վ��Դ���ݱ�ɸѡ
void CPopDataManage::Filter_DQZKYSJB()
{
	if (m_selectConditions_DQZKYSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_����վ��Դ���ݱ�_1", FilterArray_map7.find(m_selectConditions_DQZKYSJB)->second, m_ChooseIt_DQZKYSJB);
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
//����վ�����豸���ݱ�ɸѡ
void CPopDataManage::Filter_DQZBXSBSJB()
{
	if (m_selectConditions_DQZBXSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_����վ�����豸���ݱ�_1", FilterArray_map8.find(m_selectConditions_DQZBXSJB)->second, m_ChooseIt_DQZBXSB);
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
//����վ����ɸѡ
void CPopDataManage::Filter_DQZTXSJB()
{
	if (m_selectConditions_DQZTXSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("1_����վ����_1", FilterArray_map9.find(m_selectConditions_DQZTXSJB)->second, m_ChooseIt_DQZTX);
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
//�����ն����ݱ�ɸѡ
void CPopDataManage::Filter_DMZDSJB()
{
	if (m_selectConditions_DMZDSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_�����ն����ݱ�_1", FilterArray_map10.find(m_selectConditions_DMZDSJB)->second, m_ChooseIt_DMZDSJB);
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
//�ն��غ����ݱ�ɸѡ
void CPopDataManage::Filter_ZDZHSJBSJB()
{
	if (m_selectConditions_DQZDZHSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_�ն��غ����ݱ�_1", FilterArray_map11.find(m_selectConditions_DQZDZHSJB)->second, m_ChooseIt_ZDZHSJB);
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
//�����豸���ݱ�ɸѡ
void CPopDataManage::Filter_BXSBSJBSJB()
{
	if (m_selectConditions_BXSBSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_�ն˲����豸���ݱ�_1", FilterArray_map12.find(m_selectConditions_BXSBSJB)->second, m_ChooseIt_BXSBSJB);
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
//�ն�����ɸѡ
void CPopDataManage::Filter_ZDTXSJB()
{
	if (m_selectConditions_ZDTXSJB == "��ɸѡ")
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
		vecVec = ConnectMysql::Instance().mytest_QueryByColumnName("2_�ն�����_1", FilterArray_map13.find(m_selectConditions_ZDTXSJB)->second, m_ChooseIt_ZDTX);
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