#define DECODELIBDLL
#include "StdAfx.h"
#include "DecodeDll.h"
#include <stdlib.h>
#include "UtilTool.h"
#include <vector>

static	Decode * decode=NULL;

#define  _IN
#define  _OUT
//void (Func) (_IN char * , _IN int * , _IN double* ,_OUT char * , _OUT int *,_OUT double*);


Decode::Decode(const char *cfg_file,const char * filefold, char * redisIP,int redisPort):pLogFile(NULL){
	memset(&msg_library,0,sizeof(msg_library));
	ptr_msglib=&msg_library;

	msglib_config(cfg_file, filefold, redisIP,redisPort);

}

Decode * Decode::GetInstance(const char * path,const char * filefold, char * ip , int portnum ){
	if (decode==NULL)
	{
		decode=new Decode(path, filefold,ip,portnum);
	}
	return decode;
}

int Decode::msglib_register(unsigned int msgTypeId,char * msgName, unsigned int msgLength)
{
	PluginMsgNode *node = (PluginMsgNode *) malloc(sizeof(PluginMsgNode));

	node->msgDescriptor = NULL;
	node->msgTypeId = msgTypeId;
	node->msgLength = msgLength;
	node->next = NULL;


	if (ptr_msglib->nodes.head == NULL)
	{
		ptr_msglib->nodes.head = node;
	}
	else
	{
		PluginMsgNode * tmp =(PluginMsgNode * ) ptr_msglib->nodes.head;
		PluginMsgNode * last = NULL;

		do
		{
			if (msgTypeId == tmp->msgTypeId)
			{
				free(node);
				return -1;
			}

			if (msgTypeId < tmp->msgTypeId)
			{
				break;
			}

			last = tmp;
			tmp = tmp->next;
		}
		while (tmp != NULL);

		if (last == NULL)
		{
			node->next = tmp;
			ptr_msglib->nodes.head = node;
		}
		else
		{
			node->next = tmp;
			last->next = node;
		}
	}

	ptr_msglib->nodes.count++;

	return 0;
}

/******************************************************************
*函数功能:解码库初始化。
*输入参数:none
*输出参数:none
*返回值  :none
*******************************************************************/
void Decode::reflash(void)
{
	decode=NULL;
}


/******************************************************************
*函数功能:解码库初始化。
*输入参数:none
*输出参数:none
*返回值  :none
*******************************************************************/
void Decode::rawdecodelib_init(void)
{
	memset(ptr_msglib, 0, sizeof(*ptr_msglib));
}

/******************************************************************
*函数功能:解析配置文件中的消息节点。
*输入参数:节点指针
*输出参数:none
*返回值  :none
*******************************************************************/
void Decode::decode_parse_messages_config(xmlElementPtr cur_element,const char * filefold,int * index)
{
	for (cur_element=cur_element->FirstChildElement(); cur_element; cur_element = cur_element->NextSiblingElement())
	{
		//if (cur_element->Type()!=NodeType::TINYXML_ELEMENT)
		//continue;

		if (!strcmp(cur_element->Value(),  MSG_MESSAGE_NODE_NAME))
			decode_parse_message_config(cur_element,filefold,index);
		//OutputDebugString(cur_element->Value());
		//OutputDebugString("\r\n");
	}
}

/******************************************************************
*函数功能:解析XML配置文件中的解码定义部分，得到XML中的name,cfg_file,id和size.
*输入参数:
*输出参数:none
*返回值  :none
*******************************************************************/
void Decode::decode_parse_message_config(xmlElementPtr xmlNodePtr,const char * filefold,int * index)
{
	char *cfg_file,*msg_choose,*msg_type;
	const char *tmp=NULL;
	char * cfg_showname=(char * )malloc (100*sizeof(char));
	char * cfg_msgid=(char * )malloc (100*sizeof(char));
	unsigned int id = 0;
	unsigned int size = 0;
	unsigned int rt=0;
	unsigned int subrt=0;
	unsigned int msgformat=0;
	xmlElementPtr  data; 
	cfg_file = (char *)malloc(300*sizeof(char));//路径名最长为300字节

	if (xmlNodePtr != NULL)
	{

		//for(data =xmlNodePtr->FirstChildElement(); data; data = data->NextSiblingElement())
		//{
		strcpy(cfg_file, filefold);
		strcat(cfg_file, xmlNodePtr->Attribute(MSG_CONF_FILE_ATTR_NAME));
		id = *index/*atoi(xmlNodePtr->Attribute(MSG_ID_ATTR_NAME))*/;

		size= xmlNodePtr->AttributeInt(MSG_SIZE_ATTR_NAME);
		rt= xmlNodePtr->AttributeInt(MSG_RT_ID);		
		subrt= xmlNodePtr->AttributeInt(MSG_SUBRT_ID);	
		msgformat= xmlNodePtr->AttributeInt(MSG_FORMAT);	

		tmp	=xmlNodePtr->Attribute(MSG_TYPE);
		if (tmp)
		{	
			msg_type = (char *)malloc(strlen(tmp));//路径名最长为300字节
			strcpy(msg_type, tmp);
		}
		tmp	=xmlNodePtr->Attribute(MSG_CHOOSE);
		if (tmp)
		{		
			msg_choose = (char *)malloc(strlen(tmp));//路径名最长为300字节
			strcpy(msg_choose, tmp);
		}

		strcpy(cfg_file, filefold);

		//		(*idmap)[atoi(xmlNodePtr->Attribute(MSG_DISTINCT_ID))]=id;
		memset(cfg_showname,0,sizeof(cfg_showname));
		//CString str=xmlNodePtr->Attribute(MSG_CONF_FILE_ATTR_SHOWNAME);

		strcat(cfg_showname,xmlNodePtr->Attribute(MSG_CONF_FILE_ATTR_SHOWNAME));


		memset(cfg_msgid,0,sizeof(cfg_msgid));
		//CString str=xmlNodePtr->Attribute(MSG_CONF_FILE_ATTR_SHOWNAME);

		strcat(cfg_msgid,xmlNodePtr->Attribute(MSG_ID_ATTR_NAME));


		(*index)++;
		//}
		//xmlNodePtr = xmlNodePtr->NextSiblingElement();
	}

	if (cfg_file != NULL)
	{
		PluginMsgNode* node = NULL;
		msglib_register(id,cfg_showname, size);
		node = get_PluginNode_by_ID(id);
		if (node)
		{
			node->msgDescriptor = (MessageDescriptor*)malloc(sizeof(*node->msgDescriptor));

			node->msgDescriptor->param_cnt = 0;
			node->msgDescriptor->parameters = NULL;
			node->msgDescriptor->msg_size = size;
			node->msgDescriptor->rt = rt;			
			node->msgDescriptor->subrt = subrt;			
			node->msgDescriptor->msgformat = msgformat;
			node->msgDescriptor->cshowname = cfg_showname;
			node->msgDescriptor->name = cfg_msgid;
			//	node->msgDescriptor->msg_type = msg_type;
			//	node->msgDescriptor->msg_choose = msg_choose;

			parse_decodeXLS(node->msgDescriptor, cfg_file);
			node->msgName=node->msgDescriptor->name;
		}
	}
	else
		free(cfg_file);
}

/******************************************************************
*函数功能:通过比较消息的ID号给出解码队列节点。
*输入参数:ID号
*输出参数:none
*返回值  :PluginMsgNode指针
*******************************************************************/
PluginMsgNode* Decode::get_PluginNode_by_ID(unsigned int msgTypeId)
{
	PluginMsgNode *node = (PluginMsgNode*) ptr_msglib->nodes.head;

	while (node)
	{
		if (msgTypeId == node->msgTypeId)
			return node;

		node = node->next;
	}

	return NULL;
}

/******************************************************************
*函数功能:通过比较消息的Name给出解码队列节点。
*输入参数:Name号
*输出参数:none
*返回值  :PluginMsgNode指针
*******************************************************************/
PluginMsgNode* Decode::get_PluginNode_by_Name(const char * msgName)
{
	PluginMsgNode *node = (PluginMsgNode*) ptr_msglib->nodes.head;

	while (node)
	{
		if (!strcmp(node->msgName,msgName)){
			if (node->msgDescriptor->param_cnt==0)
			{
				return NULL;
			}
			return node;
		}
		node = node->next;
	}

	return NULL;
}

map<string ,TableConfigInfo> GetTableInfoMap(vector<vector<string>> info_vector){
	map<string ,TableConfigInfo>  table_info_map_temp;

	for(int i=0;i<info_vector.size()-1;i++){
		if (info_vector[i].size()==1&&info_vector[i+1].size()==2)
		{
			TableConfigInfo table_info_element;
			table_info_element.iStart=i;
			table_info_element.iRows=atoi(info_vector[i+1][0].c_str());
			table_info_element.iCols=atoi(info_vector[i+1][1].c_str());
			table_info_map_temp[info_vector[i][0]]=table_info_element;
		}
	}
	return table_info_map_temp;
}
/******************************************************************
*函数功能:解码库配置
*输入参数:XML配置文件
*输出参数:none
*返回值  :0：成功；-1：失败
*******************************************************************/
int Decode::msglib_config(const char *cfg_file,const char * filefold,const char * redisIP,int redisPort)
{
	TiXmlDocument doc;
	TiXmlElement * root;
	TiXmlElement * cur;
	string timestr="log.txt"/*UtilTool::GetTimeString()*/;
	timestr+="_log.txt";
	pLogFile = fopen(timestr.c_str(),"wb");  //建立txt文件,文件名为时间

	fprintf(pLogFile,"%s:日志文件创建成功\n",UtilTool::GetTimeString().c_str(),1);

	char xmlpath[500];

	strcpy(xmlpath,filefold);

	strcat(xmlpath,cfg_file);

	if (!doc.LoadFile(xmlpath))
		return -1;
	config_vector=	UtilTool::readblank("1.txt",',',true);
	table_info_map=GetTableInfoMap(config_vector);
	root=doc.RootElement();
	if (root == NULL || strcmp(root->Value(), (const char*) MSG_ROOT_NAME))
		return -1;

	int index=0;

	for (cur=root->FirstChildElement();cur; cur = cur->NextSiblingElement())
	{
		/*	if (cur->type !=TINYXML_ELEMENT )
		continue;*/

		if (!strcmp(cur->Value(), (const char *) MSG_MESSAGES_NODE_NAME))
		{
			decode_parse_messages_config(cur,filefold,&index);
		}
		//OutputDebugString(cur->Value());
		//OutputDebugString("\r\n");
	}

	ptr_msglib->count=index;

	doc.Clear();
	config_vector.clear();
	return 0;
}

/******************************************************************
*函数功能:。
*输入参数:
*输出参数:none
*返回值  :
*******************************************************************/
Message* Decode::create_msg_by_ID(unsigned int msgTypeId)
{
	Message* msg;
	PluginMsgNode* node = get_PluginNode_by_ID(msgTypeId);

	if (node == NULL || node->msgDescriptor == NULL)
		return NULL;

	msg = (Message*) malloc(sizeof(Message));
	msg->msg_descriptor = node->msgDescriptor;
	msg->param_value =(ParameterValue *) malloc(node->msgDescriptor->param_cnt * sizeof(ParameterValue));
	if(msg->param_value==NULL)
		return NULL;
	memset(msg->param_value, 0, node->msgDescriptor->param_cnt * sizeof(ParameterValue));
	InitMessage(msg);
	return msg;
}

Message* Decode::create_msg_by_ShowName(const char * msgShowName,double * val){
	Message* msg;
	PluginMsgNode *node = (PluginMsgNode*) ptr_msglib->nodes.head;

	while (node)
	{
		if (!strcmp(node->msgDescriptor->cshowname,msgShowName)){
			if (node->msgDescriptor->param_cnt==0)
			{
				break;
			}
			break;
		}
		node = node->next;
	}


	if (node == NULL || node->msgDescriptor == NULL)
		return NULL;

	msg = (Message*) malloc(sizeof(Message));
	msg->msg_descriptor = node->msgDescriptor;
	msg->param_value =(ParameterValue *) malloc(node->msgDescriptor->param_cnt * sizeof(ParameterValue));
	if(msg->param_value==NULL)
		return NULL;
	memset(msg->param_value, 0, node->msgDescriptor->param_cnt * sizeof(ParameterValue));

	InitMessage(msg,val);

	return msg;
}
/******************************************************************
*函数功能:。
*输入参数:
*输出参数:none
*返回值  :
*******************************************************************/
Message* Decode::create_msg_by_Name(const char * msgName,double * val)
{
	Message* msg;
	PluginMsgNode* node = get_PluginNode_by_Name(msgName);

	if (node == NULL || node->msgDescriptor == NULL)
		return NULL;

	msg =new Message/* (Message*) malloc(sizeof(Message))*/;
	msg->msg_descriptor = node->msgDescriptor;
	msg->param_value =(ParameterValue *) malloc(node->msgDescriptor->param_cnt * sizeof(ParameterValue));
	if(msg->param_value==NULL)
		return NULL;
	memset(msg->param_value, 0, node->msgDescriptor->param_cnt * sizeof(ParameterValue));

	InitMessage(msg,val);

	return msg;
}

/******************************************************************
*函数功能:。
*给函数赋默认值:
*输出参数:none
*返回值  :
*******************************************************************/
void Decode::InitMessage(Message * msg,double * val)
{

	if (msg)
	{

		for (int index=0;index<msg->msg_descriptor->param_cnt;index++)
		{	
			ParameterDescriptor *parameters=&msg->msg_descriptor->parameters[index];
			ParameterValue *param_value=&msg->param_value[index];
			if (val)
			{
				val[index]=parameters->defaultval;
			}		

			switch(parameters->display_type){
			case SHOWTYPE_FLOAT16_ID:	
				BIT32_UNION temp32;
				param_value->v.floatvalue=parameters->defaultval;
				temp32.m_Long=param_value->v.rawvalue;
				temp32.m_Long>>=16;
				param_value->v.rawvalue=temp32.m_Long;
				break;
			case SHOWTYPE_DOUBLE_ID:	
				param_value->v.doublevalue=parameters->defaultval;
				break;
			case SHOWTYPE_FLOAT_ID:		
				param_value->v.floatvalue=parameters->defaultval;
				break;
			case SHOWTYPE_ARRAY_ID:
				memcpy(param_value->rawbytes,parameters->defaultary,parameters->byte_count);
				break;
			default:	
				param_value->v.rawvalue=parameters->defaultval;;
				break;
			}
		}
	}
}



/******************************************************************
*函数功能:解码模块入口。
*输入参数:网络消息指针，消息缓冲区指针，消息字节数
*输出参数:none
*返回值  :
*******************************************************************/
int Decode::msg_decode_handle(Message* handle, u_int8_t *msg)
{
	MessageDescriptor *msgDescriptor = handle->msg_descriptor;
	if (msgDescriptor == NULL )
		return -1;

	return decode_msg(handle, msg);
}

