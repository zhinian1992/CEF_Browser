#include "stdafx.h"
#include "ConfigOperator.h"

using namespace std;


ConfigOperator::ConfigOperator()
{
}


ConfigOperator::~ConfigOperator()
{
}

string ConfigOperator::GetConfigKeyStr(Config_Type parameterName) {
	switch (parameterName) {
		case HOMEURL:
		return "HOMEURL";
		default:
		return "";
	}
}

int ConfigOperator::GetConfigValue(Config_Type parameterType, char * value)
{
	CHAR szFilePath[256]={0};
	GetCurrentDirectoryA(256,szFilePath);

	string configPath = szFilePath;
	configPath.append("\\config.ini");
	string sSegment = "config";
	string sKey = GetConfigKeyStr(parameterType);
	::GetPrivateProfileStringA(sSegment.c_str(), sKey.c_str(), NULL, value, 1024, configPath.c_str());
	if (strlen(value) == 0)
		return -1;
	return 0;
}
