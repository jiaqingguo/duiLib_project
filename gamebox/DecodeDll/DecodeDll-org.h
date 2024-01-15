#pragma once

#ifdef DECODELIBDLL
#define DECODEAPI _declspec(dllexport)
#else
#define DECODEAPI  _declspec(dllimport)
#endif

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <Winsock2.h>
#include "CRedis.h"
#include <string>
#include <stdint.h>
#include "Header.h"
#include <map>
#include "tinyxml.h"
using namespace std;

#define  u_int8_t unsigned char
#define  u_int64_t unsigned __int64
#define  u_int16_t unsigned short
#define  u_int32_t unsigned int
#define   uint64_t unsigned __int64
#define  int64_t __int64

#define xmlElementPtr  TiXmlElement *  
//解码表列数定义
#define DECODEXLS_PARAM_ID_COL			2	//参数代号所在列(列数从0开始计)
#define DECODEXLS_PARAM_NAME_COL		3	//参数名称所在列(列数从0开始计)
#define DECODEXLS_START_BYTE_COL		4	//起始字节所在列(列数从0开始计)
#define DECODEXLS_BYTE_COUNT_COL		5	//字节个数所在列(列数从0开始计)
#define DECODEXLS_BYTE_ORDER_COL		6	//字节顺序所在列(列数从0开始计)
#define DECODEXLS_DATA_TYPE_COL			7	//数据类型所在列(列数从0开始计)
#define DECODEXLS_START_BIT_COL			9	//起始位所在列(列数从0开始计)
#define DECODEXLS_END_BIT_COL			8	//终止位所在列(列数从0开始计)
#define DECODEXLS_SIGN_BYTE_COL			10	//符号位字节所在列(列数从0开始计)
#define DECODEXLS_SIGN_BIT_COL			11	//符号占位所在列(列数从0开始计)
#define DECODEXLS_EQUIVALENT_COL		12	//当量所在列(列数从0开始计)
#define DECODEXLS_DISPLAY_TYPE_COL		13	//显示类型所在列(列数从0开始计)
#define DECODEXLS_PARAM_UNIT_COL		14	//单位所在列(列数从0开始计)
#define DECODEXLS_PARAM_IS_ENABLE		15	//单位所在列(列数从0开始计)
#define DECODEXLS_DEFAULTVALUE_COL		16	//默认值所在列(列数从0开始计)
#define DECODEXLS_UPPERLIMIT_COL		17	//默认值所在列(列数从0开始计)
#define DECODEXLS_LOWERLIMIT_COL		18	//默认值所在列(列数从0开始计)
#define DECODEXLS_RADIX_COL		        19	//默认值所在列(列数从0开始计)
#define DECODEXLS_FORMULA_COL		        20	//默认值所在列(列数从0开始计)
#define DECODEXLS_A_COL		        21	//默认值所在列(列数从0开始计)
#define DECODEXLS_B_COL		        22	//默认值所在列(列数从0开始计)
#define DECODEXLS_C_COL		        23	//默认值所在列(列数从0开始计)
//#define DECODEXLS_ANNOTATION_COUNT_COL	18	//注释循环次数所在列(列数从0开始计)
#define RAWBYTE_SIZE                   240  //源码大小
#define MSG_ROWCOUNT_ID                 "rowcount"
#define MSG_COLCOUNT_ID                 "colcount"
//数据类型
#define RAW_DATA_TYPE_INTEGER  0
#define RAW_DATA_TYPE_FLOAT    1
#define RAW_DATA_TYPE_DOUBLE   2
#define RAW_DATA_TYPE_STRING   3

//显示类型
#define SHOWTYPE_DOUBLE_ID	1
#define SHOWTYPE_FLOAT_ID	2
#define SHOWTYPE_INT64_ID	3
#define SHOWTYPE_INT32_ID	4
#define SHOWTYPE_INT16_ID	5
#define SHOWTYPE_INT8_ID    6
#define SHOWTYPE_UINT64_ID	7
#define SHOWTYPE_UINT32_ID	8
#define SHOWTYPE_UINT16_ID	9
#define SHOWTYPE_UINT8_ID	10
#define SHOWTYPE_ARRAY_ID	11
#define SHOWTYPE_FLOAT16_ID	12

