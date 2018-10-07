#pragma once

#include <cstdlib>
#include <iostream>
#include "exoModule.h"


class exoSensor
{
public:

	exoSensor(std::string Name, exoModule& module);

	float GetValue();

	~exoSensor();

private:

	std::string name_;
	exoModule& module_;
};
