#include "stdafx.h"
#include "CPopLinkManage.h"
#include <map>
#include <io.h>
#include <direct.h>
#include <fstream>
#include <stdio.h>
#include <cassert>
//#include <Winsock2.h>
int iEditEmptyFlag;
#pragma execution_character_set("utf-8") ;
//vector<string> seleceColumns;
//��·���������ʵ��
DUI_BEGIN_MESSAGE_MAP(CPopLinkManage, WindowImplBase)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMSELECT, OnItemSelect)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
DUI_END_MESSAGE_MAP()
CPopLinkManage::CPopLinkManage(void)
{

}

CPopLinkManage::~CPopLinkManage(void)
{

}
wstring CPopLinkManage::stringToWstring(string str)
{

	wstring w_str;
	UtilTool::setWstring(w_str, str.c_str());
	return w_str;


}


string CPopLinkManage::wstringToString(wstring wstr)
{
	string str;
	UtilTool::setString(str, wstr.c_str());
	return str;
}
string CPopLinkManage::UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��  
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
string CPopLinkManage::string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��
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
void  FuzzyQuery(CComboBoxUI* pCombo, wstring strGoal)  //ģ����ѯ
{

	//OutputDebugString("����ģ����ѯ__ljl");
	string SendData, Getcombodata;
	std::vector<string>  LvNamesOK;
	std::vector<string>  LvNamesUOK;
	UtilTool::setString(SendData, strGoal.c_str());

	if (NULL == SendData.c_str())  //�༭����û������
	{
		iEditEmptyFlag = 0;    //��־λΪ0
		pCombo->RemoveAll();        //���combo box ������ѡ��	   
	}
	else
	{
		iEditEmptyFlag = 1;    //��־λΪ1,���ݴ���
		//����combo box�����������

		for (int i = 0; i < pCombo->GetCount(); i++)          //�ҳ�combo�������м�����
		{
			const char * cstr = NULL;
			wstring getcombodata = pCombo->GetItemAt(i)->GetText();
			UtilTool::setString(Getcombodata, getcombodata.c_str());
			cstr = strstr(Getcombodata.c_str(), SendData.c_str()/*GetData()�������ǽ�CStdStringתconst char*/);
			if (NULL != cstr)   //�����ϵ��������LvNamesOK��
			{
				getcombodata = pCombo->GetItemAt(i)->GetText();
				UtilTool::setString(Getcombodata, getcombodata.c_str());

				LvNamesOK.push_back(Getcombodata);
			}
			else               //�������ϵ��������LvNamesUOK��
			{
				getcombodata = pCombo->GetItemAt(i)->GetText();
				UtilTool::setString(Getcombodata, getcombodata.c_str());
				LvNamesUOK.push_back(Getcombodata);
			}
		}
		//��LvNamesUOK׷�ӵ�LvNamesOK��
		for (vector<string>::iterator it = LvNamesUOK.begin(); it != LvNamesUOK.end(); it++)
		{
			LvNamesOK.push_back((*it));
		}

		pCombo->RemoveAll();        //���combo box ������ѡ��	
		//��combo box���������¸�ֵ
		std::vector<string>::iterator it;
		for (it = LvNamesOK.begin(); it != LvNamesOK.end(); it++)
		{
			CListLabelElementUI* pElement = new CListLabelElementUI;
			if (!pElement)
			{
				//OutputDebugString("����CListLabelElementUI�б�ʧ��");
			}
			wstring comdata;
			UtilTool::setWstring(comdata, (*it).c_str());
			pElement->SetText(comdata.c_str());
			if (!pCombo->Add(pElement))
			{
				//OutputDebugString("combo box�������������ʧ��");
			}
		}
		//pCombo->Activate();
		//pCombo->SelectItem(0);    
	}
}

void CPopLinkManage::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	//delete this;
}
DWORD StringToDword(string val)
{
	DWORD cur_dword;
	sscanf(val.c_str(), "%ul", &cur_dword);
	return cur_dword;
}

DuiLib::CDuiString CPopLinkManage::GetSkinFile()
{
	return _T("CorrectnessTest.xml");//.xml
}