#define USERINPUT_PATH  "../../loadfile/userinput/"
//XML字段
#define MSG_ROOT_NAME            "servercfg"
#define MSG_MESSAGES_NODE_NAME   "messages"
#define MSG_MESSAGE_NODE_NAME    "message"
#define MSG_NAME_ATTR_NAME       "name"
#define MSG_CONF_FILE_ATTR_NAME  "decode_file"
#define MSG_CONF_FILE_ATTR_SHOWNAME  "showname"
#define MSG_ID_ATTR_NAME         "name"
#define MSG_SIZE_ATTR_NAME       "raw_size"
#define MSG_DISTINCT_ID       "distinct_id"

#define MSG_CHOOSE       "msg_choose"
#define MSG_TYPE       "msg_type"

#define MSG_CHANNEL_ID         "channel"
#define MSG_BOARD_ID           "board"
#define MSG_RT_ID              "rt"
#define MSG_SUBRT_ID           "sa"
#define MSG_WORDCOUNT_ID       "wordcount"
#define MSG_FORMAT       "msg_format"

struct TableConfigInfo{
	int iRows;
	int iCols;
	int iStart;
};

//BIT7~3	BIT2	BIT1	BIT 0	消 息 格 式
//	0	  0	  0	 0	BC-to-RT（如果 T/ =0 ）或者 RT-to-BC (如果 T/ =1)
//
//	0	  0	  0	 1	RT-to-RT
//	0	  0	  1	 0	Broadcast
//	0	  0	  1	 1	RT-to-RTs （Broadcast）
//	0	  1	  0	 0	Mode Code
//	0	  1	  0	 1	保留
//	0	  1	  1	 0	Broadcast Mode Code
//	0	  1	  1	 1	保留

//字节序
#define RAW_BYTE_ORDER_LOW_FIRST      1
#define RAW_BYTE_ORDER_HIGH_FIRST     0
#define RAW_BYTE_ORDER_LOWBIT_FIRST   2
#define RAW_BYTE_ORDER_LOWPARM_FIRST   3

//#if __BYTE_ORDER == __LITTLE_ENDIAN
//	#define ntohll(x) wap_64(x)
//	#define htonll(x) __bswap_64(x)
//#elif __BYTE_ORDER == __BIG_ENDIAN
//	#define ntohll(x) (x)
//	#define htonll(x) (x)
//#endif





//解码表中的每个参数的定义
typedef struct _ParameterDescriptor
{
	unsigned int start_byte; //起始字节
	unsigned int byte_count; //字节个数
	unsigned int byte_order;
	unsigned int data_type; //数据类型 0：原码 1:补码 2：浮点数
	unsigned int end_bit;
	unsigned int start_bit;
	unsigned int sign_bit; //符号位
	unsigned int display_type; //显示类型共11种:double=1,float=2,int64=3,int32=4,int16=5,int8=6,uint64=7,uint32=8,uint16=9,uint8=10,array=11
	unsigned int annotation_times; //注释循环次数
	unsigned int bit_length; 	//参数的比特位总长度
	unsigned int sign_byte; //符号位的字节数
	unsigned int radix_type; //符号位的字节数
	unsigned int annotation_index; //符号位的字节数
	unsigned int is_enable;
	int parm_index;      //符号序号
	double equivalent; //当量
	char paramID[200];	//参数代号
	//	parm_element * markelement;
	char paramName[200];//参数名称
	char paramUnit[200];//参数单位
	char formulaName[200];//参数名称
	double defaultval;
	double upperlimit;
	double lowerlimit;
	double avalue;
	double bvalue;
	double cvalue;
	unsigned char defaultary[RAWBYTE_SIZE];
	char annotation_str[130][300]; //汉字注释中的注释部分（即半角冒号:后的汉字、英文等组成的字符串），最长300字节（UTF-8，100个汉字）
	int annotation_val[130];
	int annotation_cnt;
	char annotation_raw[130][8];//汉字注释中的原码部分(即半角冒号:前的16进制原码字符串)，最长8个字节
	char zs[300];//传递给waveform类型PV的注释部分字符串（最长300个字节，100个汉字）
} ParameterDescriptor;

//网络接收到的数据按解码表处理之后的每个参数的属性
typedef struct _ParameterValue
{
	union
	{
		int64_t rawvalue;
		float floatvalue;
		double doublevalue;
	} v;//参数的解码值
	char rawbytes[RAWBYTE_SIZE];//参数的原码值
} ParameterValue;
struct _Message;



