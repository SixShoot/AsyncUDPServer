#define _WIN32_WINNT 0x0501

#include "exoUDPServer.h"


exoUDPServer::exoUDPServer(boost::asio::io_context& io_context, std::vector<exoModule*>& exoModules, unsigned short port)
	: socket_(io_context, udp::endpoint(udp::v4(), port)), exoModules_(exoModules)
{
	//std::cout << "Server started.... PORT: " << port << std::endl;
	LOGD << "Starting asynchronous UDP server. Port: " << port;
	start_receive();
	send_size = 0;
}
//-------------------------------------------------------------------------------------------

void exoUDPServer::start_receive()
{
	socket_.async_receive_from(boost::asio::buffer(boost::asio::buffer(recv_pack)), remote_endpoint_,
		boost::bind(&exoUDPServer::handle_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
//-------------------------------------------------------------------------------------------

void exoUDPServer::handle_receive(const boost::system::error_code& error, size_t bytes_recvd)
{
	if (!error && bytes_recvd > 0)
	{
		//std::string str = remote_endpoint_.address().to_string();
		bool Ident = false;
		for (int i = 0; i < exoModules_.size(); i++)
		{
			if (exoModules_[i]->GetIpAddress() == remote_endpoint_.address())
			{
				exoModules_[i]->UpdatePack();

				boost::recursive_mutex::scoped_lock lk(exoModules_[i]->client_pack.cs);
				boost::recursive_mutex::scoped_lock lk2(exoModules_[i]->server_pack.cs);

				memcpy(exoModules_[i]->client_pack.buff, recv_pack, bytes_recvd);
				exoModules_[i]->client_pack.SetLength(bytes_recvd);

				memcpy(send_pack, exoModules_[i]->server_pack.buff, exoModules_[i]->server_pack.length());
				send_size = exoModules_[i]->server_pack.length();

				start_send();
				Ident = true;
				break;
			}
		}

		if (Ident == false) // Если не удалось идентифицировать устройство
		{
			// Использыется ли V-REP.. Если до то игнорируем любые внешние устройства
			#ifdef USE_VREP
				start_receive();
			#else
				std::cout << "Unknown device... IP: " << remote_endpoint_.address().to_string() << std::endl; //
				LOGE << "Unknown device... IP: " << remote_endpoint_.address().to_string();
			#endif // USE_VREP

		}

	}
	else
	{
		LOGW << "Could not read UDP message. Error: " << error.message() << " bytes_recvd: " << bytes_recvd;
		//start_receive();
	}
}
//-------------------------------------------------------------------------------------------

void exoUDPServer::start_send()
{
	socket_.async_send_to(boost::asio::buffer(send_pack, send_size), remote_endpoint_,
		boost::bind(&exoUDPServer::handle_send, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}
//-------------------------------------------------------------------------------------------

void exoUDPServer::handle_send(const boost::system::error_code& error, size_t bytes_sent)
{
	if (!error)
	{
		start_receive();
	}
	else
	{
		LOGW << "Could not send UDP message. Error: " << error.message() << " bytes_sent: " << bytes_sent;
		start_receive();
	}

	
}