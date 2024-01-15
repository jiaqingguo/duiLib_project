#include "StdAfx.h"
#include "SplashWnd.h"
#include <process.h>
//#include <Windows.h>
#include <iostream>
#include <fstream>


//#include "ConnectMysql.h"
//#include "Common.h"
#pragma comment(lib,"Mpr.lib")
#pragma comment(lib, "../Mysql/lib/debug/libmysql.lib")
using namespace std;


//////////////////////////////////////////////////////////////////////////
///

DUI_BEGIN_MESSAGE_MAP(CSplashWnd, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_END_MESSAGE_MAP()
HANDLE ahd=NULL;
char sendBuff[255]={0};


//DynamicOutData m_dynamic;

int FirstTab;
vector<string>SQXdata;
//CSerialPort mySerialPort;


#define EOL "\r\n"
#define PRINT_Stat(name)    oss << #name << " : " << stat.name << EOL


FILE * lOG;

HANDLE Thread_lock = CreateEvent(NULL, FALSE, FALSE, _T("�߳�1ͬ����"));
CSplashWnd::CSplashWnd(void)
{
	//m_isData = FALSE;
}

CSplashWnd::~CSplashWnd(void)
{

//	delete m_CPopLinkManage;


}

void CSplashWnd::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CSplashWnd::GetSkinFile()
{
	return _T("splash.xml");//("CorrectnessTest.xml");
}

LPCTSTR CSplashWnd::GetWindowClassName( void ) const
{
	return _T("SplashWnd");
}
LRESULT CSplashWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int  id_event = (int)wParam;
	if (id_event == 1)
	{
		HWND hwnd = m_pm.GetPaintWindow();
		KillTimer(hwnd, 1);
		CListUI *pList = static_cast<CListUI*>(m_pm.FindControl(_T("state")));
		if (pList == nullptr) return 0;
		SIZE            szScrollRange;
		SIZE            szScrollPos;
		szScrollRange = pList->GetScrollRange();
		szScrollPos = pList->GetScrollPos();


		szScrollPos.cy = szScrollRange.cy;


		pList->SetScrollPos(szScrollPos);


		bHandled = TRUE;
	}
	return 0;
}

void CSplashWnd::OnLClick( CControlUI *pControl)
{
	
	CDuiString sName = pControl->GetName();
	if (sName.CompareNoCase(_T("bt_LinkManagement")) == 0)//��·�������
	{
		m_CPopLinkManage = new CPopLinkManage();
		m_CPopLinkManage->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 1000, 800);
		m_CPopLinkManage->CenterWindow();
	}
	else if (sName.CompareNoCase(_T("bt_NetworkManagement")) == 0)//����������
	{
		m_CPopNetworkManage = new CPopNetworkManage();
		m_CPopNetworkManage->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 1000, 800);
		m_CPopNetworkManage->CenterWindow();

	}
	else if (sName.CompareNoCase(_T("bt_DataManagement")) == 0)//���ݹ������
	{
		m_CPopDataManage = new CPopDataManage();
		m_CPopDataManage->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 1000, 800);
		m_CPopDataManage->CenterWindow();

	}
	else if (sName.CompareNoCase(_T("bt_EditorialInterface")) == 0)// �༭����
	{
		m_CPopEditorialClass = new CPopEditorialClass();
		m_CPopEditorialClass->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 1000, 800);
		m_CPopEditorialClass->CenterWindow();

	}
	 
}


void CSplashWnd::Notify(TNotifyUI& msg)
{

	CDuiString name = msg.pSender->GetName();
	int pTabPanel=0;
	
    if( msg.sType == _T("click") )
	{
		// ��ť��Ϣ
		OnLClick(msg.pSender);
	}

	return WindowImplBase::Notify(msg);
}


LRESULT CSplashWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
	 bHandled = FALSE;
	 return 0;
 }

LRESULT CSplashWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0L;
}


void Busy( void* param )  
{  
	/*int i = 0;
    while(i++ < 5) 
	{
		Sleep(300);
	}
	::PostMessage((HWND)param, WM_CLOSE, 0, 0);*/
}  


void CSplashWnd::StringtoHex(BYTE *GB, int glen, BYTE* SendB, int* slen)
{
	int i;    //����������ַ���
	int a = 0;
	char temp;   //�����ַ��������ж��Ƿ�Ϊ�ո�����������
	char temp1, temp2;   //����һ���ֽڵ������ַ�  ����EB����temp1='E',temp2 = 'B'
	*slen = 0;  //�����16�����ַ�������
	for (i = 0; i < glen; i++)
	{
		temp = GB[i];
		if (temp == ' ')
			continue;

		if (a == 0)
			temp1 = GB[i];
		if (a == 1)
			temp2 = GB[i];
		a++;

		if (a == 2)
		{
			a = 0;
			temp1 = temp1 - '0';
			if (temp1 > 10)
				temp1 = temp1 - 7;
			temp2 = temp2 - '0';
			if (temp2 > 10)
				temp2 = temp2 - 7;

			SendB[*slen] = temp1 * 16 + temp2;
			(*slen)++;
		}
	}

}


void CSplashWnd::InitWindow()
{

	cout << "1111111" << endl;
}




void CSplashWnd::getFiles(string path, vector<string>& files)
{
        //�ļ����  
		long   hFile = 0;
		//�ļ���Ϣ������һ���洢�ļ���Ϣ�Ľṹ��  
		struct _finddata_t fileinfo;
		string p;//�ַ��������·��
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//�����ҳɹ��������
			{
				do
				  {
					 //�����Ŀ¼,����֮�����ļ����ڻ����ļ��У�  
					   if ((fileinfo.attrib &  _A_SUBDIR))
						 {
							 //�ļ���������"."&&�ļ���������".."
							 //.��ʾ��ǰĿ¼
							 //..��ʾ��ǰĿ¼�ĸ�Ŀ¼
							 //�ж�ʱ�����߶�Ҫ���ԣ���Ȼ�����޵ݹ�������ȥ�ˣ�
							     if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						         getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
					         }
					        //�������,�����б�  
				        else
					      {
							     //files.push_back(p.assign(path).append("\\").append(fileinfo.name));
						         files.push_back(fileinfo.name);
				   }
			  } while (_findnext(hFile, &fileinfo) == 0);
		   //_findclose������������
	     _findclose(hFile);
   }
 }

unsigned __stdcall CSplashWnd::ThreadSaveData(void* arglist)
{
	CSplashWnd * pMainWnd = (CSplashWnd*)arglist;

	unsigned  char m_DataBuffer[30000] = { 0 };

	return 0;
}

void CSplashWnd::appendMsg(const char* msg)
{
	cout << msg;
	wstring text_string;
	CListTextElementUI* pItem = new CListTextElementUI();
	m_msg->Add(pItem);
	UtilTool::setWstring(text_string, msg);
	pItem->SetText(0, text_string.c_str());
}


void CSplashWnd::onError(const char* msg)
{
	appendMsg(msg);
}



