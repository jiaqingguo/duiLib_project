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
//网络管理界面调用FMU的数据包结构体
struct NetworkPackets
{
	double NumSatellites;							//卫星数量
	double NumEarthStations;						//地球站数量
	double NumGroundTerminals;						//地面终端数量
	vector<vector<double>> vecD_DeviceInformation;	//存放卫星、地球站、地面终端的数据
	double startPoint;								//起点序号
	double endPoint;								//终点序号
};

//用户
struct UserTable
{
	string Name_YH;//用户名称
	string Country_YH;//所属国家--用户
	string Affiliation_YH;//所属机构
	string Category_YH;//分类(电信港/用户)
	string UserNature_YH;//用户性质
};
//卫星
struct SatelliteTable
{
	string Name_WX;//卫星名称
	string Country_WX;//所属国家--卫星
	string Affiliation_WX;//所属机构
	string PublicInformation_WX;//是否信息公开
	string TrackType_WX;//轨道类别
};
//上行链路状态
struct UplinkState
{
	double CT_SX;//上行链路状态--C/T
	double CN_SX;//C/N--上行链路状态
	double CN0_SX;//C/N0
	string LinkMargin_SX;//链路余量
	string CommunicationQuality_SX;//通信质量
};
//下行链路状态
struct DownlinkState
{
	double CT_XX;//下行链路状态--C/T
	double CN_XX;//C/N--下行链路状态
	double CN0_XX;//C/N0
	string LinkMargin_XX;//链路余量
	string CommunicationQuality_XX;//通信质量
};

//有效链路表
struct EffectiveLinkTable
{
	int flag;
	UserTable userTable;
	SatelliteTable satelliteTable;
	UplinkState uplinkState;
	DownlinkState downlinkState;
};
//网络管理结构体
struct NetworkManagement
{
	string OperatorName;//运营商名称
	string Time;//时间 char Time[21];
	int SatelliteNumber;//卫星数量
	int EarthStationsNumber;//地球站数量
	int UserNumber;//用户数量
	string SatelliteName[12];//卫星名称
	string EarthStationsName[12];//地球站名称
	string UserName[12];//用户名称
};

struct NetworkManagement2
{
	vector<string> vec_OperatorName;//运营商名称
	vector<string> vec_SatelliteName;//卫星名称
	vector<string> vec_EarthStationsName;//地球站名称
	vector<string> vec_UserName;//用户名称
};

//struct NetworkManagement2
//{
//	vector<string> vec_OperatorName;//运营商名称
//	vector<string> vec_SatelliteName;//卫星名称
//	vector<string> vec_EarthStationsName;//地球站名称
//	vector<string> vec_UserName;//用户名称
//};

//网络管理结构体---如果是多个运营商
struct NetworkManagement1
{
	int OperatorNum;//运营商数量
	string OperatorName[10];//运营商名称
	string Time[10];//时间					--------------------- 10是根据运营商名称后面的10决定的 存放每个运营商创建时间
	int SatelliteNumber[10];//卫星数量		--------------------- 10是根据运营商名称后面的10决定的
	int EarthStationsNumber[10];//地球站数量--------------------- 
	int UserNumber[10];//用户数量			---------------------
	string SatelliteName[120];//卫星名称	---------------------从卫星数量中获取第一个运营商在这个结构体数组中的数量
	string EarthStationsName[120];//地球站名称	
	string UserName[120];//用户名称
};
struct ParameterManagement
{
	string NodeName;//节点名称
	int NodeContainsNum;
	string dataName[120]; //dataName中包含NodeContainsNum个数据
	string data[120];
};
struct ParameterManagement1
{
	int NodeNum;//节点数量
	string NodeName[20];//节点名称 -----------20代表最多可存放二级节点的数量
	int NodeContainsNum[20];//每个节点包含的信息数量
	string dataName[120];//NodeContainsNum中写了每个节点包含的数据个数 dataName的前NodeContainsNum[0]个代表的是节点1包含的数据信息
	string data[120];
};

//struct ParameterManagement
//{
//	string RootName;//根节点名称
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
	short Timing = 32000;//校时信息
	char  velocity[3] = { 0,0,-80 };//测速
	char  ranging[3] = { 48,69,-128 };//测距
	short rangingSelf = 4000;//测距自检
	short velocitySelf = 10000;//测速自检
	char  rangingSNR = 116;//测距SNR
	char  velocitySNR = 105;//测速SNR
	char  AGC = 76;//AGC
	char  Reset = 12;//看门狗复位次数
	char  ZW[2];
};
struct SerialPortData1
{
	char ZT[2];
	short Timing;//校时信息
	char  velocity[3];//测速
	char  ranging[3];//测距
	short rangingSelf;//测距自检
	short velocitySelf;//测速自检
	char  rangingSNR;//测距SNR
	char  velocitySNR;//测速SNR
	char  AGC;//AGC
	char  Reset;//看门狗复位次数
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