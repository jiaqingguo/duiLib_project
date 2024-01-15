#include "stdafx.h"
#include "ConnectMysql.h"
#include <time.h>
#include <iostream>
#include <winsock.h>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
//#pragma comment(lib, "libmysql.lib")

ConnectMysql::ConnectMysql(void)
{
	m_num = 0;
}
ConnectMysql::~ConnectMysql(void)
{
	//mysql_close(&m_mydata);
	
    //mysql_free_result(res);    
    mysql_close(&m_mydata);    
    //free(&m_mydata);    

}
vector<string> my_split_inMysql(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;

	str += pattern;//扩展字符串以方便操作
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
//获取当前时间
string ConnectMysql::getCurrentTime()
{
	char current_time[40];
	time_t rawtime;
	rawtime = time(NULL);
	struct tm tt;
	//time(&rawtime);
	localtime_s(&tt, &rawtime);
	int year, month, day, hour, min, sec;
	year = 1900 + tt.tm_year;
	month = 1 + tt.tm_mon;
	day = tt.tm_mday;
	hour = tt.tm_hour;
	min = tt.tm_min;
	sec = tt.tm_sec;
	sprintf(current_time,  "%d_%d_%d_%d_%d_%d", year, month, day, hour, min, sec);
	string time_string(current_time);
	return time_string;
}
bool ConnectMysql::InitDatabase()
{
	//MYSQL m_mydata = sql;
	if (mysql_library_init(0, NULL, NULL) == 0)
		cout << "mysql_library_init() succeed" << endl;
	else
	{
		cout << "mysql_library_init() failed" << endl;
		return false;
	}
	//初始化数据结构
	if (mysql_init(&m_mydata) != NULL)
		cout << "mysql_init() succeed" << endl;
	else
	{
		cout << "mysql_init() failed" << endl;
		return false;
	}//在连接数据库之前，设置额外的连接选项
	//可以设置的选项很多，这里设置字符集，否则无法处理中文
	if (mysql_options(&m_mydata, MYSQL_SET_CHARSET_NAME, "gbk") == 0)
		cout << "mysql_options() succeed" << endl;
	else
	{
		cout << "mysql_options() failed" << endl;
		return false;
	}
	return TRUE;
}
//连接数据库
bool ConnectMysql::StartConnectMysql()
{
	//MYSQL m_mydata = sql;
	if (!InitDatabase())
	{
		return false;
	}
	//if (mysql_real_connect(&m_mydata, "localhost", "root", "123456", "shoesshop", 3306, NULL, 0) != NULL)	//这里的地址，用户名，密码，端口可以根据自己本地的情况更改
	//if (mysql_real_connect(&m_mydata, "127.0.0.1", "root", "123456", "jh_database", 3306, NULL, 0) != NULL)
	if (mysql_real_connect(&m_mydata, "127.0.0.1", "root", "123456", "jh_database", 3306, NULL, 0) != NULL)
		cout << "mysql_real_connect() succeed" << endl;
	else
	{
		cout << "mysql_real_connect() failed: " << mysql_error(&m_mydata) << endl;
		return false;
	}
	return true;
}
//创建新的表
bool ConnectMysql::CreateNewTable(string name)
{

	//MYSQL m_mydata = sql;
	string sqlstr;
	///////////////////////创建一个表/////////////
	sqlstr = "create table if not exists ";
	sqlstr += name;
	sqlstr += " (";
	sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID',";
	sqlstr += "u_time datetime COMMENT 'record time',";
	sqlstr += "u_X VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Y VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Z VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_GD VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_FY VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_PH VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Dof7 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Dof8 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	sqlstr += "u_Dof9 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time'";
	sqlstr += ");";
	//sqlstr = "create table if not exists 光纤1_2121 (u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID',u_time VARCHAR(100) CHARACTER SET gb2312 COLLATE gb2312_chinese_ci NULL COMMENT 'Name Of User');";
	cout << sqlstr << endl;
	if (mysql_query(&m_mydata, sqlstr.c_str()) == 0)
		cout << "mysql_query() create table succeed" << endl;
	else
	{
		//string str = mysql_error(&m_mydata);
		cout << "mysql_query() create table failed" << mysql_error(&m_mydata)<<endl;
		mysql_close(&m_mydata);
		return FALSE;
	}
	return true;
}
//给了列名称创建新表
bool ConnectMysql::CreateNewTable(string name,vector<string> vec)
{

	//MYSQL m_mydata = sql;
	string sqlstr;
	///////////////////////创建一个表/////////////
	sqlstr = "create table if not exists ";
	sqlstr += name;
	sqlstr += " (";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		
		sqlstr += vec[i];
		sqlstr += " VARCHAR(30) ";
		sqlstr += ",";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += " VARCHAR(30) ";
	sqlstr += ");";
	//sqlstr += "u_time datetime COMMENT 'record time',";
	//sqlstr += "u_X VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Y VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Z VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_GD VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_FY VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_PH VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Dof7 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Dof8 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time',";
	//sqlstr += "u_Dof9 VARCHAR(30)  NOT NULL DEFAULT 0 COMMENT 'The Summation Of Using Time'";
	//sqlstr += ");";
	//sqlstr = "create table if not exists 光纤1_2121 (u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID',u_time VARCHAR(100) CHARACTER SET gb2312 COLLATE gb2312_chinese_ci NULL COMMENT 'Name Of User');";
	cout << sqlstr << endl;
	if (mysql_query(&m_mydata, sqlstr.c_str()) == 0)
		cout << "mysql_query() create table succeed" << endl;
	else
	{
		string str = mysql_error(&m_mydata);
		cout << "mysql_query() create table failed" << mysql_error(&m_mydata)<<endl;
		cout << str << endl;
		mysql_close(&m_mydata);
		return FALSE;
	}
	return true;
}
//向数据表中增加一条数据 insert into 2022_9_20_14_25_43_s values("1","2022_9_20","11","12","13","14","15","16","17","18","19");
void ConnectMysql::addMsgToTable(string name, string x, string y, string z, string gd, string fy, string ph, string dof7, string dof8, string dof9)
{
	m_num += 1;
	string sqlstr;
	string current_time;
	current_time = getCurrentTime();
	string u_id, u_time;
	//int u_x, u_y, u_z, u_GD, u_FY,u_PH ,u_Dof7,u_Dof8 ,u_Dof9;

	//向表中插入数据
	sqlstr = "insert into ";
	sqlstr += name;

	string s_num;
	std::ostringstream streamsssss;
	streamsssss << m_num;
	s_num=streamsssss.str();

	sqlstr += " values('" + s_num + "','" + current_time.c_str() + "','" + x + "','" + y + "','" + z + "','" + gd + "','" + fy +
		"','" + ph + "','" + dof7 + "','" + dof8 +"','" + dof9 + "');";
	cout << sqlstr << endl;
	if (mysql_query(&m_mydata, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() insert data succeed" << endl;
	}
	else
	{
		cout << "mysql_query() insert data failed" << endl;
		mysql_close(&m_mydata);
		//return FALSE;
	}
	//return TRUE;
}
//向数据表中添加数据
void ConnectMysql::addMsgToTable(string name,vector<string> vec)
{
	string sqlstr;
	string current_time;
	current_time = getCurrentTime();
	string u_id, u_time;
	//int u_x, u_y, u_z, u_GD, u_FY,u_PH ,u_Dof7,u_Dof8 ,u_Dof9;

	//向表中插入数据
	sqlstr = "insert into ";
	sqlstr += name;

	sqlstr += " values('";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		
		sqlstr += vec[i];
		sqlstr += "','";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += "');";
	cout << "";
	if (mysql_query(&m_mydata, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() insert data succeed" << endl;
	}
	else
	{
		cout << "mysql_query() insert data failed" << endl;
		mysql_close(&m_mydata);
		//return FALSE;
	}
}
//查询表格
bool ConnectMysql::showAllTable(string name_Table)
{
	string sqlstr;
	char id[20];
	string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	cout << sqlstr << endl;
	//MYSQL_RES *result_select = NULL;
	if (mysql_query(&m_mydata, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() select data succeed" << endl;
		res = mysql_store_result(&m_mydata);
		//取得并打印行数
		int rowcount = mysql_num_rows(res);
		cout << "row count: " << rowcount << endl;
		//取得并打印各字段的名称
		unsigned int fieldcount_select = mysql_num_fields(res);
		MYSQL_FIELD *field = NULL;
		for (unsigned int i = 0; i < fieldcount_select; i++)
		{
			field = mysql_fetch_field_direct(res, i);
			cout << field->name << "\t\t";
		}
		cout << endl;
		//打印各行
		MYSQL_ROW row_select = NULL;
		row_select = mysql_fetch_row(res);
		while (NULL != row_select)
		{
			for (int i = 0; i < fieldcount_select; i++)
				cout << row_select[i] << "\t\t";
			cout << endl;
			row_select = mysql_fetch_row(res);
		}
	}
	else
	{
		cout << "mysql_query() select data failed" << endl;
		mysql_close(&m_mydata);
		return FALSE;
	}
	return TRUE;
}
void ConnectMysql::closeDatabase()
{
	mysql_close(&m_mydata);
	//mysql_shutdown(&m_mydata, KILL_CONNECTION);
	//mysql_stmt_close(&m_mydata);
	//mysql_stmt_reset(&m_mydata);
	
}
//显示数据库中所有表名称
bool ConnectMysql::showAllTableName()
{
	string sqlstr;
	char id[20];
	string string_id(id);
	sqlstr = "show tables;";
	//sqlstr += name_Table;
	cout << sqlstr << endl;
	res = NULL;
	if (mysql_query(&m_mydata, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() select data succeed" << endl;
		res = mysql_store_result(&m_mydata);
		//取得并打印行数
		int rowcount = mysql_num_rows(res);
		cout << "row count: " << rowcount << endl;
		//取得并打印各字段的名称
		unsigned int fieldcount_select = mysql_num_fields(res);
		MYSQL_FIELD *field = NULL;
		for (unsigned int i = 0; i < fieldcount_select; i++)
		{
			field = mysql_fetch_field_direct(res, i);
			cout << field->name << "\t\t";
		}
		cout << endl;
		//打印各行
		MYSQL_ROW row_select = NULL;
		row_select = mysql_fetch_row(res);
		while (NULL != row_select)
		{
			for (int i = 0; i < fieldcount_select; i++)
				cout << row_select[i] << "\t\t";
			cout << endl;
			row_select = mysql_fetch_row(res);
		}
	}
	else
	{
		cout << "mysql_query() select data failed" << endl;
		mysql_close(&m_mydata);
		return FALSE;
	}
	return TRUE;
}
//设置当前数据库
bool ConnectMysql::SetUpDatabase(string database)
{
	bool returnResult;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "use ";
	sqlstr += database;
	sqlstr += ";";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		return false;
		//return false;
	}
	else
	{
		printf("query success\n");
		return true;
	}
}
//返回指定数据表中所有内容
vector<vector<string>> ConnectMysql::mytest_QueryDatabase(string name_Table)
{
	vector<vector<string>> vec_result;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr,outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	sqlstr += ";";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//unsigned int affect_row = mysql_affected_rows(&m_mydata);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));//

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");

	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}

	return vec_result;
}
//返回指定数据表中所有内容
string ConnectMysql::mytest_QueryDatabase_1(string name_Table)
{
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[2048]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t\n", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7]);  //column是列数组  
		snprintf(current_c, 2048, "%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s!", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7]);
		string result(current_c);
		outPut += result;
		cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}

string ConnectMysql::mytest_showDbAllName()
{
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "show tables;";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\n", column[0]);  //column是列数组  
		sprintf(current_c, "%s!", column[0]);
		string result(current_c);
		outPut += result;
		//cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}
//返回某段时间内的数据
string ConnectMysql::mytest_QueryByTime(string start, string end, string name_Table)
{
	//select * from 动力学_2022_09_27_10_21_42_292 where u_time between '2022_9_27 10_21_44'  and '2022-9-27 10:21:46';
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select * from ";
	sqlstr += name_Table;
	sqlstr += " where u_time between '";
	sqlstr += start;
	sqlstr += "' and '";
	sqlstr += end;
	sqlstr += "';";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7], column[8], column[9], column[10]);  //column是列数组  
		sprintf(current_c, "%s*%s*%s*%s*%s*%s*%s*%s*%s*%s*%s!", column[0], column[1], column[2], column[3], column[4], column[5], column[6], column[7], column[8], column[9], column[10]);
		string result(current_c);
		outPut += result;
		cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}

//根据选择的列返回某段时间内的数据
vector<vector<string>> ConnectMysql::mytest_QueryByTime(string start, string end, string name_Table,vector<string> vec)
{
	vector<vector<string>> vec_result;
	//select u_id,u_time,u_X from 动力学_2022_10_09_09_47_51_593 where u_time between '2019_9_27 10_21_44'  and '2022-9-27 10:21:46';
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	sqlstr = "select ";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		sqlstr += vec[i];
		sqlstr += ",";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += " where u_time between '";
	sqlstr += start;
	sqlstr += "' and '";
	sqlstr += end;
	sqlstr += "';";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}
	return vec_result;
}

