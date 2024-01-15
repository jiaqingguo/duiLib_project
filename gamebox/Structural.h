#pragma once
#include <string>
#include <vector>
using namespace std;
#pragma  once
#pragma  pack (1)
struct RECPOSSTAT
{
	string rec_stat_X;
	string rec_stat_Y;
	string rec_stat_Z;
	string rec_stat_GD;
	string rec_stat_FY;
	string rec_stat_PH;
	string rec_stat_dof7;
	string rec_stat_dof8;
	string rec_stat_dof9;
};
//�������������FMU�����ݰ��ṹ��
struct NetworkPackets
{
	double NumSatellites;							//��������
	double NumEarthStations;						//����վ����
	double NumGroundTerminals;						//�����ն�����
	vector<vector<double>> vecD_DeviceInformation;	//������ǡ�����վ�������ն˵�����
	double startPoint;								//������
	double endPoint;								//�յ����
};

//�û�
struct UserTable
{
	string Name_YH;//�û�����
	string Country_YH;//��������--�û�
	string Affiliation_YH;//��������
	string Category_YH;//����(���Ÿ�/�û�)
	string UserNature_YH;//�û�����
};
//����
struct SatelliteTable
{
	string Name_WX;//��������
	string Country_WX;//��������--����
	string Affiliation_WX;//��������
	string PublicInformation_WX;//�Ƿ���Ϣ����
	string TrackType_WX;//������
};
//������·״̬
struct UplinkState
{
	double CT_SX;//������·״̬--C/T
	double CN_SX;//C/N--������·״̬
	double CN0_SX;//C/N0
	string LinkMargin_SX;//��·����
	string CommunicationQuality_SX;//ͨ������
};
//������·״̬
struct DownlinkState
{
	double CT_XX;//������·״̬--C/T
	double CN_XX;//C/N--������·״̬
	double CN0_XX;//C/N0
	string LinkMargin_XX;//��·����
	string CommunicationQuality_XX;//ͨ������
};

//��Ч��·��
struct EffectiveLinkTable
{
	int flag;
	UserTable userTable;
	SatelliteTable satelliteTable;
	UplinkState uplinkState;
	DownlinkState downlinkState;
};
//�������ṹ��
struct NetworkManagement
{
	string OperatorName;//��Ӫ������
	string Time;//ʱ�� char Time[21];
	int SatelliteNumber;//��������
	int EarthStationsNumber;//����վ����
	int UserNumber;//�û�����
	string SatelliteName[12];//��������
	string EarthStationsName[12];//����վ����
	string UserName[12];//�û�����
};

struct NetworkManagement2
{
	vector<string> vec_OperatorName;//��Ӫ������
	vector<string> vec_SatelliteName;//��������
	vector<string> vec_EarthStationsName;//����վ����
	vector<string> vec_UserName;//�û�����
};

//struct NetworkManagement2
//{
//	vector<string> vec_OperatorName;//��Ӫ������
//	vector<string> vec_SatelliteName;//��������
//	vector<string> vec_EarthStationsName;//����վ����
//	vector<string> vec_UserName;//�û�����
//};

//�������ṹ��---����Ƕ����Ӫ��
struct NetworkManagement1
{
	int OperatorNum;//��Ӫ������
	string OperatorName[10];//��Ӫ������
	string Time[10];//ʱ��					--------------------- 10�Ǹ�����Ӫ�����ƺ����10������ ���ÿ����Ӫ�̴���ʱ��
	int SatelliteNumber[10];//��������		--------------------- 10�Ǹ�����Ӫ�����ƺ����10������
	int EarthStationsNumber[10];//����վ����--------------------- 
	int UserNumber[10];//�û�����			---------------------
	string SatelliteName[120];//��������	---------------------�����������л�ȡ��һ����Ӫ��������ṹ�������е�����
	string EarthStationsName[120];//����վ����	
	string UserName[120];//�û�����
};
struct ParameterManagement
{
	string NodeName;//�ڵ�����
	int NodeContainsNum;
	string dataName[120]; //dataName�а���NodeContainsNum������
	string data[120];
};
struct ParameterManagement1
{
	int NodeNum;//�ڵ�����
	string NodeName[20];//�ڵ����� -----------20�������ɴ�Ŷ����ڵ������
	int NodeContainsNum[20];//ÿ���ڵ��������Ϣ����
	string dataName[120];//NodeContainsNum��д��ÿ���ڵ���������ݸ��� dataName��ǰNodeContainsNum[0]��������ǽڵ�1������������Ϣ
	string data[120];
};

//struct ParameterManagement
//{
//	string RootName;//���ڵ�����
//
//};
struct DYWDATA
{
	double SX;
	double SY;
	double SZ;
	double SQ1;
	double SQ2;
	double SQ3;
	double SQ4;

};
struct SerialPortData
{
	char ZT[2];
	short Timing = 32000;//Уʱ��Ϣ
	char  velocity[3] = { 0,0,-80 };//����
	char  ranging[3] = { 48,69,-128 };//���
	short rangingSelf = 4000;//����Լ�
	short velocitySelf = 10000;//�����Լ�
	char  rangingSNR = 116;//���SNR
	char  velocitySNR = 105;//����SNR
	char  AGC = 76;//AGC
	char  Reset = 12;//���Ź���λ����
	char  ZW[2];
};
struct SerialPortData1
{
	char ZT[2];
	short Timing;//Уʱ��Ϣ
	char  velocity[3];//����
	char  ranging[3];//���
	short rangingSelf;//����Լ�
	short velocitySelf;//�����Լ�
	char  rangingSNR;//���SNR
	char  velocitySNR;//����SNR
	char  AGC;//AGC
	char  Reset;//���Ź���λ����
	char  ZW[2];
};
struct UDPData
{
	double velocity;
	double ranging;
};

struct ParameterTLE
{
	vector<string>Parametername;
	vector<map<string, string>>Parameterdata;
};

struct SceneTLE
{
	string Scenename;
	map<string, ParameterTLE>SceneTLEdata;
};
#pragma  pack ();