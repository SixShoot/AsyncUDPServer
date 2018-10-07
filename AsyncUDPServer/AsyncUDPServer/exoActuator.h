#pragma once

#include <cstdlib>
#include <iostream>
//#include "exoModule.h"


class exoActuator
{
public:

	exoActuator(std::string Name);

	void SetTargetPosition(float angle);
	void SetDirection(char INA, char INB);
	float GetAngle();
	void SetPWM(float pwm);
	float GetPosition();
	std::string GetName();

	~exoActuator();

private:
	std::string name_;
	
	
};