/******************************************************************
*函数功能:解码模块入口。
*输入参数:网络消息指针，消息缓冲区指针，消息字节数
*输出参数:none
*返回值  :
*******************************************************************/
int Decode::msg_recode_handle(Message* handle, u_int8_t *msg)
{
	MessageDescriptor *msgDescriptor = handle->msg_descriptor;
	if (msgDescriptor == NULL)
		return -1;

	return recode_msg(handle, msg);
}



/******************************************************************
*函数功能:读取解码表并将数据类型、显示类型等赋给TGNCCParameterDescriptor。
*输入参数:解码表路径path
*输出参数:TGNCCMessageDescriptor
*返回值  :成功：0；异常：-1。
*******************************************************************/
int Decode::parse_decodeXLS(MessageDescriptor* msg, const char * path)
{
	char * messagename=msg->name;
	char *cell_str;
	int cell_strlength = 0;
	int current_row, current_column;
	struct st_row_data* row;
	unsigned int index = 0;
	ParameterDescriptor *parameter;
	string tablename=msg->name;
	TableConfigInfo table_info=	table_info_map[tablename];
	//获取已使用表格行列数
	int iRows =table_info.iRows;

	int iCols =table_info.iCols;

	if (!iCols&&!iRows)
	{
		msg->param_cnt = 0;
		msg->parameters =NULL;
		return -1;
	}


	msg->param_cnt = iRows-1;
	msg->value_size=msg->param_cnt*sizeof(ParameterValue);
	msg->parameters =(ParameterDescriptor *) malloc(msg->param_cnt * sizeof(ParameterDescriptor));
	memset(msg->parameters, 0, msg->param_cnt * sizeof(ParameterDescriptor));
	vector<string> strvec;
	string table_cell_string;
	//printf("开始读入表格：%s，共%d行。\n", path, msg->paramCount);
	for (current_row = 2; current_row <=iRows; current_row++)
	{
		parameter = &msg->parameters[current_row - 2];
		float f=0;					
		int defaultval=0;
		int  index = 0;

		for (current_column = 1; current_column < config_vector[table_info.iStart+current_row].size()+1/*pWS->rows.lastcol*/; current_column++)
		{
			cell_str = (char*)config_vector[table_info.iStart+current_row][current_column-1].c_str()/*(char *) row->cells.cell[current_column].str*/;
			//	printf("%s %d %d %s\n" ,messagename, current_column,current_row,cell_str);
			if (cell_str != NULL)
			{
				cell_strlength = strlen(cell_str);
				switch (current_column-1)
				{
				case DECODEXLS_PARAM_ID_COL: // 参数代号
					index = 0;
					if (cell_strlength > 61) //最多支持61字节（包括终止符在内）
					{
						strncpy(parameter->paramID, cell_str, 60);
						parameter->paramID[60] = '\0';
					}
					else
					{			

						string parm_name;

						parm_name+=cell_str;

						parameter->parm_index=current_row-2;


						strcpy(parameter->paramID, cell_str);
					}
					break;

				case DECODEXLS_PARAM_NAME_COL: // 参数名称
					if (cell_strlength > 41) //最多支持41字节
					{

						strncpy(parameter->paramName, cell_str, sizeof(parameter->paramName) - 1);
						parameter->paramName[40] = '\0';
					}
					else
					{
						strcpy(parameter->paramName, cell_str);
					}
					break;

				case DECODEXLS_START_BYTE_COL: // 起始字节
					parameter->start_byte = strtol(cell_str, NULL, 10)+1;
					break;

				case DECODEXLS_BYTE_COUNT_COL: // 字节个数
					parameter->byte_count = strtol(cell_str, NULL, 10);
					break;

				case DECODEXLS_BYTE_ORDER_COL: //字节位顺序

					if (strcmp(cell_str, UTF8_STRING_HIGH_BYTE_FIRST) == 0) //高字节在前
					{
						parameter->byte_order = RAW_BYTE_ORDER_HIGH_FIRST;
					}
					else if (strcmp(cell_str, UTF8_STRING_LOW_BYTE_FIRST) == 0) //低字节在前
					{
						parameter->byte_order = RAW_BYTE_ORDER_LOW_FIRST;
					}
					else if (strcmp(cell_str, UTF8_STRING_LOW_BIT_FIRST) == 0) //低位在前
					{
						parameter->byte_order = RAW_BYTE_ORDER_LOWBIT_FIRST;
					}	
					else if (strcmp(cell_str, UTF8_STRING_LOW_PARM_FIRST) == 0) //低位在前
					{
						parameter->byte_order = RAW_BYTE_ORDER_LOWPARM_FIRST;
					}
					else //配置表填错了，默认按照高字节在前处理
					{
						fprintf(pLogFile,"%s:文件：%s，第%u行：字节顺序错误！服务器程序已退出！\n",UtilTool::GetTimeString().c_str(), messagename, current_row);
						//	exit(EXIT_FAILURE);
					}

					break;

				case DECODEXLS_DATA_TYPE_COL: //数据类型
					if (strcmp(cell_str, UTF8_STRING_RAW_ENCODE) == 0) //原码
					{
						parameter->data_type = 0;
					}
					else if (strcmp(cell_str, UTF8_STRING_COMPLEMENT_ENCODE) == 0) //补码
					{
						parameter->data_type = 1;
					}
					else if (strcmp(cell_str, UTF8_STRING_FLOAT_ENCODE) == 0) //浮点
					{
						parameter->data_type = 2;
					}
					else //默认都按照原码方式处理
					{
						fprintf(pLogFile,"%s:文件：%s，第%u行：数据类型错误！服务器程序已退出！\n",UtilTool::GetTimeString().c_str(), path, current_row);
						//		exit(EXIT_FAILURE);
					}
					break;

				case DECODEXLS_END_BIT_COL: //终止位: 0..
					parameter->start_bit = strtol(cell_str, NULL, 10);		
					break;

				case DECODEXLS_START_BIT_COL: //起始位: 63/7..
					parameter->bit_length= strtol(cell_str, NULL, 10);
					parameter->end_bit=parameter->start_bit+parameter->bit_length-1;
					if (parameter->byte_order == RAW_BYTE_ORDER_LOWPARM_FIRST)
					{
						parameter->start_bit=8*parameter->byte_count -parameter->start_bit-1;
						parameter->end_bit=8*parameter->byte_count -parameter->end_bit-1;
						parameter->byte_order = RAW_BYTE_ORDER_HIGH_FIRST;
					}else
					{
						unsigned int temp=parameter->start_bit;
						parameter->start_bit=parameter->end_bit;
						parameter->end_bit=temp;
					}
					break;

				case DECODEXLS_SIGN_BYTE_COL: //符号位字节
					if (strcmp(cell_str, UTF8_STRING_NONE) == 0)
					{
						parameter->sign_byte = -1;
					}
					else
					{
						parameter->sign_byte = strtol(cell_str, NULL, 10);
					}
					break;

				case DECODEXLS_SIGN_BIT_COL: //符号占位
					if (strcmp(cell_str, UTF8_STRING_NONE) == 0)
					{
						parameter->sign_bit = -1;
					}
					else
					{
						parameter->sign_bit = strtol(cell_str, NULL, 10);
					}
					break;

				case DECODEXLS_EQUIVALENT_COL: //当量
					if ((strcmp(cell_str, UTF8_STRING_NONE) == 0) || (strcmp(cell_str, UTF8_STRING_1) == 0))
					{
						parameter->equivalent = 0.0;
					}
					else
					{
						parameter->equivalent = atof(cell_str);
					}
					break;

				case DECODEXLS_DISPLAY_TYPE_COL: //显示类型
					if (strcmp(cell_str, SHOWTYPE_DOUBLE) == 0)
					{
						parameter->display_type = SHOWTYPE_DOUBLE_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_FLOAT) == 0)
					{
						parameter->display_type = SHOWTYPE_FLOAT_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_FLOAT16) == 0)
					{
						parameter->display_type = SHOWTYPE_FLOAT16_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_INT64) == 0)
					{
						parameter->display_type = SHOWTYPE_INT64_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_INT32) == 0)
					{
						parameter->display_type = SHOWTYPE_INT32_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_INT16) == 0)
					{
						parameter->display_type = SHOWTYPE_INT16_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_INT8) == 0)
					{
						parameter->display_type = SHOWTYPE_INT8_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_UINT64) == 0)
					{
						parameter->display_type = SHOWTYPE_UINT64_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_UINT32) == 0)
					{
						parameter->display_type = SHOWTYPE_UINT32_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_UINT16) == 0)
					{
						parameter->display_type = SHOWTYPE_UINT16_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_UINT8) == 0)
					{
						parameter->display_type = SHOWTYPE_UINT8_ID;
					}
					else if (strcmp(cell_str, SHOWTYPE_ARRAY) == 0)
					{
						parameter->display_type = SHOWTYPE_ARRAY_ID;
					}
					else //如果填错了
					{
						fprintf(pLogFile,"%s:文件：%s 第%u行：显示类型错误，请检查！服务器程序已退出！\n",UtilTool::GetTimeString().c_str(), path, current_row);
						//		exit(EXIT_FAILURE);
					}
					break;

				case DECODEXLS_PARAM_UNIT_COL: //单位
					if (cell_strlength > 16) //最多支持16字节（包括终止符在内）
					{
						strncpy(parameter->paramUnit, cell_str, 15);
						parameter->paramUnit[15] = '\0';
					}
					else
					{
						strcpy(parameter->paramUnit, cell_str);
					}
					break;
				case DECODEXLS_PARAM_IS_ENABLE: //是否赋值
					if (strcmp(cell_str, UTF8_IS_ENABLE) == 0){
						parameter->is_enable=1;
					}else
						parameter->is_enable=0;
					break;
					
				case DECODEXLS_DEFAULTVALUE_COL: //默认值	
					parameter->defaultval=0;	

					if (cell_strlength)
					{
						switch(parameter->display_type){
						case SHOWTYPE_DOUBLE_ID:
							parameter->defaultval=atof(cell_str);
							//sscanf( cell_str,"%lf",&parameter->defaultval);
							break;
						case SHOWTYPE_FLOAT_ID:
							parameter->defaultval=atof(cell_str);
							//f=0;
							//sscanf( cell_str,"%f",&f);
							//parameter->defaultval=f;
							break;
						case SHOWTYPE_FLOAT16_ID:
							parameter->defaultval=atof(cell_str);
							//f=0;
							//sscanf( cell_str,"%f",&f);
							//parameter->defaultval=f;
							break;
						case SHOWTYPE_ARRAY_ID:
							strvec= UtilTool::split_string(cell_str);
							for(int i=0;i<strvec.size();i++)
							{
								parameter->defaultary[i]=GetIntValueFromString(strvec[i]);
							}
							break;
						default:
							table_cell_string=cell_str;
							parameter->defaultval=GetIntValueFromString(table_cell_string);
							break;
						}
					}
					break;
					//case DECODEXLS_ANNOTATION_COUNT_COL: //注释循环次数
					//	if (cell_str != NULL) //若有注释才处理
					//	{
					//		parameter->annotation_times = strtol(cell_str, NULL, 10);
					//	}
					//	else
					//	{
					//		parameter->annotation_times = 0; //做保护
					//	}
					//	break;

				case DECODEXLS_RADIX_COL: //显示进制
					if (strcmp(cell_str, RADIX_BINARY) == 0)
					{
						parameter->radix_type = 2;
					}else if (strcmp(cell_str, RADIX_HEXADECIMAL) == 0)
					{
						parameter->radix_type = 16;
					}else if (strcmp(cell_str, RADIX_OCTONARY) == 0)
					{
						parameter->radix_type = 8;
					}else
						parameter->radix_type = 10;
					break;
				case DECODEXLS_FORMULA_COL:
					memcpy(parameter->formulaName,cell_str,strlen(cell_str));
					break;
				case DECODEXLS_A_COL: //注释循环次数
					sscanf( cell_str,"%lf",&parameter->avalue);
					break;
				case DECODEXLS_B_COL: //注释循环次数
					sscanf( cell_str,"%lf",&parameter->bvalue);
					break;
				case DECODEXLS_C_COL: //注释循环次数
					sscanf( cell_str,"%lf",&parameter->cvalue);
					break;
				case DECODEXLS_UPPERLIMIT_COL: //注释循环次数
					parameter->upperlimit=0;						
					if (cell_strlength)
					{
						switch(parameter->display_type){
						case SHOWTYPE_DOUBLE_ID:
							sscanf( cell_str,"%lf",&parameter->upperlimit);
							break;
						case SHOWTYPE_FLOAT_ID:
							f=0;
							sscanf( cell_str,"%f",&f);
							parameter->defaultval=f;
							break;
						default:
							table_cell_string=cell_str;
							parameter->upperlimit=GetIntValueFromString(table_cell_string);
							break;
						}

					}
					break;

				case DECODEXLS_LOWERLIMIT_COL: //注释循环次数
					parameter->lowerlimit=0;						
					if (cell_strlength)
					{
						switch(parameter->display_type){
						case SHOWTYPE_DOUBLE_ID:
							sscanf( cell_str,"%lf",&parameter->lowerlimit);
							break;
						case SHOWTYPE_FLOAT_ID:
							f=0;
							sscanf( cell_str,"%f",&f);
							parameter->defaultval=f;
							break;
						default:
							table_cell_string=cell_str;
							parameter->lowerlimit=GetIntValueFromString(table_cell_string);
							break;
						}
					}	
					break;

				default:
					break;

				}
			}
			//	parameter->bit_length = parameter->start_bit - parameter->end_bit + 1;



			//printf("读取表格文件：%s，第%d行完毕！\n", path, line);

			//注释循环程序文字显示
			int zs_lengh; //注释文字的总长度
			size_t n_str; //冒号的位置
			if (current_column>DECODEXLS_RADIX_COL +5 && (parameter->display_type > 2) && (parameter->display_type < 11)) //有注释且显示类型为原码或补码
			{
				//从“注释循环次数”后面这一列开始是注释内容，一直到16+注释循环次数这一列

				if (cell_str==0)
				{
					break;
				}
				if (strlen(cell_str)==0)
				{		
					parameter->annotation_cnt=index;
					break;
				}

				int annotation_val=-1;


				table_cell_string=cell_str;


				int pos=table_cell_string.find(':');

				if (pos==-1)
				{
					fprintf(pLogFile,"---------------------------------\n");
					fprintf(pLogFile,"%s:解码表：%s第%d行,注释%d填写错误!请检查!!!\n",UtilTool::GetTimeString().c_str(), path, current_row, current_column - DECODEXLS_RADIX_COL);
					fprintf(pLogFile,"---------------------------------\n");

					continue;
				}

				table_cell_string.erase(pos);
				annotation_val=GetIntValueFromString(table_cell_string);

				zs_lengh = strlen(cell_str);
				n_str = strcspn(cell_str, ":"); //原码部分的长度

				if (strcmp(cell_str, UTF8_STRING_NULL)) //如果注释单元格式内容不为空
				{
					char *str1 = strchr(cell_str, ':'); //:后的汉字的起始指针
					if (str1 != NULL) //找到汉字部分
					{
						str1++;
						strncpy(&parameter->annotation_str[index][0], str1, zs_lengh - n_str);
						parameter->annotation_str[index][zs_lengh - n_str] = '\0';
						parameter->annotation_val[index]=annotation_val;

					}
					else
					{
						fprintf(pLogFile,"---------------------------------\n");
						fprintf(pLogFile,"%s:解码表：%s第%d行,注释%d填写错误!服务器程序已退出!!!\n",UtilTool::GetTimeString().c_str(), path, current_row, current_column - DECODEXLS_RADIX_COL);
						fprintf(pLogFile,"---------------------------------\n");
						//		exit(EXIT_FAILURE);
					}

					//原码部分
					strncpy(&parameter->annotation_raw[index][0], cell_str, n_str);
					parameter->annotation_raw[index][n_str] = '\0';

					index++;
					parameter->annotation_cnt=index;
				}
				/*	if (index != parameter->annotation_times)
				{
				printf("解码表：%s，第%d行出错!实际注释次数=%d,表中注释次数:%d。\n", path, current_row, index, parameter->annotation_times);
				return -1;
				}*/
			}




		}

	}
	return 0;
}