typedef struct
{
	unsigned int param_cnt;
	unsigned int msg_size;
	unsigned int  rt;
	unsigned int  subrt;
	unsigned int  msgformat;
	unsigned int  value_size;
	char * msg_choose;
	char * msg_type;
	char * cshowname;
	char * name;

	ParameterDescriptor *parameters;
} MessageDescriptor;

typedef struct _Message
{
	MessageDescriptor *msg_descriptor;//解码表中的参数属性
	ParameterValue *param_value;//处理后的参数属性
	unsigned char msg_raw[2000];//消息的整包原码，最大支持6000字节

	_Message(){
		msg_descriptor=NULL;
		param_value=NULL;
	}

	~_Message(){
		free(param_value);
		param_value=NULL;
	}
} Message;

typedef struct _PluginMsgNode
{
	unsigned int msgTypeId;
	unsigned int msgLength;
	char * msgName;
	MessageDescriptor *msgDescriptor;
	struct _PluginMsgNode *next;
} PluginMsgNode;

typedef struct _MsgNodeList
{
	void *head;//指向PluginMsgNode的指针
	unsigned int count;//配置文件里定义的消息个数
} MsgNodeList;

typedef struct _MsgLibrary
{
	MsgNodeList nodes;
	unsigned int count;//配置文件里定义的消息个数
} MsgLibrary;

//参数按字节处理用到的结构
typedef union
{
	u_int8_t m_uInt8;
	u_int8_t m_Bytes[1];
} BIT8_UNION;

typedef union
{
	u_int16_t m_Word;
	u_int8_t m_Bytes[2];
} BIT16_UNION;

typedef union
{
	u_int32_t m_Long;
	u_int8_t m_Bytes[4];
} BIT32_UNION;

typedef union
{
	u_int64_t m_uInt64;
	u_int8_t m_Bytes[8];
} BIT64_UNION;

typedef union
{
	unsigned short m_Word;
	u_int8_t m_Bytes[2];
	struct
	{
		u_int8_t IO_0 :1; //
		u_int8_t IO_1 :1; //
		u_int8_t IO_2 :1; //
		u_int8_t IO_3 :1; //
		u_int8_t IO_4 :1; //
		u_int8_t IO_5 :1; //
		u_int8_t IO_6 :1; //
		u_int8_t IO_7 :1; //
		u_int8_t IO_8 :1; //
		u_int8_t IO_9 :1; //
		u_int8_t IO_10 :1; //
		u_int8_t IO_11 :1; //
		u_int8_t IO_12 :1; //
		u_int8_t IO_13 :1; //
		u_int8_t IO_14 :1; //
		u_int8_t IO_15 :1; //
	} IO_bit;
} BIT16_UNION_DETAIL;

typedef union
{
	u_int32_t m_Long;
	float m_float;
	u_int8_t m_Bytes[4];
	struct
	{
		u_int8_t IO_0 :1; //
		u_int8_t IO_1 :1; //
		u_int8_t IO_2 :1; //
		u_int8_t IO_3 :1; //
		u_int8_t IO_4 :1; //
		u_int8_t IO_5 :1; //
		u_int8_t IO_6 :1; //
		u_int8_t IO_7 :1; //
		u_int8_t IO_8 :1; //
		u_int8_t IO_9 :1; //
		u_int8_t IO_10 :1; //
		u_int8_t IO_11 :1; //
		u_int8_t IO_12 :1; //
		u_int8_t IO_13 :1; //
		u_int8_t IO_14 :1; //
		u_int8_t IO_15 :1; //
		u_int8_t IO_16 :1; //
		u_int8_t IO_17 :1; //
		u_int8_t IO_18 :1; //
		u_int8_t IO_19 :1; //
		u_int8_t IO_20 :1; //
		u_int8_t IO_21 :1; //
		u_int8_t IO_22 :1; //
		u_int8_t IO_23 :1; //
		u_int8_t IO_24 :1; //
		u_int8_t IO_25 :1; //
		u_int8_t IO_26 :1; //
		u_int8_t IO_27 :1; //
		u_int8_t IO_28 :1; //
		u_int8_t IO_29 :1; //
		u_int8_t IO_30 :1; //
		u_int8_t IO_31 :1; //
	} IO_bit;
} BIT32_UNION_DETAIL;

