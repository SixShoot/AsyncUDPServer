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

	exoModule(boost::asio::io_context& io_context, std::string str_ip, unsigned short port, std::string name);

	boost::asio::ip::address GetIpAddress();
	unsigned short GetPort();

	int GetNumberReceivPack();
	std::string GetName();
	std::string GetStringConnectStatus();
	bool GetConnectStatus();

	void Check_TimeOut();

	void UpdatePack();
	int GetFPS();

	
	void ModuleService();

	

private:
	boost::asio::ip::address ip;
	unsigned short port_;
	std::string name_;

	boost::asio::deadline_timer Timer1;
	boost::asio::deadline_timer Timer_TimeOut;	

	int FPS;

	int NumberReceivPack;

	bool ConnectionStatus;
};