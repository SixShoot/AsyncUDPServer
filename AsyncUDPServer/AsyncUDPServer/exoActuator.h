#pragma once

#include <cstdlib>
#include <iostream>
#include "exoMotor.h"
#include "exoSensor.h"


class exoActuator
{
public:

	exoActuator(std::string Name, exoMotor& motor, exoSensor& sensor);

	void SetTargetPosition(float angle);
	uint16_t GetCurrentPosition();
	std::string GetName();

	~exoActuator();

private:
	std::string name_;

	exoMotor&  motor_;
	exoSensor& sensor_;



	float Kp;
	
	
};