#include "MockServerSocketClient.h"

MockServerSocketClient::MockServerSocketClient()
{
	// Set the singleton to this class so that it is what is returned by ServerSocketClient::GetServerSocketClient
	Singleton = this;
}

int MockServerSocketClient::Connect()
{
	return 0;
}

bool MockServerSocketClient::IsConnected() const
{
	return true;
}

void MockServerSocketClient::CloseSocket() const
{
}

void MockServerSocketClient::SendMessage(const FString Message) const
{
}

std::tuple<FString, int> MockServerSocketClient::ReadMessage()
{
	return {FString(""),0};
}
