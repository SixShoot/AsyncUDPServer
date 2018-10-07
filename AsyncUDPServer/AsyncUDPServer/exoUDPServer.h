#pragma once


#define SIZE_BUFF 1024

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "exoModule.h"
#include "DataPackage.h"
#include <plog/Log.h>

using boost::asio::ip::udp;

class exoUDPServer
{
public:
	exoUDPServer(boost::asio::io_context& io_context, std::vector<exoModule*>& exoModules, unsigned short port);

private:
	void start_receive();

	void handle_receive(const boost::system::error_code& error, size_t bytes_recvd);

	void start_send();

	void handle_send(const boost::system::error_code& error, size_t bytes_sent);

	udp::socket socket_;
	udp::endpoint remote_endpoint_;


	char recv_pack[SIZE_BUFF];
	char send_pack[SIZE_BUFF];
	size_t send_size;



	std::vector<exoModule*>& exoModules_;

	
};
