#pragma once
//����fmu��

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
	void InitializeVariables(void);//��ʼ������ ������������fmu��xml�ж�Ӧ���������
	vector<double> CallTheFmuInterface(string path,vector<double> vec_inputData);




	std::vector<fmi2ValueReference> vec_InputSequenceNumber;//����������
	std::vector<fmi2ValueReference> vec_OnputSequenceNumber;//���������
private:

};

