#define _WIN32_WINNT 0x0501

#include "exoMotor.h"

exoMotor::exoMotor(std::string Name, exoModule& module) : module_(module)
{
	name_ = Name;

	PWM_Handle = module_.server_pack.init<uint8_t>((name_ + "_PWM").c_str());
	INA_Handle = module_.server_pack.init<uint8_t>((name_ + "_INA").c_str());
	INB_Handle = module_.server_pack.init<uint8_t>((name_ + "_INB").c_str());

	SetPWM(0);
	SetDirection(0, 0);
}

void exoMotor::SetPWM(uint8_t pwm)
{
	module_.server_pack.set2<uint8_t>(PWM_Handle, pwm);
}

void exoMotor::SetDirection(uint8_t INA, uint8_t INB)
{
	module_.server_pack.set2<uint8_t>(INA_Handle, INA);
	module_.server_pack.set2<uint8_t>(INB_Handle, INB);
}

std::string exoMotor::GetName()
{
	return name_;
}


exoMotor::~exoMotor()
{

}