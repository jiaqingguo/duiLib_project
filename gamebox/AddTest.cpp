#include "StdAfx.h"
#include "AddTest.h"

//////////////////////////////////////////////////////////////////////////
///
CDuiString ZTest_Name,ZDLX_Name;
bool option_zhT, option_dlx;
DUI_BEGIN_MESSAGE_MAP(CAddTest, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_END_MESSAGE_MAP()

	CAddTest::CAddTest(void)
{
	m_ZhTFlag = false;//ת̨��ѡ��ѡ�����
	m_DLXFlag = false;//����ѧ��ѡ��ѡ�����
}

CAddTest::~CAddTest(void)
{
}

void CAddTest::SetTitle(LPCTSTR lpstrTitle)
{
	if(lstrlen(lpstrTitle) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageTitle")));
	if( pControl ) pControl->SetText(lpstrTitle);
}

void CAddTest::SetMsg(LPCTSTR lpstrMsg)
{
	if(lstrlen(lpstrMsg) <= 0) return;

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("MessageText")));
	if( pControl ) pControl->SetText(lpstrMsg);
}



void CAddTest::OnFinalMessage( HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

DuiLib::CDuiString CAddTest::GetSkinFile()
{
	return _T("addtest.xml");
}

LPCTSTR CAddTest::GetWindowClassName( void ) const
{
	return _T("AddJudgeCondition");
}

void CAddTest::OnClick( TNotifyUI &msg )
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();

	if( msg.pSender == m_pCloseBtn ) {
		Close(MSGID_CANCEL);
		return; 
	}
	else if( msg.pSender == m_pMinBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
	else if( msg.pSender == m_pMaxBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
	else if( msg.pSender == m_pRestoreBtn ) { 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
	else if( msg.pSender == m_pMenuBtn ) {
	}
	else if(sName.CompareNoCase(_T("confirm_btn")) == 0)
	{
		if (m_ZhTFlag)
		{
			option_zhT = true;
			ZTest_Name = m_pm.FindControl(L"id")->GetText();
		}
		else
		{
			option_zhT = false;
		}
		if (m_DLXFlag)
		{
			option_dlx = true;
			ZDLX_Name = m_pm.FindControl(L"id1")->GetText();
		}
		else
		{
			option_dlx = false;
		}
		Close(MSGID_OK);
	}
	else if (sName.CompareNoCase(_T("optical_id")) == 0)//ת̨���ݴ洢��ѡ��
	{
		if (!m_ZhTFlag)//ѡ�и�ѡ��
		{
			m_ZhTFlag = true;
		}
		else
		{
			m_ZhTFlag = false;
		}
	}
	else if (sName.CompareNoCase(_T("optical_id1")) == 0)//����ѧ���ݴ洢��ѡ��
	{
		if (!m_DLXFlag)//ѡ�и�ѡ��
		{
			m_DLXFlag = true;
		}
		else
		{
			m_DLXFlag = false;
		}
	}
	else if(sName.CompareNoCase(_T("cancel_btn")) == 0)
	{
		Close(MSGID_CANCEL);
	}
}

LRESULT CAddTest::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void CAddTest::Notify( TNotifyUI &msg )
{
	if (msg.sType==L"textchanged" && msg.pSender->GetName() == L"id")
	{
		Test_Name=m_pm.FindControl(L"id")->GetText();
	}
	else if (msg.sType == L"textchanged" && msg.pSender->GetName() == L"id1")
	{
		DLX_Name = m_pm.FindControl(L"id1")->GetText();
	}
	return WindowImplBase::Notify(msg);
}

LRESULT CAddTest::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0L;
}

void CAddTest::InitWindow()
{
	pLabel_ID=static_cast<CLabelUI *>(m_pm.FindControl(L"id"));
	pLabel_ID1 = static_cast<CLabelUI *>(m_pm.FindControl(L"id1"));
}
