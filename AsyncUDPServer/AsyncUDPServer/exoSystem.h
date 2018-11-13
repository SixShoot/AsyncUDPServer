#pragma once

#include <cstdlib>
#include <iostream>
#include <plog/Log.h>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>


#include "exoActuator.h"


//extern std::vector<exoModule*> exoModules;
//extern std::vector<exoActuator*> exoActuators;



class exoSystem
{
public:

	exoSystem();


	void ControlFlow(int64_t t);


	~exoSystem();

private:

	exoModule* pNucleo;

};