typedef union
{
	u_int64_t m_uInt64;
	int64_t m_Int64;
	double m_double;
	u_int8_t m_Bytes[8];
	struct
	{
		u_int8_t IO_0 :1; //
		u_int8_t IO_1 :1; //
		u_int8_t IO_2 :1; //
		u_int8_t IO_3 :1; //
		u_int8_t IO_4 :1; //
		u_int8_t IO_5 :1; //
		u_int8_t IO_6 :1; //
		u_int8_t IO_7 :1; //
		u_int8_t IO_8 :1; //
		u_int8_t IO_9 :1; //
		u_int8_t IO_10 :1; //
		u_int8_t IO_11 :1; //
		u_int8_t IO_12 :1; //
		u_int8_t IO_13 :1; //
		u_int8_t IO_14 :1; //
		u_int8_t IO_15 :1; //
		u_int8_t IO_16 :1; //
		u_int8_t IO_17 :1; //
		u_int8_t IO_18 :1; //
		u_int8_t IO_19 :1; //
		u_int8_t IO_20 :1; //
		u_int8_t IO_21 :1; //
		u_int8_t IO_22 :1; //
		u_int8_t IO_23 :1; //
		u_int8_t IO_24 :1; //
		u_int8_t IO_25 :1; //
		u_int8_t IO_26 :1; //
		u_int8_t IO_27 :1; //
		u_int8_t IO_28 :1; //
		u_int8_t IO_29 :1; //
		u_int8_t IO_30 :1; //
		u_int8_t IO_31 :1; //
		u_int8_t IO_32 :1; //
		u_int8_t IO_33 :1; //
		u_int8_t IO_34 :1; //
		u_int8_t IO_35 :1; //
		u_int8_t IO_36 :1; //
		u_int8_t IO_37 :1; //
		u_int8_t IO_38 :1; //
		u_int8_t IO_39 :1; //
		u_int8_t IO_40 :1; //
		u_int8_t IO_41 :1; //
		u_int8_t IO_42 :1; //
		u_int8_t IO_43 :1; //
		u_int8_t IO_44 :1; //
		u_int8_t IO_45 :1; //
		u_int8_t IO_46 :1; //
		u_int8_t IO_47 :1; //
		u_int8_t IO_48 :1; //
		u_int8_t IO_49 :1; //
		u_int8_t IO_50 :1; //
		u_int8_t IO_51 :1; //
		u_int8_t IO_52 :1; //
		u_int8_t IO_53 :1; //
		u_int8_t IO_54 :1; //
		u_int8_t IO_55 :1; //
		u_int8_t IO_56 :1; //
		u_int8_t IO_57 :1; //
		u_int8_t IO_58 :1; //
		u_int8_t IO_59 :1; //
		u_int8_t IO_60 :1; //
		u_int8_t IO_61 :1; //
		u_int8_t IO_62 :1; //
		u_int8_t IO_63 :1; //
	} IO_bit;
} BIT64_UNION_DETAIL;

typedef union
{
	u_int8_t m_Bytes;
	struct
	{
		u_int8_t IO_0 :1; //
		u_int8_t IO_1 :1; //
		u_int8_t IO_2 :1; //
		u_int8_t IO_3 :1; //
		u_int8_t IO_4 :1; //
		u_int8_t IO_5 :1; //
		u_int8_t IO_6 :1; //
		u_int8_t IO_7 :1; //
	} IO_bit;
} BIT8_UNION_DETAIL;

class DECODEAPI  Decode
{
public:

