#include "cppAdb.h"
#include "adbCommandHex.h"

#include <Windows.h>
#include <ws2tcpip.h>
#include <iostream>

#include <format>
namespace Ctsdmm
{
	cppAdb::cppAdb()
	{
		// 首先要打开本地的adb服务
		//system("adb start-server");
		gen = std::mt19937(rd());

		initSocket();
	}
	cppAdb::~cppAdb()
	{
		// cleanup
		closesocket(_mSocket);
		WSACleanup();
	}
	
	bool cppAdb::SocketSend(const std::string& cmd,bool SocketChange)
	{
		if (SocketChange || _mSocket==INVALID_SOCKET)
		{
			ConnectSocket();
		}
		// 不重新连接socket多次send后recv可能会被阻塞住
	/*	if (!SocketChange)
		{
			ioctlsocket(_mSocket, FIONBIO, &mode);
		}*/
		iResult = send(_mSocket, cmd.c_str(), cmd.size(), 0);
		Sleep(50);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(_mSocket);
			WSACleanup();
			return false;
		}
	/*	do
		{
			memset(recvbuf, 0, recvbuflen);
			iResult = recv(_mSocket, recvbuf, recvbuflen, 0);
			if (iResult>0)
			{
				std::cout << "size : " << iResult << "\n" << std::string(recvbuf) << std::endl;
			}
		} while (iResult>0);*/

		if (SocketChange)
		{
			CloseSocket();
		}
	}

	bool cppAdb::initSocket()
	{
		WSADATA wsaData;
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}
	
		
		//ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo("127.0.0.1", "5037", &hints, &result);
		if (iResult != 0) {
			std::cout << "getaddrinfo failed:" <<iResult<<std::endl;
			WSACleanup();
			return false;
		}
		return true;
	}

	bool cppAdb::ConnectSocket()
	{
		_mSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (_mSocket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}
		// Connect to server.
		iResult = connect(_mSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			closesocket(_mSocket);
			_mSocket = INVALID_SOCKET;
			return false;
		}
	}
	
	void cppAdb::CloseSocket()
	{
		shutdown(_mSocket, SD_SEND);
		_mSocket = INVALID_SOCKET;
	}

	const std::string cppAdb::SocketSendAndRecv(const std::string& cmd)
	{
		
		
			ConnectSocket();
		

		iResult = send(_mSocket, cmd.c_str(), cmd.size(), 0);
		Sleep(50);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(_mSocket);
			WSACleanup();
			
		}
		do
		{
			memset(recvbuf, 0, recvbuflen);
			iResult = recv(_mSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				std::cout << "size : " << iResult << "\n" << std::string(recvbuf) << std::endl;
			}
		} while (iResult > 0);

			CloseSocket();

			return std::string(recvbuf);
	
	}

	void cppAdb::RecvThread()
	{

	}

	bool cppAdb::Connect(const std::string& port)
	{
		ConnectSocket();

		const char* sendbuf = "000chost:version";
		iResult = send(_mSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(_mSocket);
			WSACleanup();
			return false;
		}


		CloseSocket();
		// 重新连接
		ConnectSocket();


		sendbuf = "001chost:connect:127.0.0.1:58526";
		iResult = send(_mSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(_mSocket);
			WSACleanup();
			return false;
		}

			CloseSocket();
	}

	bool cppAdb::TapPoint(const double& X, const double& Y)
	{
		SocketSend(adbHexHostHeaderOneStr);
		SocketSend(adbHexHostHeaderTwoAnyPortStr);
		// 不会改Socket
		SocketSend(adbHexAnyPortStr,false);
		SocketSend(adbHexTap(X,Y), false);
		SocketSend(adbHexEndStr,false);
		return true;
	}
	bool cppAdb::TapPoint(const double& X, const double& Y, const double& randomSize)
	{
		std::uniform_real_distribution<double>distX(X - randomSize, X + randomSize);
		std::uniform_real_distribution<double>distY(Y - randomSize, Y + randomSize);
		TapPoint(distX(gen), distY(gen));
		return true;
	}

	bool cppAdb::TapPoint(std::string& port, const double& X, const double& Y)
	{
		SocketSend(adbHexHostHeaderOneStr);
		SocketSend(adbHexHostHeaderTwoSelectPorttPort(port));
		// 不会改Socket
		SocketSend(adbHexSelectPort(port), false);
		SocketSend(adbHexTap(X, Y), false);
		SocketSend(adbHexEndStr, false);
		return true;
	}
	bool cppAdb::TapPoint(std::string& port, const double& X, const double& Y, const double& randomSize)
	{
		std::uniform_real_distribution<double>distX(X - randomSize, X + randomSize);
		std::uniform_real_distribution<double>distY(Y - randomSize, Y + randomSize);
		TapPoint(port, distX(gen), distY(gen));
		return true;
	}

	void cppAdb::GetDevices()
	{
		SocketSend(adbHexHostHeaderOneStr);
		auto z = 	SocketSendAndRecv(adbHexDevices);

	}


}