#include "exoSensor.h"

exoSensor::exoSensor(std::string Name, exoModule& module) : module_(module)
{
	name_ = Name;
}


float exoSensor::GetValue()
{
	return module_.client_pack.get<float>(name_.c_str(), 0);
}


exoSensor::~exoSensor()
{

}