	int parse_decodeXLS(MessageDescriptor* msg, const char * path);
	int getRedisElementInt(CRedis & redis,const char * sheetName,const char * suffix);
	char * getRedisElementStr(CRedis & redis,const char * sheetName,int row,int col);
	void reverse_BitOrder(BIT64_UNION *val, u_int8_t bitstart, u_int8_t bistop);
	void rawdecodelib_init(void);
	int msglib_config(const char *cfg_file,const char * filefold,const char * redisIP,int redisPort);
	static Decode * GetInstance(const char *cfg_file,const char * filefold,char * ip = "127.0.0.1", int portnum = 6379);
	int msglib_register(unsigned int msgTypeId,char * msgName, unsigned int msgLength);
	Message* create_msg_by_ID(unsigned int msgTypeId);
	Message* create_msg_by_Name(const char * msgName,double * val=NULL);
	Message* create_msg_by_ShowName(const char * msgShowName,double * val=NULL);
	PluginMsgNode* get_PluginNode_by_ID(unsigned int msgTypeId);
	int decode_msg(Message* handle, u_int8_t *msg);
	int decode_msgbylen(Message* handle,u_int8_t *msg,int msgcnt);
	int recode_msg(Message* handle, u_int8_t *msg);
	int msg_recode_handle(Message* handle, u_int8_t *msg);
	int msg_decode_handle(Message* handle, u_int8_t *msg);
	void decode_parse_messages_config(xmlElementPtr cur_element,const char * filefold,int * index);
	void decode_parse_message_config(xmlElementPtr cur_element,const char * filefold,int * index);//xml解析
	void BitReset(ParameterDescriptor *param ,ParameterValue *paramValue,unsigned char * msg);//字节复位
	int reverse_ele(Message* handle, u_int8_t *msg);//大小端调整
	PluginMsgNode* get_PluginNode_by_Name(const char * msgName);
	unsigned  int get_Messagecount();
	void Int64Chang(unsigned char * pData);
	int getIndex(Message * msg,string elementName);
	Message * decode_msg_by_name(const char * msgName, u_int8_t *msg,double * val);
	char * GetPacketNameByShowName(const char * show_Name);
	Message *  recode_msg_by_name(char * msgName, u_int8_t *msg,double * val);
	char * getParmentName(Message * msg,int index,int * len);
	char * getParmentResult(Message * msg,int index,int * len=0);
	char * getParmentNumberString(Message * msg,int index,int * len);
	int getParmentResultCnt(Message * msg);
	int getMessageNames(char * pMessageName);
	int getMessageSize(Message * msg);
	int getParmentResultInfo(Message * msg,int * packetlen,int * rt,int *subrt,int * msgformat,char * msgchoose,char * msgtype);
	void reflash(void);
	int getAnnotation_cnt(Message * msg,int index);
	char * getAnnotation(Message * msg,int parmindex,int annotationindex,int * len);
	int getAnnotationVal(Message * msg,int parmindex,int annotationindex);
	char * getMsgName(int i);
	int setValue(Message * msg,int index,const char * val);
	int setValue(Message * msg,int index,double val);
	int setValueByNumberString(Message * msg,int index,const  char * val);
	int getAnnotation_index(Message * msg,int index);
	int getMsgRawLength(Message * msg);
	string get_result_string(Message * pMessage);
	const char * get_markid(Message * pMessage,int index);
	int get_parmrawpos(Message * pMessage,int index,int * pintStart,int * pintCount);
	void * getValueData(Message * pMessage);
	int isDefault(Message * msg,int index);
	int getParmStartByte(Message * msg,int index); //起始字节
	int getParmByteCount(Message * msg,int index); //字节个数
	int getParmEndBit(Message * msg,int index);//结束位
	int getParmStartBit(Message * msg,int index);//起始位
	int getparmBitLength(Message * msg,int index); //参数的比特位总长度
	char * getparamID(Message * msg,int index);	//参数代号
	char * getparamName(Message * msg,int index);//参数名称
	char * getparamUnit(Message * msg,int index);//参数单位
	double getparamDefaultVal(Message * msg,int index);//默认值
	double getparamUpperLimit(Message * msg,int index);//上限
	double getparamLowerLimit(Message * msg,int index);//下限
	char * getMsgShowName(int i);
	static	Message *  CreateMessageByMessage(Message * pMessage);
	MessageDescriptor *  getMsgDescribe(int i);
	int setValue(Message * msg,const  char *  name,const  char * val);
	double getparamIsEnable(Message * msg,int index);  
	int setValueByNumberString(Message * msg,const  char *  name,const  char * val);
private:
	unsigned int GetIntValueFromString(string str);
	MsgLibrary msg_library;
	MsgLibrary *ptr_msglib;
	Decode(const char *cfg_file,const char * filefold,char * redisIP,int redisPort);
	Message * decode_msg_by_name_private(const char * msgName, u_int8_t *msg);
	void InitMessage(Message * msg,double * val=NULL);
	FILE *	pLogFile;
	vector<vector<string>> config_vector;
	map<string ,TableConfigInfo>  table_info_map;
	/////////////////////////////////////
	//类的函数
};