unsigned int  Decode::GetIntValueFromString(string str){
	int radix=10;
	if (str.find('b')!=-1)
	{
		radix=2;
	}else if (str.find('h')!=-1)
	{
		radix=16;
	}
	return strtoul(str.c_str(), NULL, radix );
}

void Decode::Int64Chang(unsigned char * pData){
	unsigned char temp;
	for (int i=0;i<4;i++)
	{
		temp=pData[i];
		pData[i]=pData[7-i];
		pData[7-i]=temp;
	}
}

Message * Decode::decode_msg_by_name_private(const char * msgName, u_int8_t *msg){
	Message * pMessage=create_msg_by_Name(msgName);
	if (pMessage!=NULL)
	{
		decode_msg(pMessage,msg);
	}
	return pMessage;
}

string Decode::get_result_string(Message * pMessage){
	string result;
	if (pMessage!=NULL)
	{	
		ParameterValue *param_value=pMessage->param_value;//处理后的参数属性
		ParameterDescriptor *parameters=pMessage->msg_descriptor->parameters;
		int len=0;
		for (int i=0;i<getParmentResultCnt(pMessage);i++)
		{		
			string str=getParmentName(pMessage,i,&len);
			result+=str;
			result+=":";
			str=getParmentResult(pMessage,i,&len);
			result+=str;
			result+="\n";
		}
	}
	return result;
}

char *  Decode::GetPacketNameByShowName(const char * show_Name){

	PluginMsgNode *node = (PluginMsgNode*) ptr_msglib->nodes.head;

	while (node)
	{
		if (!strcmp(node->msgDescriptor->cshowname,show_Name)){
			if (node->msgDescriptor->param_cnt==0)
			{
				break;
			}
			break;
		}
		node = node->next;
	}


	if (node == NULL || node->msgDescriptor == NULL)
		return NULL;

	return node->msgName;
}

Message *  Decode::decode_msg_by_name(const char * msgName, u_int8_t *msg,double * val){
	Message * pMessage=decode_msg_by_name_private(msgName,msg);
	if (pMessage!=NULL)
	{	
		ParameterValue *param_value=pMessage->param_value;//处理后的参数属性
		ParameterDescriptor *parameters=pMessage->msg_descriptor->parameters;
		if (val)
			for (int i=0;i<pMessage->msg_descriptor->param_cnt;i++)
			{
				switch(parameters[i].display_type){
				case SHOWTYPE_DOUBLE_ID:
					val[i]= (float)param_value[i].v.doublevalue;
					break;
				case SHOWTYPE_FLOAT_ID:
					val[i]= (float)param_value[i].v.floatvalue;
					break;
				default:
					val[i]= param_value[i].v.rawvalue;
					break;
				}
			}
			//free(pMessage->param_value) ;
			//free(pMessage) ;
	}
	return pMessage;
}

Message *  Decode::recode_msg_by_name(char * msgName, u_int8_t *msg,double * val){
	Message * pMessage=create_msg_by_Name(msgName);
	if (pMessage!=NULL)
	{	
		ParameterValue *param_value=pMessage->param_value;//处理后的参数属性
		ParameterDescriptor *parameters=pMessage->msg_descriptor->parameters;
		for (int i=0;i<pMessage->msg_descriptor->param_cnt;i++)
		{
			switch(parameters[i].display_type){
			case SHOWTYPE_DOUBLE_ID:
				param_value[i].v.doublevalue=val[i];
				break;
			case SHOWTYPE_FLOAT_ID:
				param_value[i].v.floatvalue=val[i];
				break;
			default:
				param_value[i].v.rawvalue=val[i];
				break;
			}
		}
		recode_msg(pMessage,msg);

		//free(pMessage->param_value) ;
		//free(pMessage);
	}	
	return pMessage;
}

int Decode::decode_msg( Message* handle, u_int8_t *msg){
	int param_cnt= handle->msg_descriptor->param_cnt;
	return decode_msgbylen(handle,msg,param_cnt);
}

