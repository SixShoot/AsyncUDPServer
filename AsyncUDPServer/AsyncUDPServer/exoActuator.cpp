#include "exoActuator.h"

//----------------------------------------------------------------
exoActuator::exoActuator(std::string Name)
{
	name_ = Name;
}
//----------------------------------------------------------------
void exoActuator::SetTargetPosition(float angle)
{

}
//----------------------------------------------------------------

float exoActuator::GetAngle()
{
	return 0;
}

//----------------------------------------------------------------
float exoActuator::GetPosition()
{
	return 0;
}
//----------------------------------------------------------------
void exoActuator::SetPWM(float pwm)
{

}
//----------------------------------------------------------------

void exoActuator::SetDirection(char INA, char INB)
{

}
//---------------------------------------------------------------
std::string exoActuator::GetName()
{
	return name_;
}

exoActuator::~exoActuator()
{

}