LPCTSTR CPopLinkManage::GetWindowClassName(void) const
{
	return _T("CorrectnessTest");
}
vector<string> CPopLinkManage::my_split(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;

	str += pattern;//��չ�ַ����Է������
	int size = str.size();

	for (int i = 0; i < size; i++) {
		pos = str.find(pattern, i);
		if (pos < size) {
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
void CPopLinkManage::OnClick(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if (msg.pSender == m_pCloseBtn) {
		Close(0);
		return;
	}
	else if (sName.CompareNoCase(_T("bt_Send")) == 0)//���Ͱ�ť
	{
		unsigned char m_SendData[10];
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_User")));
		string User = wstringToString(pControl->GetText().GetData());

		cout << User << endl;

		CControlUI* pControl1 = static_cast<CControlUI*>(m_pm.FindControl(_T("Com_Satellite")));
		string Name_WX = wstringToString(pControl1->GetText().GetData());

		cout << Name_WX << endl;

		//m_Udpadmin1->UDPSendClient((char *)m_SendData, 10);

	}
	else if (sName.CompareNoCase(_T("bt_Delete")) == 0)//ɾ����ť
	{

		
		int num = m_dataBSList->GetCount();
		cout << num << endl;
		cout << SelectListLine << endl;
		int seleteNum = SelectListLine;
		m_dataBSList->RemoveAt(SelectListLine);
		
		int num1 = m_dataBSList->GetCount();
		cout << num1 << endl;
		vector<string> vec_flushed;//ˢ�º��vector

		vector<vector<string> > vec_Vec;
		for (int i = 0; i < vec_LinkData.size(); i++)
		{
			vector<string> vec;
			if (i == seleteNum)
			{
				cout << "�����ѡ��ɾ����" << endl;
			}
			else
			{
				for (int j = 0; j < vec_LinkData[i].size(); j++)
				{
					vec.push_back(vec_LinkData[i][j]);
				}
				vec_Vec.push_back(vec);
			}
			
		}
		vec_LinkData.clear();
		vec_LinkData = vec_Vec;
		cout << vec_LinkData[0][0] << endl;
		
	}
	//else if (sName.CompareNoCase(_T("bt_Send")) == 0)//���Ͱ�ť
	//{
		//vector<string> vec_Link1;
		//vec_Link1.push_back("�û�2");
		//vec_Link1.push_back("�й�1");
		//vec_Link1.push_back("��ͨ1");
		//vec_Link1.push_back("�û�1");
		//vec_Link1.push_back("����1");
		//vec_Link1.push_back("100");
		//vec_Link1.push_back("101");
		//vec_Link1.push_back("102");
		//vec_Link1.push_back("����2");
		//vec_Link1.push_back("�й�1");
		//vec_Link1.push_back("CASC1");
		//vec_Link1.push_back("��1");
		//vec_Link1.push_back("LEO1");
		//vec_Link1.push_back("2011");
		//vec_Link1.push_back("2021");
		//vec_Link1.push_back("2031");
		//vec_Link1.push_back("-159.061");
		//vec_Link1.push_back("8.29");
		//vec_Link1.push_back("69.54");
		//vec_Link1.push_back("2.36");
		//vec_Link1.push_back("��1");
		//vec_Link1.push_back("-154.21");
		//vec_Link1.push_back("7.75");
		//vec_Link1.push_back("4.39");
		//vec_Link1.push_back("2.36");
		//vec_Link1.push_back("��1");
		//ReceiveData(vec_Link1);

	//}
	else if (sName.CompareNoCase(_T("bt_SaveAs")) == 0)////bt_SaveAs ���Ϊ
	{
		cout << "���Ϊ" << endl;

		string path;
		path = "D:\\DATA1\\";
		//bool flag = CreateDirectory(_T("D:\\DATA1\\HH"), NULL);
		m_CPopNewConstellation->MessageBox(NULL);
		string name = m_CPopNewConstellation->GetNewName();//	string GetNewName();//��ȡ�޸ĺ������
		path += name;
		bool flag = CreateDirectory(stringToWstring(path).c_str(), NULL);
		//vec_LinkData
		cout << vec_LinkData[0][0] << endl;
		
		//��List�е����ݴ洢
		for (int i = 0; i < vec_LinkData.size(); i++)
		{
			string fileName = vec_LinkData[i][0] + vec_LinkData[i][8];
			GenerateTheFile(path, fileName, vec_LinkData[i]);
		}
		//������û���Combo����Ĵ洢
		GenerateTheFile(path, UTF8_To_string("�û�"), vec_localUser);
		//��������ǵ�Combo�������Ϣ�洢
		GenerateTheFile(path, UTF8_To_string("����"), vec_localSatellite);
		
	}

	else if (sName.CompareNoCase(_T("bt_Input")) == 0)//����
	{
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_User")));
		select_combox->RemoveAll();
		CComboBoxUI* select_combox1 = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_Satellite")));
		select_combox1->RemoveAll();

		vec_localUser.clear();//����û�����
		vec_localSatellite.clear();//�����������

		vec_LinkData.clear();
		m_dataBSList->RemoveAll();
		//RefreshTheInterface_tab2();
		//HideThCcontrol_2(ControlNumber);
		COptionUI* pControl = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_ScenarioData")));//��������
		if (pControl->IsSelected())
		{
			cout << "��·������泡������" << endl;


		}
		COptionUI* pControl_1 = static_cast<COptionUI*>(m_pm.FindControl(_T("optical_LocalData")));//��������
		if (pControl_1->IsSelected())
		{
			cout << "��·������汾������" << endl;
			ImportingLocalData();//���뱾������
		}
	}

}

void CPopLinkManage::OnSelectChanged(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

}

void CPopLinkManage::OnItemSelect(TNotifyUI &msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

LRESULT CPopLinkManage::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT CPopLinkManage::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


string  CPopLinkManage::get_cur_timelogdata()/*��ȡ��ǰʱ���ַ���*/
{
	string time_string;
	SYSTEMTIME stime;
	char buf[100] = { 0 };
	GetLocalTime(&stime);
	sprintf(buf, "%04d.%02d.%02d.%02d.%02d.%02d", stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	time_string += buf;
	return time_string;
}
void CPopLinkManage::OnLClick(CControlUI *pControl)
{

	CDuiString sName = pControl->GetName();

	//	if(pTabSwitch != NULL)
	//CGameBoxFrame:pTabSwitch->SelectItem(FirstTab);
}
void CPopLinkManage::Notify(TNotifyUI &msg)
{
	CDuiString strControlName = msg.pSender->GetName();
	if (msg.sType == _T("click"))
	{

		// ��ť��Ϣ
		OnLClick(msg.pSender);
	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("Com_FilterCriteria"))////ѡ��ɸѡ����
	{
		if (msg.pSender->GetName() != _T("Com_FilterCriteria"))
		{
			return;
		}
		CComboUI* pCombox = (CComboUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_FilterCriteria")));
		cout << parameters[nsel] << endl;
		string type = parameters[nsel];
		m_selectConditions = parameters[nsel];
		cout << type << endl;

	}
	else if (msg.sType == L"textchanged" && msg.pSender->GetName() == _T("edit_Conditions"))
	{
		CDuiString Test_Name;
		Test_Name = m_pm.FindControl(L"edit_Conditions")->GetText();
		string str;
		UtilTool::setString(str, Test_Name.GetData());

	}
	else if (msg.sType == _T("itemselect") && msg.pSender->GetName() == _T("list_DataShow"))
	{
		if (msg.pSender->GetName() != _T("list_DataShow"))//���ݱ�
		{
			return;
		}
		cout << "111111111111" << endl;
		CListUI* pCombox = (CListUI*)msg.pSender;
		int nsel = pCombox->GetCurSel();
		SelectListLine = nsel;
		cout << nsel << endl;

	}
	else if (msg.sType == _T("textchanged") && msg.pSender->GetName() == _T("edit_UserFuzzyQueries"))//ģ����ѯ�û�
	{
		CComboBoxUI  * m_pComOprat = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_User")));

		wstring text = msg.pSender->GetText();
		FuzzyQuery(m_pComOprat, text/*��ȡedit�ؼ�����*/);
	}
	else if (msg.sType == _T("textchanged") && msg.pSender->GetName() == _T("edit_SatelliteFuzzyQueries"))//ģ����ѯ�û�
	{
		CComboBoxUI  * m_pComOprat = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_Satellite")));

		wstring text = msg.pSender->GetText();
		FuzzyQuery(m_pComOprat, text/*��ȡedit�ؼ�����*/);
	}

	return WindowImplBase::Notify(msg);
}
unsigned __stdcall CPopLinkManage::Busys(void* param)
{

	CPopLinkManage * pWnd = (CPopLinkManage*)param;
	::PostMessage(pWnd->m_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


void CPopLinkManage::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	m_dataBSList = static_cast<CListUI*>(m_pm.FindControl(_T("list_DataShow")));//��ʾ���ݵ�List

	m_Udpadmin1 = new UDPAdmin1;//�������յ������ݷ��͸��ܿ�
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UDPSendSerialPortThread, this, 0, NULL);//������ͨ�Ž��յ������ݷ��͸��ܿ�
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UDPRecvZKThread, this, 0, NULL);//�����ܿط��͵�����

	//InitializeList();//�����list����������
	//InitializeCombo_link();

	vector<string> vec_User;
	vec_User.push_back(UTF8_To_string("����վ0"));
	vec_User.push_back(UTF8_To_string("����վ1"));
	vec_User.push_back(UTF8_To_string("����վ2"));
	vec_User.push_back(UTF8_To_string("����վ3"));
	vec_User.push_back(UTF8_To_string("�����ն�0"));
	vec_User.push_back(UTF8_To_string("�����ն�1"));
	vec_User.push_back(UTF8_To_string("�����ն�2"));
	vec_User.push_back(UTF8_To_string("�����ն�3"));
	InitializeCombo_User(vec_User);

	vector<string> vec_Satellite;
	vec_Satellite.push_back(UTF8_To_string("����0"));
	vec_Satellite.push_back(UTF8_To_string("����1"));
	vec_Satellite.push_back(UTF8_To_string("����2"));
	vec_Satellite.push_back(UTF8_To_string("����3"));
	vec_Satellite.push_back(UTF8_To_string("����4"));
	vec_Satellite.push_back(UTF8_To_string("����5"));
	InitializeCombo_Satellite(vec_Satellite);

	vector<string> vec_Link;
	vec_Link.push_back(UTF8_To_string("����վ1"));
	vec_Link.push_back(UTF8_To_string("�й�"));
	vec_Link.push_back(UTF8_To_string("��ͨ"));
	vec_Link.push_back(UTF8_To_string("�û�"));
	vec_Link.push_back(UTF8_To_string("����"));
	vec_Link.push_back(UTF8_To_string("100"));
	vec_Link.push_back(UTF8_To_string("101"));
	vec_Link.push_back(UTF8_To_string("102"));
	vec_Link.push_back(UTF8_To_string("����1"));
	vec_Link.push_back(UTF8_To_string("�й�"));
	vec_Link.push_back(UTF8_To_string("CASC"));
	vec_Link.push_back(UTF8_To_string("��"));
	vec_Link.push_back(UTF8_To_string("LEO"));
	vec_Link.push_back(UTF8_To_string("201"));
	vec_Link.push_back(UTF8_To_string("202"));
	vec_Link.push_back(UTF8_To_string("203"));
	vec_Link.push_back(UTF8_To_string("-159.06"));
	vec_Link.push_back(UTF8_To_string("8.29"));
	vec_Link.push_back(UTF8_To_string("69.54"));
	vec_Link.push_back(UTF8_To_string("2.36"));
	vec_Link.push_back(UTF8_To_string("��"));
	vec_Link.push_back(UTF8_To_string("-154.21"));
	vec_Link.push_back(UTF8_To_string("7.75"));
	vec_Link.push_back(UTF8_To_string("4.39"));
	vec_Link.push_back(UTF8_To_string("2.36"));
	vec_Link.push_back(UTF8_To_string("��"));
	ReceiveData(vec_Link);//ˢ�º�д�붼���������

	vector<string> vec_Link1;
	vec_Link1.push_back(UTF8_To_string("�����ն�2"));
	vec_Link1.push_back(UTF8_To_string("�й�"));
	vec_Link1.push_back(UTF8_To_string("��ͨ"));
	vec_Link1.push_back(UTF8_To_string("�û�"));
	vec_Link1.push_back(UTF8_To_string("����"));
	vec_Link1.push_back(UTF8_To_string("100"));
	vec_Link1.push_back(UTF8_To_string("101"));
	vec_Link1.push_back(UTF8_To_string("102"));
	vec_Link1.push_back(UTF8_To_string("����2"));
	vec_Link1.push_back(UTF8_To_string("�й�"));
	vec_Link1.push_back(UTF8_To_string("CASC"));
	vec_Link1.push_back(UTF8_To_string("��"));
	vec_Link1.push_back(UTF8_To_string("LEO"));
	vec_Link1.push_back(UTF8_To_string("201"));
	vec_Link1.push_back(UTF8_To_string("202"));
	vec_Link1.push_back(UTF8_To_string("203"));
	vec_Link1.push_back(UTF8_To_string("-159.06"));
	vec_Link1.push_back(UTF8_To_string("8.29"));
	vec_Link1.push_back(UTF8_To_string("69.54"));
	vec_Link1.push_back(UTF8_To_string("2.36"));
	vec_Link1.push_back(UTF8_To_string("��"));
	vec_Link1.push_back(UTF8_To_string("-154.21"));
	vec_Link1.push_back(UTF8_To_string("7.75"));
	vec_Link1.push_back(UTF8_To_string("4.39"));
	vec_Link1.push_back(UTF8_To_string("2.36"));
	vec_Link1.push_back(UTF8_To_string("��"));
	ReceiveData(vec_Link1);
}
UINT WINAPI CPopLinkManage::UDPSendSerialPortThread(void* pParam)
{
	CPopLinkManage * pMainWnd = (CPopLinkManage*)pParam;

	pMainWnd->m_Udpadmin1->InitClient(8088, "127.0.0.1");
	

	return 0;
}

UINT WINAPI CPopLinkManage::UDPRecvZKThread(void* pParam)
{
	CPopLinkManage * pMainWnd = (CPopLinkManage*)pParam;
	unsigned  char m_DataBuffer[44000] = { 0 };
	pMainWnd->m_UdpRecvClass = new UdpRecvClass();

	pMainWnd->m_UdpRecvClass->InitServer(8001);
	int m_DataLength = 0;
	while (1)
	{
		
		memset(m_DataBuffer, 0, sizeof(m_DataBuffer));  //��������ڴ�����ʼ��
		m_DataLength = pMainWnd->m_UdpRecvClass->UDPsRecv((char*)m_DataBuffer);

		if (m_DataLength > 0)
		{
			cout << "��������!!!" << endl;
			//pMainWnd->m_dataBSList->RemoveAll();
			//for (int i = 0; i < m_DataLength / 440; i++)
			//{
			//	memcpy(&(pMainWnd->m_EffectiveLinkTable), m_DataBuffer+440*i, sizeof(EffectiveLinkTable));
			//	cout << "1111111" << endl;
			//	pMainWnd->ReceiveDataLocally(pMainWnd->m_EffectiveLinkTable);
			//}
			//pMainWnd->UpdatingTheInterface();

		}
		Sleep(2);
	}

	return 0;
}
void CPopLinkManage::InitializeList()
{
	/**********************ģ������н����б��е�����***********************/
	SIZE            szScrollRange;
	SIZE            szScrollPos;
	szScrollRange = m_dataBSList->GetScrollRange();
	szScrollPos = m_dataBSList->GetScrollPos();
	szScrollPos.cy = szScrollRange.cy;
	m_dataBSList->SetScrollPos(szScrollPos);

	string datacs;
	wstring text_string;

	CListTextElementUI* pItem = new CListTextElementUI();
	m_dataBSList->Add(pItem);

	datacs = "�û�1";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(0, text_string.c_str());

	datacs = "�й�";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(1, text_string.c_str());

	datacs = "��ͨ";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(2, text_string.c_str());

	datacs = "�û�";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(3, text_string.c_str());

	datacs = "����";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(4, text_string.c_str());

	datacs = "100";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(5, text_string.c_str());

	datacs = "101";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(6, text_string.c_str());

	datacs = "102";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(7, text_string.c_str());

	datacs = "����1";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(8, text_string.c_str());

	datacs = "�й�";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(9, text_string.c_str());

	datacs = "CASC";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(10, text_string.c_str());

	datacs = "��";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(11, text_string.c_str());

	datacs = "LEO";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(12, text_string.c_str());

	datacs = "-159.06";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(13, text_string.c_str());

	datacs = "8.29";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(14, text_string.c_str());

	datacs = "69.54";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(15, text_string.c_str());

	datacs = "201";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(16, text_string.c_str());

	datacs = "202";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(17, text_string.c_str());

	datacs = "203";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(18, text_string.c_str());

	datacs = "2.36";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(19, text_string.c_str());

	datacs = "��";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(20, text_string.c_str());

	datacs = "-154.21";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(21, text_string.c_str());

	datacs = "7.75";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(22, text_string.c_str());

	datacs = "74.39";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(23, text_string.c_str());

	datacs = "2.36";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(24, text_string.c_str());

	datacs = "��";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(25, text_string.c_str());


	pItem = new CListTextElementUI();
	m_dataBSList->Add(pItem);

	datacs = "����վ1";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(0, text_string.c_str());

	datacs = "�й�";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(1, text_string.c_str());

	datacs = "��ͨ";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(2, text_string.c_str());

	datacs = "���Ÿ�";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(3, text_string.c_str());

	datacs = "����";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(4, text_string.c_str());

	datacs = "300";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(5, text_string.c_str());

	datacs = "301";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(6, text_string.c_str());

	datacs = "302";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(7, text_string.c_str());


	datacs = "����2";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(8, text_string.c_str());

	datacs = "�й�";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(9, text_string.c_str());

	datacs = "CASC";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(10, text_string.c_str());

	datacs = "��";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(11, text_string.c_str());

	datacs = "LEO";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(12, text_string.c_str());

	datacs = "401";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(13, text_string.c_str());

	datacs = "402";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(14, text_string.c_str());

	datacs = "403";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(15, text_string.c_str());

	datacs = "-142.41";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(16, text_string.c_str());

	datacs = "7.65";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(17, text_string.c_str());

	datacs = "86.19";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(18, text_string.c_str());

	datacs = "2";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(19, text_string.c_str());

	datacs = "��";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(20, text_string.c_str());

	datacs = "-151.97";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(21, text_string.c_str());

	datacs = "7.65";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(22, text_string.c_str());

	datacs = "76.63";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(23, text_string.c_str());

	datacs = "1.46";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(24, text_string.c_str());

	datacs = "��";
	UtilTool::setWstring(text_string, datacs.c_str());
	pItem->SetText(25, text_string.c_str());
}
void CPopLinkManage::InitializeList(vector<string> vec)//��List������Ӷ���
{

	vec_LinkData.push_back(vec);
	SIZE            szScrollRange;
	SIZE            szScrollPos;
	szScrollRange = m_dataBSList->GetScrollRange();
	szScrollPos = m_dataBSList->GetScrollPos();
	szScrollPos.cy = szScrollRange.cy;
	m_dataBSList->SetScrollPos(szScrollPos);

	string datacs;
	wstring text_string;
	CListTextElementUI* pItem = new CListTextElementUI();
	m_dataBSList->Add(pItem);
	for (int i = 0; i < vec.size(); i++)
	{

		UtilTool::setWstring(text_string, vec[i].c_str());
		pItem->SetText(i, text_string.c_str());


	}


}

//����list�е�����
void CPopLinkManage::UpdateList(vector<vector<string>> vec)
{
	SIZE            szScrollRange;
	SIZE            szScrollPos;
	szScrollRange = m_dataBSList->GetScrollRange();
	szScrollPos = m_dataBSList->GetScrollPos();
	szScrollPos.cy = szScrollRange.cy;
	m_dataBSList->SetScrollPos(szScrollPos);

	for (int i = 0; i < vec.size(); i++)
	{
		string datacs;
		wstring text_string;
		CListTextElementUI* pItem = new CListTextElementUI();
		m_dataBSList->Add(pItem);

		for (int j = 0; j < vec[i].size(); j++)
		{
			UtilTool::setWstring(text_string, vec[i][j].c_str());
			pItem->SetText(j, text_string.c_str());
		}
	}




}

//�������ݲ�����
void CPopLinkManage::ReceiveData(vector<string> vec)
{
	bool flag = false;
	int sequence = -1;
	for (int i = 0; i < vec_LinkData.size(); i++)
	{
		if (vec[0] == vec_LinkData[i][0] && vec[8] == vec_LinkData[i][8])
		{
			cout << "�Ѿ������������-----��Ҫ�滻" << endl;
			flag = true;
			sequence = i;
		}
	}

	if (flag)
	{
		vec_LinkData[sequence] = vec;
		m_dataBSList->RemoveAll();
		UpdateList(vec_LinkData);
	}
	else
	{
		InitializeList(vec);
	}
}
//��combox������Ӷ���
void CPopLinkManage::InitializeCombo_link()
{
	select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_User")));
	vector<string> vec_User;
	vec_User.push_back("����վ0");
	vec_User.push_back("����վ1");
	vec_User.push_back("����վ2");
	vec_User.push_back("����վ3");
	vec_User.push_back("�����ն�0");
	vec_User.push_back("�����ն�1");
	vec_User.push_back("�����ն�2");
	vec_User.push_back("�����ն�3");

	for (int i = 0; i < vec_User.size(); i++)
	{
		wstring temp;
		CListLabelElementUI * pLabel = new CListLabelElementUI();
		UtilTool::setWstring(temp, vec_User[i].c_str());
		pLabel->SetText(temp.c_str());
		pLabel->SetBkColor(0xff0000);
		if (select_combox)
			select_combox->AddAt(pLabel, i);
	}
	select_combox->SelectItem(0);
}
//���û���combo�Ӷ���
void CPopLinkManage::InitializeCombo_User(vector<string> vec)
{

	select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_User")));


	for (int i = 0; i < vec.size(); i++)
	{
		vec_localUser.push_back(vec[i]);
		wstring temp;
		CListLabelElementUI * pLabel = new CListLabelElementUI();
		UtilTool::setWstring(temp, vec[i].c_str());
		pLabel->SetText(temp.c_str());
		pLabel->SetBkColor(0xff0000);
		if (select_combox)
			select_combox->AddAt(pLabel, i);
	}
	select_combox->SelectItem(0);
}
//���û���combo�Ӷ���
void CPopLinkManage::InitializeCombo_Satellite(vector<string> vec)
{
	select_combox = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("Com_Satellite")));

	for (int i = 0; i < vec.size(); i++)
	{
		vec_localSatellite.push_back(vec[i]);
		wstring temp;
		CListLabelElementUI * pLabel = new CListLabelElementUI();
		UtilTool::setWstring(temp, vec[i].c_str());
		pLabel->SetText(temp.c_str());
		pLabel->SetBkColor(0xff0000);
		if (select_combox)
			select_combox->AddAt(pLabel, i);
	}
	select_combox->SelectItem(0);
}





//���ṹ�����ݽ�����ʾ�ڽ���List��
void CPopLinkManage::ParsingDisplay()
{
	CListTextElementUI* pItem = new CListTextElementUI();
	m_dataBSList->Add(pItem);

	SIZE            szScrollRange;
	SIZE            szScrollPos;
	szScrollRange = m_dataBSList->GetScrollRange();
	szScrollPos = m_dataBSList->GetScrollPos();
	szScrollPos.cy = szScrollRange.cy;
	m_dataBSList->SetScrollPos(szScrollPos);

	string datacs;
	wstring text_string;

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.userTable.Name_YH.c_str());
	pItem->SetText(0, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.userTable.Country_YH.c_str());
	pItem->SetText(1, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.userTable.Affiliation_YH.c_str());
	pItem->SetText(2, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.userTable.Category_YH.c_str());
	pItem->SetText(3, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.userTable.UserNature_YH.c_str());
	pItem->SetText(4, text_string.c_str());


	//string Name_WX = "��������";//��������
	//string Country_WX = "��������";//��������--����
	//string Affiliation_WX = "��������";//��������
	//string PublicInformation_WX = "������Ϣ";//�Ƿ���Ϣ����
	//string TrackType_WX = "������";//������
	UtilTool::setWstring(text_string, m_EffectiveLinkTable.satelliteTable.Name_WX.c_str());
	pItem->SetText(5, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.satelliteTable.Country_WX.c_str());
	pItem->SetText(6, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.satelliteTable.Affiliation_WX.c_str());
	pItem->SetText(7, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.satelliteTable.PublicInformation_WX.c_str());
	pItem->SetText(8, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.satelliteTable.TrackType_WX.c_str());
	pItem->SetText(9, text_string.c_str());

	//string CT_SX = "����C/T";//������·״̬--C/T
	//string CN_SX = "����C/N";//C/N--������·״̬
	//string CN0_SX = "����CN0";//C/N0
	//string LinkMargin_SX = "��·����";//��·����
	//string CommunicationQuality_SX = "ͨ������";//ͨ������
	std::string str = std::to_string(m_EffectiveLinkTable.uplinkState.CT_SX);
	UtilTool::setWstring(text_string, str.c_str());
	pItem->SetText(10, text_string.c_str());

	std::string str1 = std::to_string(m_EffectiveLinkTable.uplinkState.CN_SX);
	UtilTool::setWstring(text_string, str1.c_str());
	pItem->SetText(11, text_string.c_str());

	std::string str2 = std::to_string(m_EffectiveLinkTable.uplinkState.CN0_SX);
	UtilTool::setWstring(text_string, str2.c_str());
	pItem->SetText(12, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.uplinkState.LinkMargin_SX.c_str());
	pItem->SetText(13, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.uplinkState.CommunicationQuality_SX.c_str());
	pItem->SetText(14, text_string.c_str());

	//string CT_XX = "����C/T";//������·״̬--C/T
	//string CN_XX = "����C/N";//C/N--������·״̬
	//string CN0_XX = "����C/N0";//C/N0
	//string LinkMargin_XX = "��·����";//��·����
	//string CommunicationQuality_XX = "ͨ������";//ͨ������
	std::string str3 = std::to_string(m_EffectiveLinkTable.downlinkState.CT_XX);
	UtilTool::setWstring(text_string, str3.c_str());
	pItem->SetText(15, text_string.c_str());

	std::string str4 = std::to_string(m_EffectiveLinkTable.downlinkState.CN_XX);
	UtilTool::setWstring(text_string,str4.c_str());
	pItem->SetText(16, text_string.c_str());

	std::string str5 = std::to_string(m_EffectiveLinkTable.downlinkState.CN0_XX);
	UtilTool::setWstring(text_string, str5.c_str());
	pItem->SetText(17, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.downlinkState.LinkMargin_XX.c_str());
	pItem->SetText(18, text_string.c_str());

	UtilTool::setWstring(text_string, m_EffectiveLinkTable.downlinkState.CommunicationQuality_XX.c_str());
	pItem->SetText(19, text_string.c_str());
}
void CPopLinkManage::ParsingDisplay(EffectiveLinkTable table)
{
	CListTextElementUI* pItem = new CListTextElementUI();
	m_dataBSList->Add(pItem);

	SIZE            szScrollRange;
	SIZE            szScrollPos;
	szScrollRange = m_dataBSList->GetScrollRange();
	szScrollPos = m_dataBSList->GetScrollPos();
	szScrollPos.cy = szScrollRange.cy;
	m_dataBSList->SetScrollPos(szScrollPos);

	string datacs;
	wstring text_string;

	UtilTool::setWstring(text_string, table.userTable.Name_YH.c_str());
	pItem->SetText(0, text_string.c_str());

	UtilTool::setWstring(text_string, table.userTable.Country_YH.c_str());
	pItem->SetText(1, text_string.c_str());

	UtilTool::setWstring(text_string, table.userTable.Affiliation_YH.c_str());
	pItem->SetText(2, text_string.c_str());

	UtilTool::setWstring(text_string, table.userTable.Category_YH.c_str());
	pItem->SetText(3, text_string.c_str());

	UtilTool::setWstring(text_string, table.userTable.UserNature_YH.c_str());
	pItem->SetText(4, text_string.c_str());


	//string Name_WX = "��������";//��������
	//string Country_WX = "��������";//��������--����
	//string Affiliation_WX = "��������";//��������
	//string PublicInformation_WX = "������Ϣ";//�Ƿ���Ϣ����
	//string TrackType_WX = "������";//������
	UtilTool::setWstring(text_string, table.satelliteTable.Name_WX.c_str());
	pItem->SetText(5, text_string.c_str());

	UtilTool::setWstring(text_string, table.satelliteTable.Country_WX.c_str());
	pItem->SetText(6, text_string.c_str());

	UtilTool::setWstring(text_string, table.satelliteTable.Affiliation_WX.c_str());
	pItem->SetText(7, text_string.c_str());

	UtilTool::setWstring(text_string, table.satelliteTable.PublicInformation_WX.c_str());
	pItem->SetText(8, text_string.c_str());

	UtilTool::setWstring(text_string, table.satelliteTable.TrackType_WX.c_str());
	pItem->SetText(9, text_string.c_str());

	//string CT_SX = "����C/T";//������·״̬--C/T
	//string CN_SX = "����C/N";//C/N--������·״̬
	//string CN0_SX = "����CN0";//C/N0
	//string LinkMargin_SX = "��·����";//��·����
	//string CommunicationQuality_SX = "ͨ������";//ͨ������
	std::string str = std::to_string(table.uplinkState.CT_SX);
	UtilTool::setWstring(text_string, str.c_str());
	pItem->SetText(10, text_string.c_str());

	std::string str1 = std::to_string(table.uplinkState.CN_SX);
	UtilTool::setWstring(text_string, str1.c_str());
	pItem->SetText(11, text_string.c_str());

	std::string str2 = std::to_string(table.uplinkState.CN0_SX);
	UtilTool::setWstring(text_string, str2.c_str());
	pItem->SetText(12, text_string.c_str());

	UtilTool::setWstring(text_string, table.uplinkState.LinkMargin_SX.c_str());
	pItem->SetText(13, text_string.c_str());

	UtilTool::setWstring(text_string, table.uplinkState.CommunicationQuality_SX.c_str());
	pItem->SetText(14, text_string.c_str());

	//string CT_XX = "����C/T";//������·״̬--C/T
	//string CN_XX = "����C/N";//C/N--������·״̬
	//string CN0_XX = "����C/N0";//C/N0
	//string LinkMargin_XX = "��·����";//��·����
	//string CommunicationQuality_XX = "ͨ������";//ͨ������
	std::string str3 = std::to_string(table.downlinkState.CT_XX);
	UtilTool::setWstring(text_string, str3.c_str());
	pItem->SetText(15, text_string.c_str());

	std::string str4 = std::to_string(table.downlinkState.CN_XX);
	UtilTool::setWstring(text_string, str4.c_str());
	pItem->SetText(16, text_string.c_str());

	std::string str5 = std::to_string(table.downlinkState.CN0_XX);
	UtilTool::setWstring(text_string, str5.c_str());
	pItem->SetText(17, text_string.c_str());

	UtilTool::setWstring(text_string, table.downlinkState.LinkMargin_XX.c_str());
	pItem->SetText(18, text_string.c_str());

	UtilTool::setWstring(text_string, table.downlinkState.CommunicationQuality_XX.c_str());
	pItem->SetText(19, text_string.c_str());
}
//��List���������
void CPopLinkManage::AddingDataToList(int num)
{
	//CListTextElementUI* pItem = new CListTextElementUI();
	//m_dataBSList->Add(pItem);

	//SIZE            szScrollRange;
	//SIZE            szScrollPos;
	//szScrollRange = m_dataBSList->GetScrollRange();
	//szScrollPos = m_dataBSList->GetScrollPos();
	//szScrollPos.cy = szScrollRange.cy;
	//m_dataBSList->SetScrollPos(szScrollPos);

	//string datacs;
	//wstring text_string;

	//std::string str = std::to_string(num);

	//datacs = "111_" + str;
	//UtilTool::setWstring(text_string, datacs.c_str());
	//pItem->SetText(0, text_string.c_str());

	//datacs = "222_" + str;
	//UtilTool::setWstring(text_string, datacs.c_str());
	//pItem->SetText(1, text_string.c_str());

	//datacs = "333_" + str;
	//UtilTool::setWstring(text_string, datacs.c_str());
	//pItem->SetText(2, text_string.c_str());

	//datacs = "444" + str;
	//UtilTool::setWstring(text_string, datacs.c_str());
	//pItem->SetText(3, text_string.c_str());

	//datacs = "555_" + str;
	//UtilTool::setWstring(text_string, datacs.c_str());
	//pItem->SetText(4, text_string.c_str());

	//datacs = "666_" + str;
	//UtilTool::setWstring(text_string, datacs.c_str());
	//pItem->SetText(5, text_string.c_str());
}

//�����ļ�
void CPopLinkManage::GenerateTheFile(string path, string filename, vector<string> m_vec)
{
	//string path1 = "D:\\DATA1";
	if (0 != _access(path.c_str(), 0))
	{
		_mkdir(path.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	string logdata = path + "\\" + filename + ".txt";

	ofstream fout(logdata, ios::out);

	for (int i = 0; i < m_vec.size(); i++)
	{
		string str = m_vec[i];
		fout << str << endl;
	}

}

void CPopLinkManage::ImportingLocalData()
{
	vector<vector<string> > vecVec_Data;//���List��vector
	vector<string> vec_User;//�û�
	vector<string> vec_Satellite;//����

	vector<wstring> upload_fileName;
	upload_fileName = UtilTool::GetOpenFilePaths_txt();

	/*******************���ļ�����***************************/
	for (int i = 0; i < upload_fileName.size(); i++)
	{
		vector<string> vec_ListInform;
		string fileN;
		UtilTool::setString(fileN, upload_fileName[i].c_str());

		vector<string> str_xzFileName;
		string fgf = "\\";
		str_xzFileName = my_split(fileN, fgf);
		string fileName = str_xzFileName[str_xzFileName.size() - 1];

		vector<string> str_xzName;
		fgf = ".";
		str_xzName = my_split(fileName, fgf);
		string Name = str_xzName[str_xzName.size() - 1];
		if (str_xzName[0] == UTF8_To_string("�û�"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;
				bool flag = true;
				string type;
				while (getline(infile, s))
				{

					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vec_User.push_back(s);

				}


				infile.close();             //�ر��ļ������� 
			}
		}
		else if (str_xzName[0] == UTF8_To_string("����"))
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;
				bool flag = true;
				string type;
				while (getline(infile, s))
				{

					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vec_Satellite.push_back(s);

				}


				infile.close();             //�ر��ļ������� 
			}
		}
		else
		{
			ifstream fin(fileN, ios::binary);
			unsigned char  s2;
			fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
			int p = s2 << 8;
			fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
			p += s2;

			string code;
			fin.close();

			ifstream infile;
			infile.open(fileN);   //���ļ����������ļ��������� 
			//assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
			if (infile)
			{
				string s;
				map<string, string> l_map;
				bool flag = true;
				string type;
				while (getline(infile, s))
				{

					cout << "�ڶ���" << endl;
					cout << s.c_str() << endl;
					vec_ListInform.push_back(s);

				}

				vecVec_Data.push_back(vec_ListInform);
				cout << vec_ListInform[0] << endl;

				infile.close();             //�ر��ļ������� 
			}
		}


	}



	/****************�ڽ�����ʾList�е�����*****************/
	if (vecVec_Data.size() > 0)
	{
		for (int i = 0; i < vecVec_Data.size(); i++)
		{
			ReceiveData(vecVec_Data[i]);
		}
	}

	if (vec_User.size() > 0) 
	{
		InitializeCombo_User(vec_User);
	}
	if (vec_Satellite.size() > 0)
	{
		InitializeCombo_Satellite(vec_Satellite);
	}


}