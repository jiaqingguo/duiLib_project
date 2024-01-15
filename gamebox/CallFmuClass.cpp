#include "stdafx.h"
#include "CallFmuClass.h"




CallFmuClass::CallFmuClass()
{
	//std::vector<fmi2Real> wef(1812);
//std::vector<fmi2Real> ref(100);
	InitializeVariables();

}
CallFmuClass::~CallFmuClass()
{

}

string CallFmuClass::UTF8_To_string(const std::string& str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;

}

string CallFmuClass::string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

void CallFmuClass::InitializeVariables(void)
{
	for (int i = 109; i < 1921; i++)
	{
		vec_InputSequenceNumber.push_back(i);
	}


	for (int i = 1921; i < 2021; i++)
	{
		vec_OnputSequenceNumber.push_back(i);
	}
}

vector<double> CallFmuClass::CallTheFmuInterface(string path, vector<double> vec_inputData)
{
	vector<fmi2Real> vec_inputValue;//要给fmu输入的值
	vector<fmi2Real> vec_outputValue(100);//输出
	vector<double> vec_outPut;

	//string exe_config_paths = "D:\\0_TwoDimensional\\atspaceNet.fmu";
	const std::string fmu_path = string_To_UTF8(path);
	fmi2::fmu fmu(fmu_path);
	auto cs_fmu = fmu.as_cs_fmu();
	auto md = cs_fmu->get_model_description();
	auto slave1 = cs_fmu->new_instance();
	slave1->setup_experiment();
	slave1->enter_initialization_mode();
	slave1->exit_initialization_mode();

	for (int i = 0; i < 1812; i++)
	{
		vec_inputValue.push_back(vec_inputData[i]);
	}

	if (!slave1->write_real(vec_InputSequenceNumber, vec_inputValue))
	{
		cout << "111111111111" << endl;
	}
	else
	{
		cout << "111111111111" << endl;
	}
	if (!slave1->step(stepSize))
	{
		cout << "111111111111" << endl;
	}
	else
	{
		cout << "111111111111" << endl;
	}
	if (!slave1->read_real(vec_OnputSequenceNumber, vec_outputValue))
	{
		cout << "1111111111" << endl;
	}
	else
	{

		for (int i = 0; i < 100; i++)
		{
			cout << vec_outputValue[i] << " ";
			vec_outPut.push_back(vec_outputValue[i]);
		}
		cout << endl;
	}

	return vec_outPut;
}