/******************************************************************
*函数功能:解算流程。
*输入参数:
*输出参数:
*返回值  :
*******************************************************************/
int Decode::decode_msgbylen(Message* handle, u_int8_t *msg,int msgcnt )
{
	int index = 0;unsigned int paramCount = 0;
	unsigned int start_byte;
	MessageDescriptor *msgDescriptor = handle->msg_descriptor;
	if (msgcnt>msgDescriptor->param_cnt)
	{
		paramCount=msgDescriptor->param_cnt;
	}
	else
	{
		paramCount=msgcnt;
	}//防止溢出出错

	ParameterDescriptor *param = NULL;
	ParameterValue *paramValue;

	for (index = 0; index < paramCount; index++)
	{
		paramValue = &handle->param_value[index];
		param = &msgDescriptor->parameters[index];
		start_byte = param->start_byte;
		//增加对起始字节填写错误的判断
		if(start_byte < 1)
		{
			fprintf(pLogFile,"%s:参数%s的起始字节错误,服务器程序已退出.\n",UtilTool::GetTimeString().c_str(), param->paramID);
			//exit(EXIT_FAILURE);
		}

		//根据字节个数处理
		if ((param->byte_count <= 8) && (param->byte_count >= 1))
		{
			switch (param->byte_count)
			{
			case 1:
				{
					BIT8_UNION temp8;
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST) //默认高字节在前
					{
						memcpy(&temp8.m_Bytes[0], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp8.m_Bytes[0], &msg[param->start_byte - 1], 1);
						unsigned char tempswap[8]; //用来交换数据临时变量
						BIT8_UNION_DETAIL tempResult; //交换，存储缓存
						BYTE s1 = 0;
						int k = 0;
						for (k = param->end_bit; k <= param->start_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp8.m_Bytes[0]) >> k; //获取原始位值右移K
						}
						temp8.m_Bytes[0] = 0;
						int i = 0;
						for (i = 0; i < 8; i++)
						{
							switch (i)
							{
							case 0:
								tempResult.IO_bit.IO_0 = tempswap[7];
								break;

							case 1:
								tempResult.IO_bit.IO_1 = tempswap[6];
								break;

							case 2:
								tempResult.IO_bit.IO_2 = tempswap[5];
								break;

							case 3:
								tempResult.IO_bit.IO_3 = tempswap[4];
								break;

							case 4:
								tempResult.IO_bit.IO_4 = tempswap[3];
								break;

							case 5:
								tempResult.IO_bit.IO_5 = tempswap[2];
								break;

							case 6:
								tempResult.IO_bit.IO_6 = tempswap[1];
								break;

							case 7:
								tempResult.IO_bit.IO_7 = tempswap[0];
								break;
							}
						}
						temp8.m_uInt8 = tempResult.m_Bytes;
					}
					else //如果写成低字节在前，使用默认方法解析，因为就1个字节
					{
						memcpy(&temp8.m_Bytes[0], &msg[param->start_byte - 1], 1);
					}
					temp8.m_uInt8 &= (0xff >> (8 - (param->start_bit + 1))); //BITEND为7
					paramValue->v.rawvalue = (temp8.m_uInt8 >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
				}
				break;

			case 2:
				{
					BIT16_UNION temp16;
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST) //高字节在前
					{
						memcpy(&temp16.m_Bytes[0], &msg[param->start_byte - 1], 2);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						memcpy(&temp16.m_Bytes[0], &msg[param->start_byte], 1);
						memcpy(&temp16.m_Bytes[1], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp16.m_Bytes[0], &msg[param->start_byte - 1], 2);
						unsigned char tempswap[16]; //用来交换数据临时变量
						BIT16_UNION_DETAIL tempResule; //交换，存储缓存
						unsigned short s1 = 0;
						int k = 0;
						for (k = param->end_bit; k <= param->start_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp16.m_Word) >> k; //获取原始位值右移K
						}
						temp16.m_Word = 0;
						int i = 0;
						for (i = 0; i < 16; i++)
						{
							switch (i)
							{
							case 0:
								tempResule.IO_bit.IO_0 = tempswap[15];
								break;

							case 1:
								tempResule.IO_bit.IO_1 = tempswap[14];
								break;

							case 2:
								tempResule.IO_bit.IO_2 = tempswap[13];
								break;

							case 3:
								tempResule.IO_bit.IO_3 = tempswap[12];
								break;

							case 4:
								tempResule.IO_bit.IO_4 = tempswap[11];
								break;

							case 5:
								tempResule.IO_bit.IO_5 = tempswap[10];
								break;

							case 6:
								tempResule.IO_bit.IO_6 = tempswap[9];
								break;

							case 7:
								tempResule.IO_bit.IO_7 = tempswap[8];
								break;

							case 8:
								tempResule.IO_bit.IO_8 = tempswap[7];
								break;

							case 9:
								tempResule.IO_bit.IO_9 = tempswap[6];
								break;

							case 10:
								tempResule.IO_bit.IO_10 = tempswap[5];
								break;

							case 11:
								tempResule.IO_bit.IO_11 = tempswap[4];
								break;

							case 12:
								tempResule.IO_bit.IO_12 = tempswap[3];
								break;

							case 13:
								tempResule.IO_bit.IO_13 = tempswap[2];
								break;

							case 14:
								tempResule.IO_bit.IO_14 = tempswap[1];
								break;

							case 15:
								tempResule.IO_bit.IO_15 = tempswap[0];
								break;
							}
						}
						temp16.m_Word = tempResule.m_Word; //赋值
					}
					if (param->display_type!=SHOWTYPE_FLOAT16_ID)
						temp16.m_Word = ntohs(temp16.m_Word);
					temp16.m_Word &= (0xffff >> (16 - (param->start_bit + 1)));
					paramValue->v.rawvalue = (temp16.m_Word >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
				}
				break;
			case 3: //3个
				{
					BIT32_UNION temp32;
					memset(&temp32.m_Bytes[0], 0, 1);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST) //高字节在前
					{
						memcpy(&temp32.m_Bytes[1], &msg[param->start_byte - 1], 3);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						memcpy(&temp32.m_Bytes[1], &msg[param->start_byte + 1], 1);
						memcpy(&temp32.m_Bytes[2], &msg[param->start_byte], 1);
						memcpy(&temp32.m_Bytes[3], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp32.m_Bytes[1], &msg[param->start_byte - 1], 3);
						unsigned char tempswap[24]; //用来交换数据临时变量
						BIT32_UNION_DETAIL tempResule; //交换，存储缓存
						unsigned int s1 = 0;
						int k = 0;
						for (k = param->end_bit; k <= param->start_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp32.m_Long) >> k; //获取原始位值右移K
						}
						temp32.m_Long = 0;
						int i = 0;
						for (i = 0; i < 24; i++)
						{
							switch (i)
							{
							case 0:
								tempResule.IO_bit.IO_0 = tempswap[23];
								break;

							case 1:
								tempResule.IO_bit.IO_1 = tempswap[22];
								break;

							case 2:
								tempResule.IO_bit.IO_2 = tempswap[21];
								break;

							case 3:
								tempResule.IO_bit.IO_3 = tempswap[20];
								break;

							case 4:
								tempResule.IO_bit.IO_4 = tempswap[19];
								break;

							case 5:
								tempResule.IO_bit.IO_5 = tempswap[18];
								break;

							case 6:
								tempResule.IO_bit.IO_6 = tempswap[17];
								break;

							case 7:
								tempResule.IO_bit.IO_7 = tempswap[16];
								break;

							case 8:
								tempResule.IO_bit.IO_8 = tempswap[15];
								break;

							case 9:
								tempResule.IO_bit.IO_9 = tempswap[14];
								break;

							case 10:
								tempResule.IO_bit.IO_10 = tempswap[13];
								break;

							case 11:
								tempResule.IO_bit.IO_11 = tempswap[12];
								break;

							case 12:
								tempResule.IO_bit.IO_12 = tempswap[11];
								break;

							case 13:
								tempResule.IO_bit.IO_13 = tempswap[10];
								break;

							case 14:
								tempResule.IO_bit.IO_14 = tempswap[9];
								break;

							case 15:
								tempResule.IO_bit.IO_15 = tempswap[8];
								break;

							case 16:
								tempResule.IO_bit.IO_16 = tempswap[7];
								break;

							case 17:
								tempResule.IO_bit.IO_17 = tempswap[6];
								break;

							case 18:
								tempResule.IO_bit.IO_18 = tempswap[5];
								break;

							case 19:
								tempResule.IO_bit.IO_19 = tempswap[4];
								break;

							case 20:
								tempResule.IO_bit.IO_20 = tempswap[3];
								break;

							case 21:
								tempResule.IO_bit.IO_21 = tempswap[2];
								break;

							case 22:
								tempResule.IO_bit.IO_22 = tempswap[1];
								break;

							case 23:
								tempResule.IO_bit.IO_23 = tempswap[0];
								break;
							}
						}
						temp32.m_Long = tempResule.m_Long; //赋值
					}
					temp32.m_Long = ntohl(temp32.m_Long);
					temp32.m_Long &= (0xffffffff >> (32 - (param->start_bit + 1)));
					paramValue->v.rawvalue = (temp32.m_Long >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
					break;
				}
			case 4: //4个
				{
					BIT32_UNION temp32;
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						memcpy(&temp32.m_Bytes[0], &msg[param->start_byte - 1], 4);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						memcpy(&temp32.m_Bytes[0], &msg[param->start_byte + 2], 1);
						memcpy(&temp32.m_Bytes[1], &msg[param->start_byte + 1], 1);
						memcpy(&temp32.m_Bytes[2], &msg[param->start_byte], 1);
						memcpy(&temp32.m_Bytes[3], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp32.m_Bytes[0], &msg[param->start_byte - 1], 4);
						unsigned char tempswap[32]; //用来交换数据临时变量
						BIT32_UNION_DETAIL tempResule; //交换，存储缓存
						unsigned int s1 = 0;
						int k = 0;
						for (k = param->end_bit; k <= param->start_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp32.m_Long) >> k; //获取原始位值右移K
						}
						temp32.m_Long = 0;
						int i = 0;
						for (i = 0; i < 32; i++)
						{
							switch (i)
							{
							case 0:
								tempResule.IO_bit.IO_0 = tempswap[31];
								break;

							case 1:
								tempResule.IO_bit.IO_1 = tempswap[30];
								break;

							case 2:
								tempResule.IO_bit.IO_2 = tempswap[29];
								break;

							case 3:
								tempResule.IO_bit.IO_3 = tempswap[28];
								break;

							case 4:
								tempResule.IO_bit.IO_4 = tempswap[27];
								break;

							case 5:
								tempResule.IO_bit.IO_5 = tempswap[26];
								break;

							case 6:
								tempResule.IO_bit.IO_6 = tempswap[25];
								break;

							case 7:
								tempResule.IO_bit.IO_7 = tempswap[24];
								break;

							case 8:
								tempResule.IO_bit.IO_8 = tempswap[23];
								break;

							case 9:
								tempResule.IO_bit.IO_9 = tempswap[22];
								break;

							case 10:
								tempResule.IO_bit.IO_10 = tempswap[21];
								break;

							case 11:
								tempResule.IO_bit.IO_11 = tempswap[20];
								break;

							case 12:
								tempResule.IO_bit.IO_12 = tempswap[19];
								break;

							case 13:
								tempResule.IO_bit.IO_13 = tempswap[18];
								break;

							case 14:
								tempResule.IO_bit.IO_14 = tempswap[17];
								break;

							case 15:
								tempResule.IO_bit.IO_15 = tempswap[16];
								break;

							case 16:
								tempResule.IO_bit.IO_16 = tempswap[15];
								break;

							case 17:
								tempResule.IO_bit.IO_17 = tempswap[14];
								break;

							case 18:
								tempResule.IO_bit.IO_18 = tempswap[13];
								break;

							case 19:
								tempResule.IO_bit.IO_19 = tempswap[12];
								break;

							case 20:
								tempResule.IO_bit.IO_20 = tempswap[11];
								break;

							case 21:
								tempResule.IO_bit.IO_21 = tempswap[10];
								break;

							case 22:
								tempResule.IO_bit.IO_22 = tempswap[9];
								break;

							case 23:
								tempResule.IO_bit.IO_23 = tempswap[8];
								break;

							case 24:
								tempResule.IO_bit.IO_24 = tempswap[7];
								break;

							case 25:
								tempResule.IO_bit.IO_25 = tempswap[6];
								break;

							case 26:
								tempResule.IO_bit.IO_26 = tempswap[5];
								break;

							case 27:
								tempResule.IO_bit.IO_27 = tempswap[4];
								break;

							case 28:
								tempResule.IO_bit.IO_28 = tempswap[3];
								break;

							case 29:
								tempResule.IO_bit.IO_29 = tempswap[2];
								break;

							case 30:
								tempResule.IO_bit.IO_30 = tempswap[1];
								break;

							case 31:
								tempResule.IO_bit.IO_31 = tempswap[0];
								break;
							}
						}
						temp32.m_Long = tempResule.m_Long; //赋值
					}
					if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001)))
						temp32.m_Long = temp32.m_Long;
					else
						temp32.m_Long = ntohl(temp32.m_Long);
					temp32.m_Long &= (0xffffffff >> (32 - (param->start_bit + 1)));
					paramValue->v.rawvalue = (temp32.m_Long >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
					break;
				}
			case 5:
				{
					BIT64_UNION temp64;
					memset(&temp64.m_Bytes[0], 0, 3);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						memcpy(&temp64.m_Bytes[3], &msg[param->start_byte - 1], 5);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						memcpy(&temp64.m_Bytes[3], &msg[param->start_byte + 3], 1);
						memcpy(&temp64.m_Bytes[4], &msg[param->start_byte + 2], 1);
						memcpy(&temp64.m_Bytes[5], &msg[param->start_byte + 1], 1);
						memcpy(&temp64.m_Bytes[6], &msg[param->start_byte], 1);

						memcpy(&temp64.m_Bytes[7], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp64.m_Bytes[3], &msg[param->start_byte - 1], 5);
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
					}
					temp64.m_uInt64 = /*ntohll*/( temp64.m_uInt64 );
					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit + 1)));
					paramValue->v.rawvalue = (temp64.m_uInt64 >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
					break;
				}
			case 6:
				{
					BIT64_UNION temp64;
					memset(&temp64.m_Bytes[0], 0, 2);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						memcpy(&temp64.m_Bytes[2], &msg[param->start_byte - 1], 6);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						memcpy(&temp64.m_Bytes[2], &msg[param->start_byte + 4], 1);
						memcpy(&temp64.m_Bytes[3], &msg[param->start_byte + 3], 1);
						memcpy(&temp64.m_Bytes[4], &msg[param->start_byte + 2], 1);
						memcpy(&temp64.m_Bytes[5], &msg[param->start_byte + 1], 1);
						memcpy(&temp64.m_Bytes[6], &msg[param->start_byte], 1);
						memcpy(&temp64.m_Bytes[7], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp64.m_Bytes[2], &msg[param->start_byte - 1], 6);
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
					}
					temp64.m_uInt64 = /*ntohll*/( temp64.m_uInt64 );
					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit + 1)));
					paramValue->v.rawvalue = (temp64.m_uInt64 >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
					break;
				}
			case 7:
				{
					BIT64_UNION temp64;
					memset(&temp64.m_Bytes[0], 0, 1);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						memcpy(&temp64.m_Bytes[1], &msg[param->start_byte - 1], 7);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						memcpy(&temp64.m_Bytes[1], &msg[param->start_byte + 5], 1);
						memcpy(&temp64.m_Bytes[2], &msg[param->start_byte + 4], 1);
						memcpy(&temp64.m_Bytes[3], &msg[param->start_byte + 3], 1);
						memcpy(&temp64.m_Bytes[4], &msg[param->start_byte + 2], 1);
						memcpy(&temp64.m_Bytes[5], &msg[param->start_byte + 1], 1);
						memcpy(&temp64.m_Bytes[6], &msg[param->start_byte], 1);
						memcpy(&temp64.m_Bytes[7], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp64.m_Bytes[1], &msg[param->start_byte - 1], 7);
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
					}
					temp64.m_uInt64 = /*ntohll*/( temp64.m_uInt64 );
					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit + 1)));
					paramValue->v.rawvalue = (temp64.m_uInt64 >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
					break;
				}
			case 8:
				{
					BIT64_UNION temp64;
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						memcpy(&temp64.m_Bytes[0], &msg[param->start_byte - 1], 8);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						memcpy(&temp64.m_Bytes[0], &msg[param->start_byte + 6], 1);
						memcpy(&temp64.m_Bytes[1], &msg[param->start_byte + 5], 1);
						memcpy(&temp64.m_Bytes[2], &msg[param->start_byte + 4], 1);
						memcpy(&temp64.m_Bytes[3], &msg[param->start_byte + 3], 1);
						memcpy(&temp64.m_Bytes[4], &msg[param->start_byte + 2], 1);
						memcpy(&temp64.m_Bytes[5], &msg[param->start_byte + 1], 1);
						memcpy(&temp64.m_Bytes[6], &msg[param->start_byte], 1);
						memcpy(&temp64.m_Bytes[7], &msg[param->start_byte - 1], 1);
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						memcpy(&temp64.m_Bytes[0], &msg[param->start_byte - 1], 8);
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
					}

					if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001)))
						temp64.m_uInt64 = temp64.m_uInt64 ;
					else
						if (SHOWTYPE_DOUBLE_ID!=param->display_type)
							Int64Chang((unsigned char *)& temp64.m_uInt64 );



					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit + 1)));
					paramValue->v.rawvalue = (temp64.m_uInt64 >> param->end_bit);
					memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
					break;
				}
			default:
				break;
			}
		}
		else //字节个数大于8时为数组类型，用于内存下泄参数
		{
			memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
		}

		//解码并赋值
		switch (param->data_type)
		{
		case 0: //原码
			{
				if (param->sign_byte != -1) //////////////////如果有符号位占位字节
				{
					u_int8_t byte = msg[param->sign_byte - 1]; //取出当前字节
					byte &= (1 << param->sign_bit); //获得值

					if (byte != 0) //如果等于零不做处理，如果不等于零取反
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.rawvalue *= (-1);
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.rawvalue *= (-1);
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节
						{
							paramValue->v.rawvalue *= (-1);
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.rawvalue *= (-1);
						}
					}
				}

				if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001))) //如果有当量
				{
					//有当量且当量不为1的原码类型，其显示类型一定为float或double
					if (param->display_type == SHOWTYPE_DOUBLE_ID) //double
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.doublevalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.doublevalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节
						{
							paramValue->v.doublevalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.doublevalue = paramValue->v.rawvalue * param->equivalent;
						}
					}
					else if (param->display_type == SHOWTYPE_FLOAT_ID) //float
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
					}
					else
					{
						printf("---------------------------------------------------------------\n");
						printf("解码表中参数%s的显示类型填写不正确！因为此参数有当量且当量不为1,其显示类型应填写DOUBLE或FLOAT!\n", param->paramID);
						printf("---------------------------------------------------------------\n");
					}
				}
				else
				{
					if (param->display_type == SHOWTYPE_DOUBLE_ID) //double
					{
						paramValue->v.doublevalue = (double)paramValue->v.doublevalue;
					}
					else if (param->display_type == SHOWTYPE_FLOAT_ID) //float
					{
						paramValue->v.rawvalue=	htonl(paramValue->v.rawvalue);
						paramValue->v.floatvalue = (float)paramValue->v.floatvalue ;
					}
					//else if (param->display_type == SHOWTYPE_FLOAT16_ID) //float
					//{
					//	paramValue->rawbytes[4]=paramValue->rawbytes[1];
					//	paramValue->rawbytes[3]=paramValue->rawbytes[0];
					//	paramValue->rawbytes[0]=0;
					//	paramValue->rawbytes[1]=0;
					//}
				}

				//把参数名称赋给参数的汉字注释
				//strcpy(param->zs, param->paramName);//（20150113修改
				if (param->annotation_times > 0) //如果有汉字注释
				{
					unsigned int ic;
					long temp_HzRaw = 0;
					char temp_char[20] = "0x";

					for (ic = 0; ic < param->annotation_times; ic++)
					{
						//取出注释的原码部分并转成16进制数，赋给temp_HzRaw
						strcat(temp_char, &param->annotation_raw[ic][0]);
						sscanf(temp_char, "%lx", &temp_HzRaw);

						//与当前的解码值进行比较
						//应保证解码表中注释的格式填写正确无误,即16进制原码:汉字的形式,如2A00:在轨段_半自动姿态控制_全姿态捕获_速率阻尼
						if (paramValue->v.rawvalue == temp_HzRaw)
						{
							memset(param->zs, 0, sizeof(param->zs));						//清理一下
							sprintf(param->zs, "0x%lX:", paramValue->v.rawvalue);
							strcat(param->zs, &param->annotation_str[ic][0]);
							break;
						}
						else//若没有注释或注释中的原码部分不等于当前收到的数据，则zs为收到的原码值（20150113修改，原来为参数名称）
						{
							sprintf(param->zs, "0x%lX", paramValue->v.rawvalue);
						}
						strcpy(temp_char, "0x");
					}
				}



			}
			break;

		case 1: //补码
			{
				int HighBit = 0;
				if (param->byte_count == 1) //1字节
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						unsigned long yiwei = ~paramValue->v.rawvalue + 1;
						unsigned long pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 32)
						{
							pingbi = 0xffffffff;
						}
						paramValue->v.rawvalue = (yiwei & pingbi);
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}
				else if (param->byte_count == 2) //2字节
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						unsigned long yiwei = ~paramValue->v.rawvalue + 1;
						unsigned long pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 32)
						{
							pingbi = 0xffffffff;
						}
						paramValue->v.rawvalue = yiwei & pingbi;
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}
				else if ((param->byte_count <= 4) && (param->byte_count > 2)) //3,4字节
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						unsigned long yiwei = ~paramValue->v.rawvalue + 1;
						unsigned long pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 32)
						{
							pingbi = 0xffffffff;
						}
						paramValue->v.rawvalue = yiwei & pingbi;
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}
				else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						unsigned long yiwei = ~paramValue->v.rawvalue + 1;
						unsigned long pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 64)
						{
							pingbi = 0xffffffffffffffff;
						}
						paramValue->v.rawvalue = yiwei & pingbi;
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}

				if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001))) //如果有当量
				{
					//有当量且当量不为1的补码类型，其显示类型一定为float或double
					if (param->display_type == SHOWTYPE_DOUBLE_ID) //double
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.doublevalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.doublevalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于4字节
						{
							paramValue->v.doublevalue = (double)(paramValue->v.rawvalue * param->equivalent);
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.doublevalue = paramValue->v.rawvalue * param->equivalent;
						}
					}
					else if (param->display_type == SHOWTYPE_FLOAT_ID) //float
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于4字节
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.floatvalue = paramValue->v.rawvalue * param->equivalent;
						}
					}
				}
				else
				{
					if (param->display_type == SHOWTYPE_DOUBLE_ID) //double
					{
						paramValue->v.doublevalue = (double)paramValue->v.rawvalue;
					}
					else if (param->display_type == SHOWTYPE_FLOAT_ID) //float
					{
						paramValue->v.floatvalue = (float)paramValue->v.rawvalue;
					}
				}




				if (param->annotation_times > 0) //如果有汉字注释
				{
					int ic;
					long temp_HzRaw = 0;
					char temp_char[20] = "0x";

					for (ic = 0; ic < param->annotation_times; ic++)
					{
						strcat(temp_char, &param->annotation_raw[ic][0]);
						sscanf(temp_char, "%lx", &temp_HzRaw); //取出注释的原码部分并转成16进制数，赋给temp_HzRaw
						//与当前的解码值进行比较
						/*用户应保证解码表中注释的格式填写正确无误,即16进制原码:汉字的形式,如2A00:在轨段_半自动姿态控制_全姿态捕获_速率阻尼*/
						if (paramValue->v.rawvalue == temp_HzRaw)
						{
							memset(param->zs, 0, sizeof(param->zs));
							sprintf(param->zs, "0x%lX:", paramValue->v.rawvalue);
							strcat(param->zs, &param->annotation_str[ic][0]);
							break;
						}
						else//若没有注释或注释中的原码部分不等于当前收到的数据，则zs为收到的原码值（20150113修改，原来为参数名称）
						{
							sprintf(param->zs, "0x%lX", paramValue->v.rawvalue);
						}
						strcpy(temp_char, "0x");
					}
				}
			}
			break;

		case 2:	//浮点
			{
				if (param->byte_count == 1) //1字节
				{
					u_int8_t tempfloatvalue = (u_int8_t) paramValue->v.rawvalue;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					memcpy(&paramValue->v.floatvalue, &tempfloatvalue, param->byte_count);
				}
				else if (param->byte_count == 2) //2字节
				{
					u_int16_t tempfloatvalue = (u_int16_t) paramValue->v.rawvalue;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					memcpy(&paramValue->v.floatvalue, &tempfloatvalue, param->byte_count);
				}
				else if ((param->display_type == 2) && (param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节，且显示类型填的是FLOAT
				{
					u_int32_t tempfloatvalue = (u_int32_t) paramValue->v.rawvalue;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					memcpy(&paramValue->v.floatvalue, &tempfloatvalue, param->byte_count);
				}
				else if ((param->display_type == 1) && (param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节，但显示类型填的是DOUBLE
				{
					u_int32_t tempfloatvalue = (u_int32_t) paramValue->v.rawvalue;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					memcpy(&paramValue->v.doublevalue, &tempfloatvalue, param->byte_count);
				}
				else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
				{
					u_int64_t tempdoublevalue = (u_int64_t) paramValue->v.rawvalue;
					tempdoublevalue &= (0xffffffffffffffff << (64 - (param->start_bit - param->end_bit + 1)));
					memcpy(&paramValue->v.doublevalue, &tempdoublevalue, param->byte_count);
				}

				if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001)))
				{
					if (param->byte_count <= 4) //小于4字节
					{
						paramValue->v.floatvalue = paramValue->v.floatvalue * param->equivalent;
					}
					else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
					{
						paramValue->v.doublevalue = paramValue->v.doublevalue * param->equivalent;
					}
				}
			}
			break;

		default:
			break;
		}
		string parmname=handle->msg_descriptor->name;
		parmname+=".";
		parmname+=param->paramName;
		//		redis.AddInt((char *)parmname.c_str(),paramValue->v.rawvalue);

	}

	return 0;
}


