#pragma  once
#include <winsock.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#define BUFFERSIZE 30000
#include <string>

using namespace std;

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

class UDPAdmin2 {
public:
	UDPAdmin2() {
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
		int ret = recvfrom(hSOCKET, buf, BUFFERSIZE, 0, (sockaddr *)&addrSrv, &nAddlen);//������Ϣ

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
		remoteAddrClient.sin_addr.S_un.S_addr = inet_addr(ip.c_str());//IP��ַ//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ���� һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��


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

class UDPAdmin3 {
public:
	UDPAdmin3() {
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
		int ret = recvfrom(hSOCKET, buf, BUFFERSIZE, 0, (sockaddr *)&addrSrv, &nAddlen);//������Ϣ

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
		remoteAddrClient.sin_addr.S_un.S_addr = inet_addr(ip.c_str());//IP��ַ//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ���� һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��


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

class UDP_Recv {
public:
	UDP_Recv() {
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
		remoteAddr.sin_port = htons(8888); //1024���ϵĶ˿ں�
		remoteAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//IP��ַ//INADDR_ANY����ָ����ַΪ0.0.0.0�ĵ�ַ�������ַ��ʵ�ϱ�ʾ��ȷ����ַ�������е�ַ�����������ַ���� һ����˵���ڸ���ϵͳ�о������Ϊ0ֵ��


		return true;
	}

	int	UDPSend(char * buf, int size)
	{
		int nAddlen = sizeof(remoteAddr);
		sendto(serSocket, buf, size, 0, (sockaddr *)&remoteAddr, nAddlen);//����
		return 0;
	}

	int	UDPRecv(char * buf)
	{
		int nAddlen = sizeof(remoteAddr);
		int ret = recvfrom(serSocket, buf, BUFFERSIZE, 0, (sockaddr *)&remoteAddr, &nAddlen);//������Ϣ

		return ret;
	}


	int Close()
	{
		int n = closesocket(hSOCKET);
		return n;
	}


	int len;
	SOCKET hSOCKET;
	SOCKADDR_IN addrSrv;
	SOCKET serSocket;
	sockaddr_in remoteAddr;
};

class UDPRecvtle {
public:
	UDPRecvtle() {
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

	int	UDPsRecv(char * buf, int size)
	{
		int nAddlen = sizeof(addrSrv);
		int ret = recvfrom(hSOCKET, buf, size, 0, (sockaddr *)&addrSrv, &nAddlen);//������Ϣ

		return ret;
	}


	int Close()
	{
		int n = closesocket(hSOCKET);
		return n;
	}

	int len;
	SOCKET hSOCKET;
	SOCKADDR_IN addrSrv;
};