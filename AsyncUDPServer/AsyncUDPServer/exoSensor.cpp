#include "exoSensor.h"

exoSensor::exoSensor(std::string Name, exoModule& module) : module_(module)
{
	name_ = Name;
}


uint16_t exoSensor::GetValue()
{
	return module_.client_pack.get<uint16_t>(name_.c_str(), 0);
}


exoSensor::~exoSensor()
{

}