/******************************************************************
*函数功能:4字节-8字节位反序。
*输入参数:
*输出参数:
*返回值  :none
*******************************************************************/
void Decode::reverse_BitOrder(BIT64_UNION *val, u_int8_t bitstart, u_int8_t bistop)
{
	unsigned char tempswap[64]; //用来交换数据临时变量
	BIT64_UNION_DETAIL tempResule; //交换，存储缓存
	uint64_t s1 = 0;
	int k = 0;
	for (k = bitstart; k <= bistop; k++)
	{
		s1 = 1 << k;
		tempswap[k] = (s1 & val->m_uInt64) >> k; //获取原始位值右移K
	}
	val->m_uInt64 = 0;
	int i = 0;
	for (i = 0; i < 64; i++)
	{
		switch (i)
		{
		case 0:
			tempResule.IO_bit.IO_0 = tempswap[63];
			break;

		case 1:
			tempResule.IO_bit.IO_1 = tempswap[62];
			break;

		case 2:
			tempResule.IO_bit.IO_2 = tempswap[61];
			break;

		case 3:
			tempResule.IO_bit.IO_3 = tempswap[60];
			break;

		case 4:
			tempResule.IO_bit.IO_4 = tempswap[59];
			break;

		case 5:
			tempResule.IO_bit.IO_5 = tempswap[58];
			break;

		case 6:
			tempResule.IO_bit.IO_6 = tempswap[57];
			break;

		case 7:
			tempResule.IO_bit.IO_7 = tempswap[56];
			break;

		case 8:
			tempResule.IO_bit.IO_8 = tempswap[55];
			break;

		case 9:
			tempResule.IO_bit.IO_9 = tempswap[54];
			break;

		case 10:
			tempResule.IO_bit.IO_10 = tempswap[53];
			break;

		case 11:
			tempResule.IO_bit.IO_11 = tempswap[52];
			break;

		case 12:
			tempResule.IO_bit.IO_12 = tempswap[51];
			break;

		case 13:
			tempResule.IO_bit.IO_13 = tempswap[50];
			break;

		case 14:
			tempResule.IO_bit.IO_14 = tempswap[49];
			break;

		case 15:
			tempResule.IO_bit.IO_15 = tempswap[48];
			break;

		case 16:
			tempResule.IO_bit.IO_16 = tempswap[47];
			break;

		case 17:
			tempResule.IO_bit.IO_17 = tempswap[46];
			break;

		case 18:
			tempResule.IO_bit.IO_18 = tempswap[45];
			break;

		case 19:
			tempResule.IO_bit.IO_19 = tempswap[44];
			break;

		case 20:
			tempResule.IO_bit.IO_20 = tempswap[43];
			break;

		case 21:
			tempResule.IO_bit.IO_21 = tempswap[42];
			break;

		case 22:
			tempResule.IO_bit.IO_22 = tempswap[41];
			break;

		case 23:
			tempResule.IO_bit.IO_23 = tempswap[40];
			break;

		case 24:
			tempResule.IO_bit.IO_24 = tempswap[39];
			break;

		case 25:
			tempResule.IO_bit.IO_25 = tempswap[38];
			break;

		case 26:
			tempResule.IO_bit.IO_26 = tempswap[37];
			break;

		case 27:
			tempResule.IO_bit.IO_27 = tempswap[36];
			break;

		case 28:
			tempResule.IO_bit.IO_28 = tempswap[35];
			break;

		case 29:
			tempResule.IO_bit.IO_29 = tempswap[34];
			break;

		case 30:
			tempResule.IO_bit.IO_30 = tempswap[33];
			break;

		case 31:
			tempResule.IO_bit.IO_31 = tempswap[32];
			break;

		case 32:
			tempResule.IO_bit.IO_32 = tempswap[31];
			break;

		case 33:
			tempResule.IO_bit.IO_33 = tempswap[30];
			break;

		case 34:
			tempResule.IO_bit.IO_34 = tempswap[29];
			break;

		case 35:
			tempResule.IO_bit.IO_35 = tempswap[28];
			break;

		case 36:
			tempResule.IO_bit.IO_36 = tempswap[27];
			break;

		case 37:
			tempResule.IO_bit.IO_37 = tempswap[26];
			break;

		case 38:
			tempResule.IO_bit.IO_38 = tempswap[25];
			break;

		case 39:
			tempResule.IO_bit.IO_39 = tempswap[24];
			break;

		case 40:
			tempResule.IO_bit.IO_40 = tempswap[23];
			break;

		case 41:
			tempResule.IO_bit.IO_41 = tempswap[22];
			break;

		case 42:
			tempResule.IO_bit.IO_42 = tempswap[21];
			break;

		case 43:
			tempResule.IO_bit.IO_43 = tempswap[20];
			break;

		case 44:
			tempResule.IO_bit.IO_44 = tempswap[19];
			break;

		case 45:
			tempResule.IO_bit.IO_45 = tempswap[18];
			break;

		case 46:
			tempResule.IO_bit.IO_46 = tempswap[17];
			break;

		case 47:
			tempResule.IO_bit.IO_47 = tempswap[16];
			break;

		case 48:
			tempResule.IO_bit.IO_48 = tempswap[15];
			break;

		case 49:
			tempResule.IO_bit.IO_49 = tempswap[14];
			break;

		case 50:
			tempResule.IO_bit.IO_50 = tempswap[13];
			break;

		case 51:
			tempResule.IO_bit.IO_51 = tempswap[12];
			break;

		case 52:
			tempResule.IO_bit.IO_52 = tempswap[11];
			break;

		case 53:
			tempResule.IO_bit.IO_53 = tempswap[10];
			break;

		case 54:
			tempResule.IO_bit.IO_54 = tempswap[9];
			break;

		case 55:
			tempResule.IO_bit.IO_55 = tempswap[8];
			break;

		case 56:
			tempResule.IO_bit.IO_56 = tempswap[7];
			break;

		case 57:
			tempResule.IO_bit.IO_57 = tempswap[6];
			break;

		case 58:
			tempResule.IO_bit.IO_58 = tempswap[5];
			break;

		case 59:
			tempResule.IO_bit.IO_59 = tempswap[4];
			break;

		case 60:
			tempResule.IO_bit.IO_60 = tempswap[3];
			break;

		case 61:
			tempResule.IO_bit.IO_61 = tempswap[2];
			break;

		case 62:
			tempResule.IO_bit.IO_62 = tempswap[1];
			break;

		case 63:
			tempResule.IO_bit.IO_63 = tempswap[0];
			break;
		}
	}

	val->m_uInt64 = tempResule.m_uInt64; //赋值
}

