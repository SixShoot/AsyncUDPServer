#include "exoMotor.h"

exoMotor::exoMotor(std::string Name, exoModule& module) : module_(module)
{
	name_ = Name;

	SetPWM(0);
	SetDirection(0, 0);
}

void exoMotor::SetPWM(uint8_t pwm)
{
	//module_.server_pack.set<uint8_t>(name_+"PWM", pwm);
}

void exoMotor::SetDirection(uint8_t INA, uint8_t INB)
{
	//module_.server_pack.set<uint8_t>(name_ + "INA", INA);
	//module_.server_pack.set<uint8_t>(name_ + "INB", INB);
}

std::string exoMotor::GetName()
{
	return name_;
}


exoMotor::~exoMotor()
{

}