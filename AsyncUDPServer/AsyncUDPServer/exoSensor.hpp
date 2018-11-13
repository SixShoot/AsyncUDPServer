#pragma once

#include <cstdlib>
#include <iostream>
#include "exoModule.h"


class exoSensor
{
public:

	//-------------------------------------------------------------------------------
	exoSensor()
	{

	}

	//-------------------------------------------------------------------------------
	void SetModule(exoModule& module)
	{
		module_ = &module;
	}
	//-------------------------------------------------------------------------------
	void SetName(std::string name)
	{
		name_ = name;
	}
	//-------------------------------------------------------------------------------
	uint16_t GetValue()
	{
		return module_->client_pack.get<uint16_t>(name_.c_str(), 0);
	}
	//-------------------------------------------------------------------------------
	~exoSensor()
	{

	}
	//-------------------------------------------------------------------------------
private:

	std::string name_;
	exoModule* module_;
};
