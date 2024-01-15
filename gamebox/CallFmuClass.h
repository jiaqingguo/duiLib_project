#pragma once
//调用fmu类

#include <iostream> 
#include <fmi4cpp/fmi4cpp.hpp>
using namespace fmi4cpp;
using namespace std;
const double stop = 10;
const double stepSize = 1;

class CallFmuClass
{
public:
	CallFmuClass();
	~CallFmuClass();

	string UTF8_To_string(const std::string& str);
	string string_To_UTF8(const std::string& str);
	void InitializeVariables(void);//初始化变量 将输入和输出在fmu中xml中对应的序号填上
	vector<double> CallTheFmuInterface(string path,vector<double> vec_inputData);




	std::vector<fmi2ValueReference> vec_InputSequenceNumber;//存放输入序号
	std::vector<fmi2ValueReference> vec_OnputSequenceNumber;//存放输出序号
private:

};

