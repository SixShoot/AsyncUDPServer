#include "exoActuator.h"


float map(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//----------------------------------------------------------------
exoActuator::exoActuator(std::string Name, exoMotor& motor, exoSensor& sensor) : motor_(motor), sensor_(sensor)
{
	name_ = Name;

	Kp = 1;
}
//----------------------------------------------------------------
void exoActuator::SetTargetPosition(float angle)
{
	float error = (GetCurrentPosition() - angle);

	if (error < 0)  motor_.SetDirection(1, 0);
	if (error > 0)  motor_.SetDirection(0, 1);
	if (error == 0) motor_.SetDirection(1, 1);

	float U = fabs(error) * Kp;

	if (U > 100) U = 100;

	motor_.SetPWM(U);
}
//----------------------------------------------------------------
uint16_t exoActuator::GetCurrentPosition()
{
	return map(sensor_.GetValue(),1,1,1,10);
}
//---------------------------------------------------------------
std::string exoActuator::GetName()
{
	return name_;
}

exoActuator::~exoActuator()
{

}