/******************************************************************
*函数功能:合成源码流程。
*输入参数:
*输出参数:
*返回值  :
*******************************************************************/
int Decode::recode_msg(Message* handle, u_int8_t *msg)
{
	if (handle==0)
	{
		return -1;
	}
	int index = 0;
	unsigned int start_byte;
	MessageDescriptor *msgDescriptor = handle->msg_descriptor;
	unsigned int paramCount = msgDescriptor->param_cnt;
	ParameterDescriptor *param = NULL;
	ParameterValue parmvalue;
	ParameterValue *paramValue=NULL;
	paramValue=&parmvalue;

	for (index = 0; index < paramCount; index++)
	{

		memcpy(paramValue , &handle->param_value[index],sizeof(ParameterValue));

		param = &msgDescriptor->parameters[index];

		BitReset(param,paramValue,msg);
		start_byte = param->start_byte;	

		//增加对起始字节填写错误的判断
		if(start_byte < 1)
		{
			fprintf(pLogFile,"%s:参数%s的起始字节错误,服务器程序已退出.\n",UtilTool::GetTimeString().c_str(), param->paramID);
			//exit(EXIT_FAILURE);
		}
		if (param->start_bit==0&&param->end_bit==0)
		{
			continue;
		}
		//解码并赋值
		switch (param->data_type)
		{
		case 0: //原码
			{		
				if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001))) //如果有当量
				{
					//有当量且当量不为1的原码类型，其显示类型一定为float或double
					if (param->display_type == SHOWTYPE_DOUBLE_ID) //double
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
					}
					else if (param->display_type == SHOWTYPE_FLOAT_ID) //float
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
					}
					else
					{
						printf("---------------------------------------------------------------\n");
						printf("解码表中参数%s的显示类型填写不正确！因为此参数有当量且当量不为1,其显示类型应填写DOUBLE或FLOAT!\n", param->paramID);
						printf("---------------------------------------------------------------\n");
					}
					if (param->sign_byte != -1) //////////////////如果有符号位占位字节
					{
						u_int8_t byte = 0; //取出当前字节
						if (paramValue->v.rawvalue<0)
						{
							msg[param->sign_byte - 1] |= (1 << param->sign_bit); //获得值
						}else
							msg[param->sign_byte - 1] &= (0 << param->sign_bit); //获得值
						//
					}
				}	

				if (param->display_type == SHOWTYPE_ARRAY_ID) //double
				{
					memcpy(	&msg[param->start_byte - 1],paramValue->rawbytes,param->byte_count);
					continue;
				}

			}
			break;

		case 1: //补码
			{

				if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001))) //如果有当量
				{
					//有当量且当量不为1的补码类型，其显示类型一定为float或double
					if (param->display_type == SHOWTYPE_DOUBLE_ID) //double
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于4字节
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.rawvalue =paramValue->v.doublevalue / param->equivalent;
						}
					}
					else if (param->display_type == SHOWTYPE_FLOAT_ID) //float
					{
						if (param->byte_count == 1) //1字节
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
						else if (param->byte_count == 2) //2字节
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
						else if ((param->byte_count <= 4) && (param->byte_count > 2)) //小于4字节
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
						else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
						{
							paramValue->v.rawvalue =paramValue->v.floatvalue / param->equivalent;
						}
					}
				}else
				{
					if (param->display_type == SHOWTYPE_DOUBLE_ID) //double
					{
						paramValue->v.rawvalue=paramValue->v.doublevalue;
					}
					else if (param->display_type == SHOWTYPE_FLOAT_ID) //float
					{
						paramValue->v.rawvalue= paramValue->v.floatvalue;
					}
				}

				int HighBit = 0;
				if (param->byte_count == 1) //1字节
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						unsigned long yiwei = ~(paramValue->v.rawvalue - 1);
						unsigned long pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 32)
						{
							pingbi = 0xffffffff;
						}
						paramValue->v.rawvalue = (yiwei & pingbi);
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}
				else if (param->byte_count == 2) //2字节
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						unsigned long yiwei = ~(paramValue->v.rawvalue - 1);
						unsigned long pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 32)
						{
							pingbi = 0xffffffff;
						}
						paramValue->v.rawvalue = yiwei & pingbi;
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}
				else if ((param->byte_count <= 4) && (param->byte_count > 2)) //3,4字节
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						unsigned long yiwei = ~(paramValue->v.rawvalue - 1);
						unsigned long pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 32)
						{
							pingbi = 0xffffffff;
						}
						paramValue->v.rawvalue = yiwei & pingbi;
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}
				else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
				{
					HighBit = paramValue->v.rawvalue >> (param->bit_length - 1);
					if (HighBit != 0) //
					{
						u_int64_t yiwei = ~(paramValue->v.rawvalue - 1);
						u_int64_t pingbi = (1 << param->bit_length) - 1; //向左移动32位越界，类型转换后还是为1;
						if (param->bit_length == 64)
						{
							pingbi = 0xffffffffffffffff;
						}
						paramValue->v.rawvalue = yiwei & pingbi;
						paramValue->v.rawvalue = paramValue->v.rawvalue - 2 * paramValue->v.rawvalue;
					}
				}

			}
			break;

		case 2:	//浮点
			{		
				if (!((param->equivalent > -0.000000000001) && (param->equivalent < 0.000000000001)))
				{
					if (param->byte_count <= 4) //小于4字节
					{
						paramValue->v.floatvalue = paramValue->v.floatvalue / param->equivalent;
					}
					else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
					{
						paramValue->v.floatvalue = paramValue->v.floatvalue / param->equivalent;
					}
				}

				if (param->byte_count == 1) //1字节
				{
					u_int8_t tempfloatvalue=0;
					memcpy( &tempfloatvalue, &paramValue->v.floatvalue,param->byte_count);;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));			
					paramValue->v.rawvalue=tempfloatvalue;

					//u_int8_t tempfloatvalue = (u_int8_t) paramValue->v.rawvalue;
					//tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					//memcpy(&paramValue->v.floatvalue, &tempfloatvalue, param->byte_count);
				}
				else if (param->byte_count == 2) //2字节
				{
					u_int16_t tempfloatvalue=0;
					memcpy( &tempfloatvalue, &paramValue->v.floatvalue,param->byte_count);;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));			
					paramValue->v.rawvalue=tempfloatvalue;

					//u_int16_t tempfloatvalue = (u_int16_t) paramValue->v.rawvalue;
					//tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					//memcpy(&paramValue->v.floatvalue, &tempfloatvalue, param->byte_count);
				}
				else if ((param->display_type == 2) && (param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节，且显示类型填的是FLOAT
				{
					u_int32_t tempfloatvalue=0;
					memcpy( &tempfloatvalue, &paramValue->v.floatvalue,param->byte_count);;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));			
					paramValue->v.rawvalue=tempfloatvalue;

					//u_int32_t tempfloatvalue = (u_int32_t) paramValue->v.rawvalue;
					//tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					//memcpy(&paramValue->v.floatvalue, &tempfloatvalue, param->byte_count);
				}
				else if ((param->display_type == 1) && (param->byte_count <= 4) && (param->byte_count > 2)) //小于等于4字节，但显示类型填的是DOUBLE
				{
					u_int32_t tempfloatvalue=0;
					memcpy( &tempfloatvalue, &paramValue->v.floatvalue,param->byte_count);;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));			
					paramValue->v.rawvalue=tempfloatvalue;

					//u_int32_t tempfloatvalue = (u_int32_t) paramValue->v.rawvalue;
					//tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));
					//memcpy(&paramValue->v.doublevalue, &tempfloatvalue, param->byte_count);
				}
				else if ((param->byte_count <= 8) && (param->byte_count > 4)) //5-8字节补码
				{
					u_int64_t tempfloatvalue=0;
					memcpy( &tempfloatvalue, &paramValue->v.floatvalue,param->byte_count);;
					tempfloatvalue &= (0xffffffff << (32 - (param->start_bit - param->end_bit + 1)));			
					paramValue->v.rawvalue=tempfloatvalue;

					//u_int64_t tempdoublevalue = (u_int64_t) paramValue->v.rawvalue;
					//tempdoublevalue &= (0xffffffffffffffff << (64 - (param->start_bit - param->end_bit + 1)));
					//memcpy(&paramValue->v.doublevalue, &tempdoublevalue, param->byte_count);
				}
			}
			break;

		default:
			break;
		}




		//根据字节个数处理
		if ((param->byte_count <= 8) && (param->byte_count >= 1))
		{
			switch (param->byte_count)
			{
			case 1:
				{
					BIT8_UNION temp8;
					temp8.m_uInt8  =( unsigned char )(paramValue->v.rawvalue);
					temp8.m_uInt8 &= (0xff >> (8 - (param->start_bit-param->end_bit + 1))); //BITEND为7
					temp8.m_uInt8 = (temp8.m_uInt8 << param->end_bit);
					//	memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);


					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST) //默认高字节在前
					{
						msg[param->start_byte - 1]|=temp8.m_Bytes[0];
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						unsigned char tempswap[8]; //用来交换数据临时变量
						BIT8_UNION_DETAIL tempResult; //交换，存储缓存
						BYTE s1 = 0;
						int k = 0;
						for (k = param->start_bit; k <= param->end_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp8.m_Bytes[0]) >> k; //获取原始位值右移K
						}
						temp8.m_Bytes[0] = 0;
						int i = 0;
						for (i = 0; i < 8; i++)
						{
							switch (i)
							{
							case 0:
								tempResult.IO_bit.IO_0 = tempswap[7];
								break;

							case 1:
								tempResult.IO_bit.IO_1 = tempswap[6];
								break;

							case 2:
								tempResult.IO_bit.IO_2 = tempswap[5];
								break;

							case 3:
								tempResult.IO_bit.IO_3 = tempswap[4];
								break;

							case 4:
								tempResult.IO_bit.IO_4 = tempswap[3];
								break;

							case 5:
								tempResult.IO_bit.IO_5 = tempswap[2];
								break;

							case 6:
								tempResult.IO_bit.IO_6 = tempswap[1];
								break;

							case 7:
								tempResult.IO_bit.IO_7 = tempswap[0];
								break;
							}
						}
						temp8.m_uInt8 = tempResult.m_Bytes;
						msg[param->start_byte - 1]|=temp8.m_Bytes[0];
					}
					else //如果写成低字节在前，使用默认方法解析，因为就1个字节
					{
						msg[param->start_byte - 1] |=temp8.m_uInt8;
					}
				}
				break;

			case 2:
				{
					BIT16_UNION temp16;
					temp16.m_Word  = (paramValue->v.rawvalue);
					temp16.m_Word &= (0xffff >> (16 - (param->start_bit-param->end_bit + 1)));
					temp16.m_Word  = (temp16.m_Word << param->end_bit);
					if (param->display_type == SHOWTYPE_FLOAT16_ID)
						temp16.m_Word  = temp16.m_Word;
					else
						temp16.m_Word  = ntohs(temp16.m_Word);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST) //高字节在前
					{
						for (int i=0;i<2;i++)
						{
							msg[param->start_byte - 1+i]|=temp16.m_Bytes[0+i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						for (int i=0;i<2;i++)
						{
							msg[param->start_byte-i]|=temp16.m_Bytes[0+i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						unsigned char tempswap[16]; //用来交换数据临时变量
						BIT16_UNION_DETAIL tempResule; //交换，存储缓存
						unsigned short s1 = 0;
						int k = 0;
						for (k = param->end_bit; k <= param->start_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp16.m_Word) >> k; //获取原始位值右移K
						}
						temp16.m_Word = 0;
						int i = 0;
						for (i = 0; i < 16; i++)
						{
							switch (i)
							{
							case 0:
								tempResule.IO_bit.IO_0 = tempswap[15];
								break;

							case 1:
								tempResule.IO_bit.IO_1 = tempswap[14];
								break;

							case 2:
								tempResule.IO_bit.IO_2 = tempswap[13];
								break;

							case 3:
								tempResule.IO_bit.IO_3 = tempswap[12];
								break;

							case 4:
								tempResule.IO_bit.IO_4 = tempswap[11];
								break;

							case 5:
								tempResule.IO_bit.IO_5 = tempswap[10];
								break;

							case 6:
								tempResule.IO_bit.IO_6 = tempswap[9];
								break;

							case 7:
								tempResule.IO_bit.IO_7 = tempswap[8];
								break;

							case 8:
								tempResule.IO_bit.IO_8 = tempswap[7];
								break;

							case 9:
								tempResule.IO_bit.IO_9 = tempswap[6];
								break;

							case 10:
								tempResule.IO_bit.IO_10 = tempswap[5];
								break;

							case 11:
								tempResule.IO_bit.IO_11 = tempswap[4];
								break;

							case 12:
								tempResule.IO_bit.IO_12 = tempswap[3];
								break;

							case 13:
								tempResule.IO_bit.IO_13 = tempswap[2];
								break;

							case 14:
								tempResule.IO_bit.IO_14 = tempswap[1];
								break;

							case 15:
								tempResule.IO_bit.IO_15 = tempswap[0];
								break;
							}
						}
						temp16.m_Word = tempResule.m_Word; //赋值
						for (int i=0;i<2;i++)
						{
							msg[param->start_byte - 1+i]|=temp16.m_Bytes[0+i];
						}
					}

				}
				break;
			case 3: //3个
				{
					BIT32_UNION temp32;
					temp32.m_Long  = /*ntohl*/(paramValue->v.rawvalue);
					temp32.m_Long &= (0xffffffff >> (32 - (param->start_bit - param->end_bit+1)));
					temp32.m_Long  = (temp32.m_Long << param->end_bit);
					temp32.m_Long  = ntohl(temp32.m_Long  );
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST) //高字节在前
					{	
						for (int i=0;i<3;i++)
							msg[param->start_byte - 1+i]|=temp32.m_Bytes[1+i];
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						for (int i=0;i<3;i++)
							msg[param->start_byte + 1-i]|=temp32.m_Bytes[1+i];
					}

					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						unsigned char tempswap[24]; //用来交换数据临时变量
						BIT32_UNION_DETAIL tempResule; //交换，存储缓存
						unsigned int s1 = 0;
						int k = 0;
						for (k = param->end_bit; k <= param->start_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp32.m_Long) >> k; //获取原始位值右移K
						}
						temp32.m_Long = 0;
						int i = 0;
						for (i = 0; i < 24; i++)
						{
							switch (i)
							{
							case 0:
								tempResule.IO_bit.IO_0 = tempswap[23];
								break;

							case 1:
								tempResule.IO_bit.IO_1 = tempswap[22];
								break;

							case 2:
								tempResule.IO_bit.IO_2 = tempswap[21];
								break;

							case 3:
								tempResule.IO_bit.IO_3 = tempswap[20];
								break;

							case 4:
								tempResule.IO_bit.IO_4 = tempswap[19];
								break;

							case 5:
								tempResule.IO_bit.IO_5 = tempswap[18];
								break;

							case 6:
								tempResule.IO_bit.IO_6 = tempswap[17];
								break;

							case 7:
								tempResule.IO_bit.IO_7 = tempswap[16];
								break;

							case 8:
								tempResule.IO_bit.IO_8 = tempswap[15];
								break;

							case 9:
								tempResule.IO_bit.IO_9 = tempswap[14];
								break;

							case 10:
								tempResule.IO_bit.IO_10 = tempswap[13];
								break;

							case 11:
								tempResule.IO_bit.IO_11 = tempswap[12];
								break;

							case 12:
								tempResule.IO_bit.IO_12 = tempswap[11];
								break;

							case 13:
								tempResule.IO_bit.IO_13 = tempswap[10];
								break;

							case 14:
								tempResule.IO_bit.IO_14 = tempswap[9];
								break;

							case 15:
								tempResule.IO_bit.IO_15 = tempswap[8];
								break;

							case 16:
								tempResule.IO_bit.IO_16 = tempswap[7];
								break;

							case 17:
								tempResule.IO_bit.IO_17 = tempswap[6];
								break;

							case 18:
								tempResule.IO_bit.IO_18 = tempswap[5];
								break;

							case 19:
								tempResule.IO_bit.IO_19 = tempswap[4];
								break;

							case 20:
								tempResule.IO_bit.IO_20 = tempswap[3];
								break;

							case 21:
								tempResule.IO_bit.IO_21 = tempswap[2];
								break;

							case 22:
								tempResule.IO_bit.IO_22 = tempswap[1];
								break;

							case 23:
								tempResule.IO_bit.IO_23 = tempswap[0];
								break;
							}
						}
						temp32.m_Long = tempResule.m_Long; //赋值

						if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST) //高字节在前
						{	
							for (int i=0;i<3;i++)
								msg[param->start_byte - 1+i]|=temp32.m_Bytes[1+i];
						}
					}
					break;
				}
			case 4: //4个
				{
					BIT32_UNION temp32;
					temp32.m_Long  = /*ntohl*/(paramValue->v.rawvalue);
					temp32.m_Long &= (0xffffffff >> (32 - (param->start_bit - param->end_bit + 1)));
					temp32.m_Long  = (temp32.m_Long << param->end_bit);
					if (param->display_type==SHOWTYPE_DOUBLE_ID||param->display_type==SHOWTYPE_FLOAT_ID||param->display_type==SHOWTYPE_FLOAT16_ID)
					{	
						temp32.m_Long  =temp32.m_Long ;
					}else {
						temp32.m_Long  =ntohl(temp32.m_Long  );
					}


					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						for (int i=0;i<4;i++)
						{
							msg[param->start_byte - 1+i]|=temp32.m_Bytes[i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						for (int i=0;i<4;i++)
						{
							msg[param->start_byte + 2-i]|=temp32.m_Bytes[i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						unsigned char tempswap[32]; //用来交换数据临时变量
						BIT32_UNION_DETAIL tempResule; //交换，存储缓存
						unsigned int s1 = 0;
						int k = 0;
						for (k = param->end_bit; k <= param->start_bit; k++)
						{
							s1 = 1 << k;
							tempswap[k] = (s1 & temp32.m_Long) >> k; //获取原始位值右移K
						}
						temp32.m_Long = 0;
						int i = 0;
						for (i = 0; i < 32; i++)
						{
							switch (i)
							{
							case 0:
								tempResule.IO_bit.IO_0 = tempswap[31];
								break;

							case 1:
								tempResule.IO_bit.IO_1 = tempswap[30];
								break;

							case 2:
								tempResule.IO_bit.IO_2 = tempswap[29];
								break;

							case 3:
								tempResule.IO_bit.IO_3 = tempswap[28];
								break;

							case 4:
								tempResule.IO_bit.IO_4 = tempswap[27];
								break;

							case 5:
								tempResule.IO_bit.IO_5 = tempswap[26];
								break;

							case 6:
								tempResule.IO_bit.IO_6 = tempswap[25];
								break;

							case 7:
								tempResule.IO_bit.IO_7 = tempswap[24];
								break;

							case 8:
								tempResule.IO_bit.IO_8 = tempswap[23];
								break;

							case 9:
								tempResule.IO_bit.IO_9 = tempswap[22];
								break;

							case 10:
								tempResule.IO_bit.IO_10 = tempswap[21];
								break;

							case 11:
								tempResule.IO_bit.IO_11 = tempswap[20];
								break;

							case 12:
								tempResule.IO_bit.IO_12 = tempswap[19];
								break;

							case 13:
								tempResule.IO_bit.IO_13 = tempswap[18];
								break;

							case 14:
								tempResule.IO_bit.IO_14 = tempswap[17];
								break;

							case 15:
								tempResule.IO_bit.IO_15 = tempswap[16];
								break;

							case 16:
								tempResule.IO_bit.IO_16 = tempswap[15];
								break;

							case 17:
								tempResule.IO_bit.IO_17 = tempswap[14];
								break;

							case 18:
								tempResule.IO_bit.IO_18 = tempswap[13];
								break;

							case 19:
								tempResule.IO_bit.IO_19 = tempswap[12];
								break;

							case 20:
								tempResule.IO_bit.IO_20 = tempswap[11];
								break;

							case 21:
								tempResule.IO_bit.IO_21 = tempswap[10];
								break;

							case 22:
								tempResule.IO_bit.IO_22 = tempswap[9];
								break;

							case 23:
								tempResule.IO_bit.IO_23 = tempswap[8];
								break;

							case 24:
								tempResule.IO_bit.IO_24 = tempswap[7];
								break;

							case 25:
								tempResule.IO_bit.IO_25 = tempswap[6];
								break;

							case 26:
								tempResule.IO_bit.IO_26 = tempswap[5];
								break;

							case 27:
								tempResule.IO_bit.IO_27 = tempswap[4];
								break;

							case 28:
								tempResule.IO_bit.IO_28 = tempswap[3];
								break;

							case 29:
								tempResule.IO_bit.IO_29 = tempswap[2];
								break;

							case 30:
								tempResule.IO_bit.IO_30 = tempswap[1];
								break;

							case 31:
								tempResule.IO_bit.IO_31 = tempswap[0];
								break;
							}
						}
						temp32.m_Long = tempResule.m_Long; //赋值
					}else
						for (int i=0;i<4;i++)
						{
							msg[param->start_byte - 1+i]|=temp32.m_Bytes[i];
						}
						break;
				}
			case 5:
				{
					BIT64_UNION temp64;
					temp64.m_uInt64 = /*ntohl*/( paramValue->v.rawvalue );
					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit-param->end_bit + 1)));
					temp64.m_uInt64  = (temp64.m_uInt64 << param->end_bit);

					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						for (int i=0;i<5;i++)
						{
							msg[param->start_byte - 1+i]|=temp64.m_Bytes[3+i];
						}
					}

					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						for (int i=0;i<5;i++)
						{
							msg[param->start_byte + 3-i]|=temp64.m_Bytes[3+i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
						for (int i=0;i<5;i++)
						{
							msg[param->start_byte - 1+i]|=temp64.m_Bytes[3+i];
						}
					}
					break;
				}
			case 6:
				{
					BIT64_UNION temp64;
					temp64.m_uInt64 = /*ntohl*/( paramValue->v.rawvalue );
					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit-param->end_bit + 1)));
					temp64.m_uInt64  = (temp64.m_uInt64 << param->end_bit);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						for (int i=0;i<6;i++)
						{
							msg[param->start_byte - 1+i]|=temp64.m_Bytes[2+i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						for (int i=0;i<6;i++)
						{
							msg[param->start_byte + 4-i]|=temp64.m_Bytes[2+i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						for (int i=0;i<6;i++)
						{
							msg[param->start_byte - 1+i]|=temp64.m_Bytes[2+i];
						}
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
					}
					break;
				}
			case 7:
				{
					BIT64_UNION temp64;
					temp64.m_uInt64 = /*ntohl*/( paramValue->v.rawvalue );
					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit-param->end_bit + 1)));
					temp64.m_uInt64  = (temp64.m_uInt64 << param->end_bit);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						for (int i=0;i<7;i++)
						{
							msg[param->start_byte - 1+i]|=temp64.m_Bytes[1+i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						for (int i=0;i<7;i++)
						{
							msg[param->start_byte + 5-i]|=temp64.m_Bytes[1+i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
						for (int i=0;i<7;i++)
						{
							msg[param->start_byte - 1+i]|=temp64.m_Bytes[1+i];
						}
					}
					break;
				}
			case 8:
				{
					BIT64_UNION temp64;
					temp64.m_uInt64 =( paramValue->v.rawvalue ); 
					temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit-param->end_bit + 1)));
					temp64.m_uInt64  = (temp64.m_uInt64 << param->end_bit);
					//Int64Chang(temp64.m_Bytes);
					if (param->byte_order == RAW_BYTE_ORDER_HIGH_FIRST)
					{
						for (int i=0;i<8;i++)
						{
							msg[param->start_byte - 1 + i]|=temp64.m_Bytes[i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOW_FIRST) //低字节在前
					{
						for (int i=0;i<8;i++)
						{
							msg[param->start_byte + 6-i]|=temp64.m_Bytes[i];
						}
					}
					else if (param->byte_order == RAW_BYTE_ORDER_LOWBIT_FIRST) //低位在前
					{
						reverse_BitOrder(&temp64, param->end_bit, param->start_bit);
						for (int i=0;i<8;i++)
						{
							msg[param->start_byte - 1+i]|=temp64.m_Bytes[i];
						}
					}
					break;
				}
			default:
				break;
			}
		}
		else //字节个数大于8时为数组类型，用于内存下泄参数
		{
			//memcpy(&paramValue->rawbytes[0], &msg[param->start_byte - 1], param->byte_count);
		}
	}
	return 0;
}

