#pragma once
#define    WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <windows.h>
#include <iostream>

#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")
//#pragma comment(lib, "WSOCK32.lib")
#include <tchar.h>
#include <ws2tcpip.h>

#define BUFFERSIZE 10000
using namespace std;
class UdpRecvClass
{


public:
	UdpRecvClass() {
		Startup();
		len = sizeof(SOCKADDR);
	}

	inline  void Startup(void) {
#ifdef _WIN32
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == -1) {
			return;
		}
		if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
			WSACleanup();
			return;
		}
#endif
	}

	//~UdpRecvClass();
public:

	int SetNoBlocking(int bblocking) {
		DWORD ul = bblocking;
		return ioctlsocket(hSOCKET, FIONBIO, &ul);
	}

	bool InitServer(int port)
	{
		// �����׽���
		hSOCKET = socket(AF_INET, SOCK_DGRAM, 0);
		if (hSOCKET == INVALID_SOCKET)
		{
			//			OutputDebugString(L"socket() failed, Error Code:%d\n");
			//AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
			//exit(-1) ;
			return false;
		}

		//SOCKADDR_IN addrSrv;
		BOOL bReuseaddr;
		addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		setsockopt(hSOCKET, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));
		// ���׽���
		int nRet = bind(hSOCKET, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
		if (SOCKET_ERROR == nRet)
		{
			//			OutputDebugString(L"bind failed !\n");   
			closesocket(hSOCKET);
			//exit(-1) ;
			return false;
		}
		return true;
	}
	int	UDPsSend(char* buf, int size)
	{
		int nAddlen = sizeof(addrSrv);
		sendto(hSOCKET, buf, size, 0, (sockaddr*)&addrSrv, nAddlen);//����
		return 0;
	}

	int	UDPsRecv(char* buf)
	{
		int nAddlen = sizeof(addrSrv);
		int ret = recvfrom(hSOCKET, buf, BUFFERSIZE, 0, (sockaddr*)&addrSrv, &nAddlen);//������Ϣ

		return ret;
	}

	int InitClient()
	{
		WSADATA wsaData;
		// WORD sokeVersion = MAKEWORD(2,2);
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)

		{
			return 0;
		}

		serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//����UDP����˵��׽���
		if (serSocket == INVALID_SOCKET)
		{
			return 0;
		}


		remoteAddr.sin_family = AF_INET;
		remoteAddr.sin_port = htons(8001); //1024���ϵĶ˿ں�
		remoteAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP��ַ//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ���� һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��

		if (bind(serSocket, (sockaddr*)&addrSrv, sizeof(addrSrv)) == SOCKET_ERROR)//�����IP�͵�ַ
		{
			closesocket(serSocket);
			return 0;
		}

		return true;
	}

	int	UDPSend(char* buf, int size)
	{
		int nAddlen = sizeof(remoteAddr);
		sendto(serSocket, buf, size, 0, (sockaddr*)&remoteAddr, nAddlen);//����
		return 0;
	}

	int	UDPRecv(char* buf)
	{
		int nAddlen = sizeof(remoteAddr);
		int ret = recvfrom(serSocket, buf, BUFFERSIZE, 0, (sockaddr*)&remoteAddr, &nAddlen);//������Ϣ

		return ret;
	}

	int Close()
	{
		int n = closesocket(hSOCKET);
		return n;
	}

public:

	int len;
	SOCKET hSOCKET;
	SOCKADDR_IN addrSrv;
	SOCKET serSocket;
	sockaddr_in remoteAddr;
};

