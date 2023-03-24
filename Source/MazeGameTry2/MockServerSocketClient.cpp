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
	UE_LOG(LogTemp, Display, TEXT("Here in the test method lalalala"));
}

std::tuple<FString, int> MockServerSocketClient::ReadMessage()
{
	return {FString(""),0};
}
