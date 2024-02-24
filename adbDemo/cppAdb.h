#pragma once
#include <WinSock2.h>
#include <string>
#include <random>
namespace Ctsdmm
{
	class cppAdb
	{
	public:
		cppAdb();
		~cppAdb();
		
		bool Connect(const std::string& port);
		bool TapPoint(const const double& X, const double& Y);
		bool TapPoint(std::string& port, const double& X, const double& Y);

		bool TapPoint(const double& X, const double& Y, const double& randomSize);
		bool TapPoint(std::string& port, const double& X, const double& Y, const double &randomSize);

		void GetDevices();
	private:
	
		//random
		u_long mode{ 1 };
		std::random_device rd;
		std::mt19937 gen;
		const char* sendbuf{nullptr};
		char recvbuf[512]{0};
		int recvbuflen = 512;
		struct addrinfo* result{nullptr}, hints{0};
		SOCKET _mSocket{ INVALID_SOCKET };
		bool SocketSend(const std::string& cmd, bool SocketChange=true);
		const std::string SocketSendAndRecv(const std::string& cmd);
		bool initSocket();
		int iResult{ 0 };
		bool ConnectSocket();
		void CloseSocket();
		void RecvThread();
	};

}