//class UDPAdmin1 {
//public:
//	UDPAdmin1() {
//		Startup();
//		len = sizeof(SOCKADDR);
//	}
//
//	inline  void Startup(void) {
//#ifdef _WIN32
//		WSADATA wsa_data;
//		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == -1) {
//			return;
//		}
//		if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
//			WSACleanup();
//			return;
//		}
//#endif
//	}
//
//	int SetNoBlocking(int bblocking) {
//		DWORD ul = bblocking;
//		return ioctlsocket(hSOCKET, FIONBIO, &ul);
//	}
//
//	bool InitServer(int port)
//	{
//
//		// �����׽���
//		hSOCKET = socket(AF_INET, SOCK_DGRAM, 0);
//		if (hSOCKET == INVALID_SOCKET)
//		{
//			//			OutputDebugString(L"socket() failed, Error Code:%d\n");
//			//AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
//			//exit(-1) ;
//			return false;
//		}
//
//
//		BOOL bReuseaddr;
//		addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
//		addrSrv.sin_family = AF_INET;
//		addrSrv.sin_port = htons(port);
//		setsockopt(hSOCKET, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));
//		// ���׽���
//		int nRet = ::bind(hSOCKET, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
//		if (SOCKET_ERROR == nRet)
//		{
//			//			OutputDebugString(L"bind failed !\n");   
//			closesocket(hSOCKET);
//			//exit(-1) ;
//			return false;
//		}
//		return true;
//
//
//	}
//
//	int	UDPsSend(char * buf, int size)
//	{
//		int nAddlen = sizeof(addrSrv);
//		sendto(hSOCKET, buf, size, 0, (sockaddr *)&addrSrv, nAddlen);//����
//		return 0;
//	}
//
//	int	UDPsRecv(char * buf)
//	{
//		int nAddlen = sizeof(addrSrv);
//		int ret = recvfrom(hSOCKET, buf, 10000, 0, (sockaddr *)&addrSrv, &nAddlen);//������Ϣ
//
//		return ret;
//	}
//
//
//	int Close()
//	{
//		int n = closesocket(hSOCKET);
//		return n;
//	}
//
//	int InitClient(int port, string ip)
//	{
//		WSADATA wsaData;
//		// WORD sokeVersion = MAKEWORD(2,2);
//		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//
//		{
//			return 0;
//		}
//
//		serSockeClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//����UDP����˵��׽���
//		if (serSockeClient == INVALID_SOCKET)
//		{
//			return 0;
//		}
//
//
//		remoteAddrClient.sin_family = AF_INET;
//		remoteAddrClient.sin_port = htons(port); //1024���ϵĶ˿ں�
//		remoteAddrClient.sin_addr.S_un.S_addr = inet_addr(ip.c_str());//IP��ַ//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ���� һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��
//
//
//		return true;
//	}
//
//	int	UDPSendClient(char * buf, int size)
//	{
//		int nAddlen = sizeof(remoteAddrClient);
//		sendto(serSockeClient, buf, size, 0, (sockaddr *)&remoteAddrClient, nAddlen);//����
//		return 0;
//	}
//
//	int	UDPRecvClient(char * buf)
//	{
//		int nAddlen = sizeof(remoteAddrClient);
//		int ret = recvfrom(serSockeClient, buf, BUFFERSIZE, 0, (sockaddr *)&remoteAddrClient, &nAddlen);//������Ϣ
//
//		return ret;
//	}
//
//	int len;
//	SOCKET hSOCKET;
//	SOCKADDR_IN addrSrv;
//	SOCKET serSocket;
//	sockaddr_in remoteAddr;
//	SOCKET serSockeClient;
//	sockaddr_in remoteAddrClient;
//};

class UDPAdmin1 {
public:
	UDPAdmin1() {
		Startup();
		len = sizeof(SOCKADDR);
	}

	inline  void Startup(void) {
#ifdef _WIN32
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == -1) {
			return;
		}
		if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 2) {
			WSACleanup();
			return;
		}
#endif
	}

	int SetNoBlocking(int bblocking) {
		DWORD ul = bblocking;
		return ioctlsocket(hSOCKET, FIONBIO, &ul);
	}

	bool InitServer(int port)
	{

		// �����׽���
		hSOCKET = socket(AF_INET, SOCK_DGRAM, 0);
		if (hSOCKET == INVALID_SOCKET)
		{
			//			OutputDebugString(L"socket() failed, Error Code:%d\n");
			//AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
			//exit(-1) ;
			return false;
		}


		BOOL bReuseaddr;
		addrSrv.sin_addr.s_addr = htonl(INADDR_ANY);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		setsockopt(hSOCKET, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReuseaddr, sizeof(BOOL));
		// ���׽���
		int nRet = ::bind(hSOCKET, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
		if (SOCKET_ERROR == nRet)
		{
			//			OutputDebugString(L"bind failed !\n");   
			closesocket(hSOCKET);
			//exit(-1) ;
			return false;
		}
		return true;


	}

	int	UDPsSend(char * buf, int size)
	{
		int nAddlen = sizeof(addrSrv);
		sendto(hSOCKET, buf, size, 0, (sockaddr *)&addrSrv, nAddlen);//����
		return 0;
	}

	int	UDPsRecv(char * buf)
	{
		int nAddlen = sizeof(addrSrv);
		int ret = recvfrom(hSOCKET, buf, 10000, 0, (sockaddr *)&addrSrv, &nAddlen);//������Ϣ

		return ret;
	}


	int Close()
	{
		int n = closesocket(hSOCKET);
		return n;
	}

	int InitClient(int port, string ip)
	{
		WSADATA wsaData;
		// WORD sokeVersion = MAKEWORD(2,2);
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)

		{
			return 0;
		}

		serSockeClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//����UDP����˵��׽���
		if (serSockeClient == INVALID_SOCKET)
		{
			return 0;
		}


		remoteAddrClient.sin_family = AF_INET;
		remoteAddrClient.sin_port = htons(port); //1024���ϵĶ˿ں�
		//remoteAddrClient.sin_addr.S_un.S_addr = inet_addr(ip.c_str());//IP��ַ//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ���� һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��
		remoteAddrClient.sin_addr.s_addr = inet_addr(ip.c_str());

		return true;
	}

	int	UDPSendClient(char * buf, int size)
	{
		int nAddlen = sizeof(remoteAddrClient);
		sendto(serSockeClient, buf, size, 0, (sockaddr *)&remoteAddrClient, nAddlen);//����
		return 0;
	}

	int	UDPRecvClient(char * buf)
	{
		int nAddlen = sizeof(remoteAddrClient);
		int ret = recvfrom(serSockeClient, buf, BUFFERSIZE, 0, (sockaddr *)&remoteAddrClient, &nAddlen);//������Ϣ

		return ret;
	}

	int len;
	SOCKET hSOCKET;
	SOCKADDR_IN addrSrv;
	SOCKET serSocket;
	sockaddr_in remoteAddr;
	SOCKET serSockeClient;
	sockaddr_in remoteAddrClient;
};

