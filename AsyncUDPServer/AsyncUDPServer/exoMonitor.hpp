#pragma once

#include "exoModule.h"
#include "exoActuator.hpp"



class exoMonitor
{
public:

	//------------------------------------------------------------------------------
	exoMonitor()
	{	
		
	}
	//------------------------------------------------------------------------------
	void SendData(exoActuator actuators[],uint32_t t)
	{
		for (int i = 0; i < ActuatorSize; i++)
		{
			PC->server_pack.set<float>(CurrentPosition_Handle[i], actuators[i].GetCurrentPosition());
			PC->server_pack.set<float>(TargetPosition_Handle[i],  actuators[i].GetTargetPosition());
		}

		PC->server_pack.set<uint32_t>(CurrentTime_Handle, t);
	}
	//------------------------------------------------------------------------------
	void Init(exoModule& module_)
	{
		PC = &module_;

		std::string smName[10] = { "A", "B","C" ,"D" ,"E" ,"F" ,"G" ,"H" ,"I" ,"J" };

		for (int i = 0; i < ActuatorSize; i++)
		{
			CurrentPosition_Handle[i] = PC->server_pack.init<float>((smName[i] + "CP").c_str());
			TargetPosition_Handle[i] = PC->server_pack.init<float>((smName[i] + "TP").c_str());
		}

		CurrentTime_Handle = PC->server_pack.init<uint32_t>("TIME");
	}
	//-------------------------------------------------------------------
	~exoMonitor()
	{

	}

private:
	exoModule* PC;
	uint8_t CurrentPosition_Handle[ActuatorSize];
	uint8_t TargetPosition_Handle[ActuatorSize];
	uint8_t CurrentTime_Handle;
};