//查询数据表中各个列名称
//SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR "*") FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '111111' AND TABLE_NAME = '动力学_2022_10_09_09_47_51_593';
vector<string> ConnectMysql::mytest_QueryColumns(string dbName,string name_Table)
{
	m_ColumnLenght = 0;
	vector<string> vec;
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	//sqlstr = "SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR '*') FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '";
	sqlstr = "SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR '*') FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '";
	sqlstr += dbName;
	sqlstr += "' AND TABLE_NAME = '";
	sqlstr += name_Table;
	sqlstr += "';";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%s", column[0]);  //column是列数组  
		sprintf(current_c, "%s", column[0]);
		string result(current_c);
		outPut += result;
		cout << result << endl;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;
	vec = my_split_inMysql(outPut,"*");
	m_ColumnLenght = vec.size();

	return vec;
}
//获取所有数据库名称
vector<string> ConnectMysql::mytest_QueryDatabase()
{
	vector<string > vec;
	vector<vector<string>> vec_result;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "show databases;";

	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//unsigned int affect_row = mysql_affected_rows(&m_mydata);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));//

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");

	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		//vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		//vec_result.push_back(vec);

	}

	return vec;

	//m_ColumnLenght = 0;
	//vector<string> vec;
	////MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	//MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	//char current_c[150]; //查询语句
	//string sqlstr, outPut;

	////char id[20];
	////string string_id(id);
	////sqlstr = "SELECT GROUP_CONCAT(COLUMN_NAME SEPARATOR '*') FROM information_schema.COLUMNS WHERE TABLE_SCHEMA = '";
	//sqlstr = "show databases;";

	//cout << sqlstr << endl;
	////sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	////mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	////返回0 查询成功，返回1查询失败  
	//if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	//{
	//	printf("Query failed (%s)\n", mysql_error(&m_mydata));
	//	//return false;
	//}
	//else
	//{
	//	printf("query success\n");
	//}
	////获取结果集  
	//if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	//{
	//	printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
	//	//return false;
	//}
	//unsigned int fieldcount_select = mysql_num_fields(res);
	////打印数据行数  
	//printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	////获取字段的信息  
	//char *str_field[40];  //定义一个字符串数组存储字段信息  
	//for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	//{
	//	str_field[i] = mysql_fetch_field(res)->name;
	//}
	//for (int i = 0; i < fieldcount_select; i++)  //打印字段  
	//	printf("%10s\t", str_field[i]);
	//printf("---------------------------\n");
	////打印获取的数据  
	//while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	//{
	//	printf("%s", column[0]);  //column是列数组  
	//	sprintf(current_c, "%s", column[0]);
	//	string result(current_c);
	//	outPut += result;
	//	cout << result << endl;
	//	//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	//}
	////string result = clumn[0] + ;
	////vec = my_split_inMysql(outPut, "*");


	//return vec;
}
bool ConnectMysql::CreateNewTableAndInsert(string name,vector<string> columnName,vector<string> columnValue)//columnName是列名 columnValue是数据的vector
{
	if(CreateNewTable(name,columnName))
	{
		cout << "创建数据表成功";
	}else
	{
		cout << "创建数据表失败";
	}
	string sqlstr;
	//insert into 2022_9_20_14_25_43_s values("1","2022_9_20","11","12","13","14","15","16","17","18","19");
	sqlstr = "insert into ";
	sqlstr += name;
	sqlstr += " values('";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";
	for(int i = 0; i < columnValue.size() - 1; i++)
	{

		sqlstr += columnValue[i];
		sqlstr += "','";
	}
	sqlstr += columnValue[columnValue.size() - 1];
	sqlstr += "');";
	if (mysql_query(&m_mydata, sqlstr.c_str()) == 0)
	{
		cout << "mysql_query() insert data succeed" << endl;
	}
	else
	{
		cout << "mysql_query() insert data failed" << endl;
		mysql_close(&m_mydata);
		return false;
	}
	return true;
}

