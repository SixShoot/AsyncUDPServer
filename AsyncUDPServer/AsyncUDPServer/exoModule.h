#pragma once

#include <cstdlib>
#include <iostream>

#include <string>
#include "DataPackage.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "exoPackage.hpp"
#include <plog/Log.h>


class exoModule
{
public:

	// Create a root
	exo::exoPackage client_pack;
	exo::exoPackage server_pack;

	void Timer1_Hendle(const boost::system::error_code& /*e*/, boost::asio::deadline_timer* t);

	exoModule(boost::asio::io_context& io_context, std::string str_ip, std::string name);

	boost::asio::ip::address GetIpAddress();

	int GetNumberReceivPack();
	std::string GetName();
	std::string GetStringConnectStatus();

	void Check_TimeOut();

	void UpdatePack();

	
	void ModuleService();

	int FPS;

private:
	boost::asio::ip::address ip;
	std::string name_;

	boost::asio::deadline_timer Timer1;
	boost::asio::deadline_timer Timer_TimeOut;	

	int NumberReceivPack;

	bool ConnectionStatus;
};