void Decode::BitReset(ParameterDescriptor *param ,ParameterValue *paramValue,unsigned char * msg){
	//if ((param->byte_count>8))
	//{
	//	memset(&msg[param->start_byte-1],0,param->byte_count);
	//}
	if ((param->byte_count <= 8) && (param->byte_count >= 1))
	{
		if ((param->start_bit+1)%8==0&&param->end_bit==0)
		{
			memset(&msg[param->start_byte-1],0,param->byte_count);
			return;
		}
		switch (param->byte_count)
		{
		case 1:
			{
				BIT8_UNION  temp8;
				temp8.m_uInt8 = (0xff >> (8 - (param->start_bit-param->end_bit + 1))); //BITEND为7
				temp8.m_uInt8 = (temp8.m_uInt8 << param->end_bit);
				temp8.m_uInt8=~(temp8.m_uInt8);
				BIT8_UNION * pbit8_union=(BIT8_UNION *)&msg[param->start_byte - 1];
				pbit8_union->m_uInt8&=temp8.m_uInt8;
				break;
			}

		case 2:
			{
				BIT16_UNION temp16;
				temp16.m_Word  = (0xffff >> (16 - (param->start_bit-param->end_bit + 1)));
				temp16.m_Word  = (temp16.m_Word << param->end_bit);
				temp16.m_Word=~(temp16.m_Word);
				temp16.m_Word=/*htons*/(temp16.m_Word);
				BIT16_UNION * pbit16_union=(BIT16_UNION *)&msg[param->start_byte - 1];
				pbit16_union->m_Word&=temp16.m_Word;
				break;
			}
		case 3: //3个
			{
				BIT32_UNION temp32;
				temp32.m_Long &= (0xffffffff >> (32 - (param->end_bit - param->start_bit+1)));
				temp32.m_Long  = (temp32.m_Long << param->end_bit);
				temp32.m_Long  = ~(temp32.m_Long);
				temp32.m_Long  = /*htons*/(temp32.m_Long);
				BIT32_UNION * pbit32_union=(BIT32_UNION *)&msg[param->start_byte - 1];
				pbit32_union->m_Long&=temp32.m_Long;
				break;
			}
		case 4: //4个
			{
				BIT32_UNION temp32;
				temp32.m_Long &= (0xffffffff >> (32 - (param->end_bit - param->start_bit+1)));
				temp32.m_Long  = (temp32.m_Long << param->end_bit);
				temp32.m_Long  = ~(temp32.m_Long);
				temp32.m_Long  = /*htons*/(temp32.m_Long);
				BIT32_UNION * pbit32_union=(BIT32_UNION *)&msg[param->start_byte - 1];
				pbit32_union->m_Long&=temp32.m_Long;
				break;
			}
		case 5:
			{
				BIT64_UNION temp64;
				temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit-param->end_bit + 1)));
				temp64.m_uInt64  = (temp64.m_uInt64 << param->end_bit);
				temp64.m_uInt64  = ~(temp64.m_uInt64);
				Int64Chang(temp64.m_Bytes);
				BIT64_UNION * pbit64_union=(BIT64_UNION *)&msg[param->start_byte - 1];
				pbit64_union->m_uInt64&=temp64.m_uInt64;
				break;
			}
		case 7:
			{
				BIT64_UNION temp64;
				temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit-param->end_bit + 1)));
				temp64.m_uInt64  = (temp64.m_uInt64 << param->end_bit);
				temp64.m_uInt64  = ~(temp64.m_uInt64);
				Int64Chang(temp64.m_Bytes);
				BIT64_UNION * pbit64_union=(BIT64_UNION *)&msg[param->start_byte - 1];
				pbit64_union->m_uInt64&=temp64.m_uInt64;
				break;
			}
		case 8:
			{
				BIT64_UNION temp64;
				temp64.m_uInt64 &= (0xffffffffffffffff >> (64 - (param->start_bit-param->end_bit + 1)));
				temp64.m_uInt64  = (temp64.m_uInt64 << param->end_bit);
				temp64.m_uInt64  = ~(temp64.m_uInt64);
				Int64Chang(temp64.m_Bytes);
				BIT64_UNION * pbit64_union=(BIT64_UNION *)&msg[param->start_byte - 1];
				pbit64_union->m_uInt64&=temp64.m_uInt64;
				break;
			}
		default:
			break;
		}
	}
}

unsigned int Decode::get_Messagecount(){
	return ptr_msglib->count;
}

/******************************************************************
*函数功能:解算流程。
*输入参数:
*输出参数:
*返回值  :
*******************************************************************/

int Decode::reverse_ele(Message* handle, u_int8_t *msg)
{
	int index = 0;
	unsigned int start_byte;
	MessageDescriptor *msgDescriptor = handle->msg_descriptor;
	unsigned int paramCount = msgDescriptor->param_cnt;
	ParameterDescriptor *param = NULL;
	ParameterValue *paramValue;


	for (index = 0; index < paramCount; index++)
	{
		paramValue = &handle->param_value[index];
		param = &msgDescriptor->parameters[index];
		start_byte = param->start_byte;
		//增加对起始字节填写错误的判断
		if(start_byte < 1)
		{
			fprintf(pLogFile,"%s:参数%s的起始字节错误,服务器程序已退出.\n",UtilTool::GetTimeString().c_str(), param->paramID);
			//exit(EXIT_FAILURE);
		}

		//根据字节个数处理
		if ((param->byte_count <= 8) && (param->byte_count >= 1))
		{
			switch (param->byte_count)
			{
			case 1:

				break;

			case 2:
				{
					BIT16_UNION * temp16=(BIT16_UNION *)&msg[param->start_byte - 1];
					temp16->m_Word = /*ntohs*/(temp16->m_Word);
				}
				break;
			case 3: //3个
				{
					BIT32_UNION * temp32=(BIT32_UNION *)&msg[param->start_byte - 1];
					temp32->m_Long = /*ntohl*/(temp32->m_Long);
					break;
				}
			case 4: //4个
				{
					BIT32_UNION * temp32=(BIT32_UNION *)&msg[param->start_byte - 1];
					temp32->m_Long = /*ntohl*/(temp32->m_Long);
					break;
				}
			case 5:
				{
					BIT64_UNION * temp64=(BIT64_UNION *)&msg[param->start_byte - 1];
					Int64Chang((unsigned char *)temp64);
					break;
				}
			case 6:
				{
					BIT64_UNION * temp64=(BIT64_UNION *)&msg[param->start_byte - 1];
					Int64Chang((unsigned char *)temp64);
					break;
				}
			case 7:
				{
					BIT64_UNION * temp64=(BIT64_UNION *)&msg[param->start_byte - 1];
					Int64Chang((unsigned char *)temp64);
					break;
				}
			case 8:
				{
					BIT64_UNION * temp64=(BIT64_UNION *)&msg[param->start_byte - 1];
					Int64Chang((unsigned char *)temp64);
					break;
				}
			default:
				break;
			}
		}
	}
	return 1;
}

int Decode::getRedisElementInt(CRedis & redis, const char * sheetName,const char * suffix){
	char keystr[100]={0};
	int keylen=sprintf_s(keystr,"%s%s",sheetName,suffix);
	return redis.GetIntFromString(keystr);
}

Message * Decode::CreateMessageByMessage(Message * pMessage){
	Message * message_ptr=new Message();
	message_ptr->msg_descriptor=pMessage->msg_descriptor;
	message_ptr->param_value=(ParameterValue *) malloc(message_ptr->msg_descriptor->param_cnt*sizeof(ParameterValue));
	memcpy(message_ptr->param_value,pMessage->param_value,message_ptr->msg_descriptor->param_cnt*sizeof(ParameterValue));
	return message_ptr;
}


char * Decode::getParmentName(Message * pMessage,int index,int * len){
	if (pMessage==NULL)
	{
		fprintf(pLogFile,"数据包为空\n");
		return "";
	}
	if (index<pMessage->msg_descriptor->param_cnt)
	{
		char * result= pMessage->msg_descriptor->parameters[index].paramName;
		*len=strlen(result);
		return result;
	}
	return "";
}

int Decode::getParmentResultInfo(Message * pMessage,int * packetlen,int * rt,int *subrt,int * msgformat,char * msgchoose,char * msgtype){
	if (pMessage==NULL)
	{
		* packetlen=0; * rt=0; *subrt=0; * msgformat=0;
		return 0;
	}
	else{
		* packetlen=pMessage->msg_descriptor->msg_size; 
		* rt=pMessage->msg_descriptor->rt;
		*subrt=pMessage->msg_descriptor->subrt; 
		* msgformat=pMessage->msg_descriptor->msgformat;
		strcpy(msgchoose,pMessage->msg_descriptor->msg_choose);
		strcpy(msgtype,pMessage->msg_descriptor->msg_type);
		return 1;
	}
}

int Decode::getParmentResultCnt(Message * pMessage){
	if (pMessage==NULL)
	{
		return 0;
	}
	return pMessage->msg_descriptor->param_cnt;
}

char * Decode::getParmentNumberString(Message * pMessage,int index,int * len){
	if (pMessage==NULL)
	{
		fprintf(pLogFile,"数据包为空\n");
		return "";
	}
	char * presultstr=new char[300];
	memset(presultstr,0,300);
	char temp[10]={0};
	int size=0;
	if (index<(pMessage)->msg_descriptor->param_cnt)
	{	
		ParameterDescriptor *parameters=&(pMessage)->msg_descriptor->parameters[index];
		ParameterValue *param_value=&(pMessage)->param_value[index];
		switch(parameters->display_type){
		case SHOWTYPE_DOUBLE_ID:	
			*len=sprintf(presultstr,"%f",param_value->v.doublevalue);
			return presultstr;
			break;
		case SHOWTYPE_FLOAT_ID:		
			*len=sprintf(presultstr,"%f",param_value->v.floatvalue);
			return presultstr;
			break;
		case SHOWTYPE_FLOAT16_ID:
			BIT32_UNION temp32;
			temp32.m_Long  = param_value->v.rawvalue;
			temp32.m_Long  <<=16;
			ParameterValue parmvaluetemp;
			parmvaluetemp.v.rawvalue=temp32.m_Long;
			*len=sprintf(presultstr,"%f",parmvaluetemp.v.floatvalue);
			return presultstr;
			break;		
		case SHOWTYPE_ARRAY_ID:
			*len=0;
			for (int i=0;i<parameters->byte_count;i++)
				*len+=sprintf(&presultstr[strlen(presultstr)],"%02X ",parameters->defaultary[i]);
			return presultstr;
			break;
		default:	
			char * result=UtilTool::radixtran(presultstr,param_value->v.rawvalue,parameters->radix_type,0);
			*len=strlen(result);
			//	memcpy(presultstr,result,*len);
			return presultstr;
			break;
		}
	}
	return "";
}