string ConnectMysql::mytest_QueryByColumnName(string name_Table,string columnName)
{
	//select u_id,u_time,u_X from 动力学_2022_10_09_09_47_51_593;
	//MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select u_id,u_time,";
	sqlstr += columnName;
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += ";";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  
	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		printf("%10s\t%10s\t%10s", column[0], column[1], column[2]);  //column是列数组  
		sprintf(current_c, "%s*%s*%s!", column[0], column[1], column[2]);
		string result(current_c);
		outPut += result;
		cout << result;
		//result = column[0] + column[1] + column[2] + column[3] + column[4] + column[5] + column[6] + column[7], +column[8] + column[9] + column[10];
	}
	//string result = clumn[0] + ;

	return outPut;
}

vector<vector<string>> ConnectMysql::mytest_QueryByColumnName(string name_Table, string columnName, string data)
{
	vector<vector<string>> vec_result;
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr, outPut;
	//select * from 0_星座数据表_1 where constellationName="StarLink";
	//char id[20];
	//string string_id(id);
	sqlstr = "select * ";
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += " where ";
	sqlstr += columnName;
	sqlstr += "='";
	sqlstr += data;
	sqlstr += "'";
	cout << sqlstr << endl;
	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");
	//打印获取的数据  


	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}

	return vec_result;

}
//select u_id,u_time,u_X from 转台_2022_10_09_09_39_43_545;
vector<vector<string>> ConnectMysql::mytest_QueryByColumnName(string name_Table,vector<string> vec)
{
	vector<vector<string>> vec_result;
	MYSQL_RES *res; //这个结构代表返回行的一个查询结果集  
	MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列 
	char current_c[150]; //查询语句
	string sqlstr,outPut;

	//char id[20];
	//string string_id(id);
	sqlstr = "select ";
	for(int i = 0; i < vec.size() - 1; i++)
	{
		sqlstr += vec[i];
		sqlstr += ",";
	}
	sqlstr += vec[vec.size() - 1];
	sqlstr += " from ";
	sqlstr += name_Table;
	sqlstr += ";";
	//sqlstr += "u_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY COMMENT 'Unique User ID'";

	//sprintf_s(query, "select * from t_dept"); //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以  
	//mysql_query(&mydata, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
	//返回0 查询成功，返回1查询失败  
	if (mysql_query(&m_mydata, sqlstr.c_str()))    //执行SQL语句
	{
		printf("Query failed (%s)\n", mysql_error(&m_mydata));
		//return false;
	}
	else
	{
		printf("query success\n");
	}
	//获取结果集  
	if (!(res = mysql_store_result(&m_mydata)))   //获得sql语句结束后返回的结果集  
	{
		printf("Couldn't get result from %s\n", mysql_error(&m_mydata));
		//return false;
	}
	unsigned int fieldcount_select = mysql_num_fields(res);
	//unsigned int affect_row = mysql_affected_rows(&m_mydata);
	//打印数据行数  
	printf("number of dataline returned: %d\n", mysql_affected_rows(&m_mydata));//

	//获取字段的信息  
	char *str_field[40];  //定义一个字符串数组存储字段信息  
	for (int i = 0; i < fieldcount_select; i++)  //在已知字段数量的情况下获取字段名  
	{
		str_field[i] = mysql_fetch_field(res)->name;
	}
	for (int i = 0; i < fieldcount_select; i++)  //打印字段  
		printf("%10s\t", str_field[i]);
	printf("---------------------------\n");

	while (column = mysql_fetch_row(res))   //在已知字段数量情况下，获取并打印下一行  
	{
		vector<string> vec;
		for (int j = 0; j < fieldcount_select; j++)
		{
			//vector<string> vec;
			sprintf(current_c, "%s", column[j]);
			string result(current_c);
			vec.push_back(result);
		}
		vec_result.push_back(vec);

	}

	return vec_result;
}