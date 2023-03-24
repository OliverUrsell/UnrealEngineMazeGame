#pragma once
#include "ServerSocketClient.h"

class MockServerSocketClient: ServerSocketClient
{
public:
	using ServerSocketClient::Singleton;
	int Connect();
	bool IsConnected() const;
	void CloseSocket() const;
	void SendMessage(const FString Message) const;
	std::tuple<FString, int> ReadMessage();
	MockServerSocketClient();
}; 
