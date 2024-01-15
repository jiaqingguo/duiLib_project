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

HANDLE Thread_lock = CreateEvent(NULL, FALSE, FALSE, _T("线程1同步锁"));
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
	if (sName.CompareNoCase(_T("bt_LinkManagement")) == 0)//链路管理界面
	{
		m_CPopLinkManage = new CPopLinkManage();
		m_CPopLinkManage->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 1000, 800);
		m_CPopLinkManage->CenterWindow();
	}
	else if (sName.CompareNoCase(_T("bt_NetworkManagement")) == 0)//网络管理界面
	{
		m_CPopNetworkManage = new CPopNetworkManage();
		m_CPopNetworkManage->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 1000, 800);
		m_CPopNetworkManage->CenterWindow();

	}
	else if (sName.CompareNoCase(_T("bt_DataManagement")) == 0)//数据管理界面
	{
		m_CPopDataManage = new CPopDataManage();
		m_CPopDataManage->Create(m_hWnd, NULL, WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 1000, 800);
		m_CPopDataManage->CenterWindow();

	}
	else if (sName.CompareNoCase(_T("bt_EditorialInterface")) == 0)// 编辑界面
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
		// 按钮消息
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
	int i;    //遍历输入的字符串
	int a = 0;
	char temp;   //接收字符，用来判断是否为空格，若是则跳过
	char temp1, temp2;   //接收一个字节的两个字符  例如EB，则temp1='E',temp2 = 'B'
	*slen = 0;  //输出的16进制字符串长度
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
        //文件句柄  
		long   hFile = 0;
		//文件信息，声明一个存储文件信息的结构体  
		struct _finddata_t fileinfo;
		string p;//字符串，存放路径
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入
			{
				do
				  {
					 //如果是目录,迭代之（即文件夹内还有文件夹）  
					   if ((fileinfo.attrib &  _A_SUBDIR))
						 {
							 //文件名不等于"."&&文件名不等于".."
							 //.表示当前目录
							 //..表示当前目录的父目录
							 //判断时，两者都要忽略，不然就无限递归跳不出去了！
							     if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						         getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
					         }
					        //如果不是,加入列表  
				        else
					      {
							     //files.push_back(p.assign(path).append("\\").append(fileinfo.name));
						         files.push_back(fileinfo.name);
				   }
			  } while (_findnext(hFile, &fileinfo) == 0);
		   //_findclose函数结束查找
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



