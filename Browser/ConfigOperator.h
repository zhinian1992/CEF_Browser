#pragma once
#include <string>

typedef enum
{
	HOMEURL = 0

}Config_Type;

class ConfigOperator
{
public:
	ConfigOperator();
	~ConfigOperator();

	int GetConfigValue(Config_Type parameterType,char *value);

private:
	std::string GetConfigKeyStr(Config_Type parameterName);
};

