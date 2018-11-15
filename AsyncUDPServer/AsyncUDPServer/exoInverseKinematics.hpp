#pragma once
#include "exoSolver.hpp"

class exoInverseKinematics : public exoSolver
{
public:
	
	//------------------------------------------------------------------------------
	exoInverseKinematics()
	{

	}
	//------------------------------------------------------------------------------
	void GetCurrentAngles(exoActuator actuators[], const int size, float t)
	{
		for (int i = 0; i < size; i++)
		{
			actuators[i].SetTargetPosition(0);
		}
	}
	//-------------------------------------------------------------------
	~exoInverseKinematics()
	{

	}

private:


};
