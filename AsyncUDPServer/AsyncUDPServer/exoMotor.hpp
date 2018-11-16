#pragma once


#include <cstdlib>
#include <iostream>
#include "exoModule.h"


class exoMotor
{
public:

	exoMotor() 
	{
	}

	
	void Init(std::string name , exoModule& module)
	{
		SetModule(module);
		SetName(name);

		PWM_Handle = module_->server_pack.init<uint8_t>((name_ + "_PWM").c_str());
		INA_Handle = module_->server_pack.init<uint8_t>((name_ + "_INA").c_str());
		INB_Handle = module_->server_pack.init<uint8_t>((name_ + "_INB").c_str());

		SetPWM(0);
		SetDirection(0, 0);

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

	void SetPWM(uint8_t pwm)
	{
		module_->server_pack.set<uint8_t>(PWM_Handle, pwm);
	}

	void SetDirection(uint8_t INA, uint8_t INB)
	{
		module_->server_pack.set<uint8_t>(INA_Handle, INA);
		module_->server_pack.set<uint8_t>(INB_Handle, INB);
	}

	std::string GetName()
	{
		return name_;
	}

	~exoMotor()
	{

	}

private:

	std::string name_;
	exoModule* module_;

	uint8_t PWM_Handle;
	uint8_t INA_Handle;
	uint8_t INB_Handle;

};
