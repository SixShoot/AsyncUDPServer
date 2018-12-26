#pragma once

#include "exoModule.h"


class exoMonitor
{
public:

	//------------------------------------------------------------------------------
	exoMonitor()
	{

	}
	//------------------------------------------------------------------------------
	void Init(exoModule& module_)
	{
		PC = &module_;
	}
	//-------------------------------------------------------------------
	~exoMonitor()
	{

	}

private:
	exoModule* PC;

};
