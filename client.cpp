#include <stdio.h>
#include "winsock2.h"
#include <iostream>

int main()
{
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		std::cout << "Error at WSAStartup()\n";

	// Create a socket.
	SOCKET m_socket;
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket == INVALID_SOCKET)
	{
		std::cout << "Error at socket()\n";
		WSACleanup();
		return 1;
	}

	// Connect to a server.
	struct sockaddr_in clientService;

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("192.168.56.104");
	clientService.sin_port = htons(3425);

	if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		std::cout << "Failed to connect.\n";
		WSACleanup();
		return 1;
	}

	// Send and receive data.
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[32] = "Client: Sending data.";
	char recvbuf[32] = "";

	bytesSent = send(m_socket, sendbuf, strlen(sendbuf), 0);

	while (1)
	{
		bytesRecv = recv(m_socket, recvbuf, 32, 0);
		if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
		{
			std::cout << "Connection Closed.\n" << std::endl;
			break;
		}
		if (bytesRecv < 0)
			return 1;
		std::cout << "[server]" << recvbuf << std::endl;
	}
	system("pause");
	return 0;
}