char * Decode::getParmentResult(Message * pMessage,int index,int * len){
	if (pMessage==NULL)
	{
		fprintf(pLogFile,"数据包为空\n");
		return "";
	}
	int size=0;
	if (len==0)
	{
		len=&size;
	}
	char * presultstr=new char[1000];
	memset(presultstr,0,300);
	char temp[10]={0};

	if (index<(pMessage)->msg_descriptor->param_cnt)
	{	
		ParameterDescriptor *parameters=&(pMessage)->msg_descriptor->parameters[index];
		ParameterValue *param_value=&(pMessage)->param_value[index];
		switch(parameters->display_type){
		case SHOWTYPE_DOUBLE_ID:	
			*len=sprintf(presultstr,"%f",param_value->v.doublevalue);
			return presultstr;
			break;
		case SHOWTYPE_FLOAT_ID:		
			*len=sprintf(presultstr,"%f",param_value->v.floatvalue);
			return presultstr;
			break;
		case SHOWTYPE_FLOAT16_ID:
			BIT32_UNION temp32;
			temp32.m_Long  = param_value->v.rawvalue;
			temp32.m_Long  <<=16;
			ParameterValue parmvaluetemp;
			parmvaluetemp.v.rawvalue=temp32.m_Long;
			*len=sprintf(presultstr,"%f",parmvaluetemp.v.floatvalue);
			return presultstr;
			break;		
		case SHOWTYPE_ARRAY_ID:
			*len=0;
			for (int i=0;i<parameters->byte_count;i++)
				*len+=sprintf(&presultstr[strlen(presultstr)],"%02X ",(unsigned char)param_value->rawbytes[i]);
			return presultstr;
			break;

		case SHOWTYPE_INT16_ID:
			if (parameters->radix_type==10)
			{
				*len=sprintf(presultstr,"%d",(short)param_value->v.floatvalue);	
				return presultstr;
				break;
			}


		case SHOWTYPE_INT8_ID:
			if (parameters->radix_type==10)
			{
				*len=sprintf(presultstr,"%d",(char)param_value->v.rawvalue);	
				return presultstr;
				break;
			}


		case SHOWTYPE_INT32_ID:

			if (parameters->radix_type==10)
			{
				*len=sprintf(presultstr,"%d",(int)param_value->v.rawvalue);	
				return presultstr;
				break;
			}

		case SHOWTYPE_INT64_ID:

			if (parameters->radix_type==10)
			{
				*len=sprintf(presultstr,"%d",(__int64)param_value->v.rawvalue);	
				return presultstr;
				break;
			}

		default:	
			if (parameters->annotation_cnt)
			{
				for (int i=0;i<parameters->annotation_cnt;i++)
				{
					if (param_value->v.rawvalue==parameters->annotation_val[i])
					{
						* len=strlen(parameters->annotation_str[i]);
						memcpy(presultstr,parameters->annotation_str[i],strlen(parameters->annotation_str[i])) ;
						return presultstr;
					}
				}
				return presultstr;
			}
			char * result=UtilTool::radixtran(presultstr,param_value->v.rawvalue,parameters->radix_type);
			*len=strlen(result);
			//		memcpy(presultstr,result,*len);
			return presultstr;
			break;
		}

	}
	return "";
}

char * Decode::getRedisElementStr(CRedis & redis,const char * sheetName,int row,int col){
	char keystr[100]={0};
	int keylen=sprintf_s(keystr,"%s%03d%03d",sheetName,row,col);
	int len=0;
	return redis.Get(keystr,&len);
}

int Decode::getIndex(Message * msg,string elementName){
	if (msg)
	{
		for (int index=0;index<msg->msg_descriptor->param_cnt;index++)
		{
			ParameterDescriptor *parameters=&msg->msg_descriptor->parameters[index];
			ParameterValue *param_value=&msg->param_value[index];


			if (!strcmp(parameters->paramID,elementName.c_str()))   {
				return parameters->parm_index;
			}
		}
		return -1;
	}
	return -1;
}


int Decode::getMessageNames(char * pMessageName){
	int i=0;
	int index=0;
	for (;;i++)
	{
		PluginMsgNode* pNode=get_PluginNode_by_ID(i);
		if (pNode)
		{
			if (index){
				pMessageName[index]=',';
				index++;
			}
			memcpy(&pMessageName[index],pNode->msgName,strlen(pNode->msgName));	
			index+=strlen(pNode->msgName);
		}else{
			break;
		}
		pMessageName[index]=',';
	}
	return i;
}

int Decode::getMessageSize(Message * pMessage){
	if (pMessage)
	{
		return pMessage->msg_descriptor->msg_size;
	}else 
		return 0;
}

int  Decode::getAnnotation_cnt(Message * pMessage,int index){
	if (pMessage)
	{
		return pMessage->msg_descriptor->parameters[index].annotation_cnt;
	}else 
		return 0; 
}

char * Decode::getAnnotation(Message * pMessage,int parmindex,int annotationindex,int * len){
	if (pMessage)
	{
		if (parmindex<pMessage->msg_descriptor->param_cnt)
		{
			if (annotationindex<pMessage->msg_descriptor->parameters[parmindex].annotation_cnt)
			{
				char * panntation= pMessage->msg_descriptor->parameters[parmindex].annotation_str[annotationindex];
				*len=strlen(panntation);
				return panntation;
			}
		}
	}else 
		return 0; 
	return 0;
}

int Decode::getAnnotationVal(Message * pMessage,int parmindex,int annotationindex){
	if (pMessage)
	{
		if (parmindex<pMessage->msg_descriptor->param_cnt)
		{
			if (annotationindex<pMessage->msg_descriptor->parameters[parmindex].annotation_cnt)
			{
				return pMessage->msg_descriptor->parameters[parmindex].annotation_val[annotationindex];
			}
		}
	}else 
		return 0; 
	return 0;
}

char * Decode::getMsgName(int i){
	PluginMsgNode* pNode=get_PluginNode_by_ID(i);
	if (pNode)
	{
		return pNode->msgDescriptor->name;
	}else 
		return NULL;
}

char * Decode::getMsgShowName(int i){
	PluginMsgNode* pNode=get_PluginNode_by_ID(i);
	if (pNode)
	{
		return pNode->msgDescriptor->cshowname;
	}else 
		return NULL;
}

MessageDescriptor *  Decode::getMsgDescribe(int i)
{
	PluginMsgNode* pNode=get_PluginNode_by_ID(i);
	if (pNode)
	{
		return pNode->msgDescriptor;
	}else 
		return NULL;
}


int Decode::isDefault(Message * msg,int index){
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *parameters=&msg->msg_descriptor->parameters[index];
		ParameterValue *param_value=&msg->param_value[index];
		double value=0;

		char * presult=decode->getParmentResult(msg,index);
		value=atof(presult);
		delete presult;
		switch(parameters->display_type){
		case SHOWTYPE_DOUBLE_ID:	
			return value==parameters->defaultval;	 
			break;
		case SHOWTYPE_FLOAT_ID:	
			if (!((parameters->equivalent > -0.000000000001) && (parameters->equivalent < 0.000000000001))) //如果有当量

				return (float)value==(float)parameters->defaultval;	
			else
				return (float)param_value->v.floatvalue==(float)parameters->defaultval;	
			break;
		case SHOWTYPE_FLOAT16_ID:	
			ParameterValue parmvaluetemp;
			memset(&parmvaluetemp,0,sizeof(ParameterValue));
			parmvaluetemp.v.floatvalue=parameters->defaultval;
			parmvaluetemp.v.rawvalue=parmvaluetemp.v.rawvalue>>16;
			return param_value->v.rawvalue==parmvaluetemp.v.rawvalue;
			break;
		case  SHOWTYPE_ARRAY_ID:
			for (int i=0;i<parameters->byte_count;i++)
			{
				if (parameters->defaultary[i]!=(unsigned char)param_value->rawbytes[i])
				{
					return false;
				}
			}	
			return true;
			break;
		default:	
			return	 param_value->v.rawvalue==(int64_t)parameters->defaultval;	
			break;
		}	
	}
	return -1;
}
int Decode::setValueByNumberString(Message * msg,const  char *  name,const  char * val){
	int index=getIndex(msg,name);
	if (index<0)
	{
		return -1;
	}else
		setValueByNumberString(msg,index,val);
	return 0;
}

int Decode::setValueByNumberString(Message * msg,int index,const  char * val){
	if (msg&&val)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		double value=0;
		ParameterDescriptor *parameters=&msg->msg_descriptor->parameters[index];
		ParameterValue *param_value=&msg->param_value[index];

		vector<string> strvec;
		switch(parameters->display_type){

		case SHOWTYPE_DOUBLE_ID:		
			value=atof(val);
			param_value->v.doublevalue=value;
			break;
		case SHOWTYPE_FLOAT_ID:	
			value=atof(val);
			param_value->v.floatvalue=value;
			break;
		case SHOWTYPE_FLOAT16_ID:
			value=atof(val);
			param_value->v.floatvalue=value;
			BIT32_UNION temp32;
			temp32.m_Long = param_value->v.rawvalue;
			temp32.m_Long >>=16;
			param_value->v.rawvalue=temp32.m_Long;
			break;
		case SHOWTYPE_ARRAY_ID:
			strvec= UtilTool::split_string((char *)val);
			memset(param_value->rawbytes,0,parameters->byte_count);
			for(int i=0;i<strvec.size();i++)
				param_value->rawbytes[i]=strtol( strvec[i].c_str(), NULL, 16 );
			break;
		default:	
			param_value->v.rawvalue= strtoul(val, NULL, parameters->radix_type );
			break;
		}		


		return 0;
	}else{
		return -1;
	}
}

int Decode::setValue(Message * msg,int index,double val){
	char buf[100];
	sprintf_s(buf,"%f",val);
	return	Decode::setValue(msg,index,buf);
}
int Decode::setValue(Message * msg,const  char *  name,const  char * val){
	int index=getIndex(msg,name);
	if (index<0)
	{
		return -1;
	}else
		setValue(msg,index,val);
	return 0;
}
int Decode::setValue(Message * msg,int index,const  char * val){
	if (msg&&val)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		double value=0;
		ParameterDescriptor *parameters=&msg->msg_descriptor->parameters[index];
		ParameterValue *param_value=&msg->param_value[index];
		if (parameters->annotation_cnt)
		{
			int i=0;
			for (;i<parameters->annotation_cnt;i++)
			{
				if (strcmp(val,parameters->annotation_str[i])==0)
				{
					value= parameters->annotation_val[i];
					if (parameters->upperlimit!=parameters->lowerlimit)
					{
						{
							if (value>parameters->upperlimit)
							{
								value=parameters->upperlimit;
								return -1;
							}else if (value<parameters->lowerlimit)
							{
								value=parameters->lowerlimit;
								return -1;
							}
						}
					}else {
						param_value->v.rawvalue=value;
					}
					break;
				}	
			}
			if (i==parameters->annotation_cnt)
			{
				return -1;
			}
		}	
		else		
		{
			vector<string> strvec;
			switch(parameters->display_type){

			case SHOWTYPE_DOUBLE_ID:		
				value=atof(val);
				param_value->v.doublevalue=value;
				break;
			case SHOWTYPE_FLOAT_ID:	
				value=atof(val);
				param_value->v.floatvalue=value;
				break;
			case SHOWTYPE_FLOAT16_ID:
				value=atof(val);
				param_value->v.floatvalue=value;
				BIT32_UNION temp32;
				temp32.m_Long = param_value->v.rawvalue;
				temp32.m_Long >>=16;
				param_value->v.rawvalue=temp32.m_Long;
				break;
			case SHOWTYPE_ARRAY_ID:
				strvec= UtilTool::split_string((char *)val);
				memset(param_value->rawbytes,0,parameters->byte_count);
				for(int i=0;i<strvec.size();i++)
					param_value->rawbytes[i]=strtol( strvec[i].c_str(), NULL, 16 );
				break;
			default:	

				param_value->v.rawvalue=GetIntValueFromString(val);
				break;
			}		
		}

		return 0;
	}else{
		return -1;
	}
}

int Decode::getMsgRawLength(Message * msg){
	if (msg)
	{
		return msg->msg_descriptor->msg_size;
	}
	return -1;
}

int Decode::getAnnotation_index(Message * msg,int index){
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		ParameterValue *paramValue=&msg->param_value[index];
		for (int i=0;i<param->annotation_cnt;i++)
		{
			if (paramValue->v.rawvalue==param->annotation_val[i])
			{
				return i;
			}
		}
	}
	return -1;
}

const char * Decode::get_markid(Message * msg,int index){
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return NULL;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->paramID;
	}
	return NULL;
}



int Decode::get_parmrawpos(Message * msg,int index,int * pintStart,int * pintCount){
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		if (pintStart)
		{
			*pintStart=param->start_byte;
		}
		if (pintCount)
		{
			*pintCount=param->byte_count;
		}
	}
	return -1;
}

void * Decode::getValueData(Message * pMessage){
	if (pMessage)
	{		
		void * pData=malloc(pMessage->msg_descriptor->value_size);
		memcpy(pData,pMessage->param_value,pMessage->msg_descriptor->value_size);
		return pData;
	}
	return NULL;
}

int Decode::getParmStartByte(Message * msg,int index) //起始字节
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->start_byte;

	}
	return -1;
}

int Decode::getParmByteCount(Message * msg,int index) //字节个数
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->byte_count;

	}
	return -1;
}

int Decode::getParmEndBit(Message * msg,int index)  //结束位
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->end_bit;

	}
	return -1;
}

int Decode::getParmStartBit(Message * msg,int index)  //起始位
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->start_bit;

	}
	return -1;
}

int Decode::getparmBitLength(Message * msg,int index)  //参数的比特位总长度
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->bit_length;

	}
	return -1;
}

char * Decode::getparamID(Message * msg,int index)	//参数代号
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return NULL;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->paramID;

	}
	return NULL;
}

char * Decode::getparamName(Message * msg,int index) //参数名称
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return NULL;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->paramName;

	}
	return NULL;
}

char * Decode::getparamUnit(Message * msg,int index)  //参数单位
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return NULL;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->paramUnit;

	}
	return NULL;
}
double Decode::getparamIsEnable(Message * msg,int index)  //默认值
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->is_enable;

	}
	return -1;

}

double Decode::getparamDefaultVal(Message * msg,int index)  //默认值
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->defaultval;

	}
	return -1;
}


double Decode::getparamUpperLimit(Message * msg,int index)   //上限
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->upperlimit;

	}
	return -1;
}


double Decode::getparamLowerLimit(Message * msg,int index)  //下限
{
	if (msg)
	{
		if (index>=msg->msg_descriptor->param_cnt)
		{
			return -1;
		}
		ParameterDescriptor *param=&msg->msg_descriptor->parameters[index];
		return param->lowerlimit;

	}
	return -1;
}

