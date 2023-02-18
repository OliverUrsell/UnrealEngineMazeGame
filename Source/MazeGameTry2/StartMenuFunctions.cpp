
#include "StartMenuFunctions.h"

#include "ServerSocketClient.h"

/**
 * @brief Connect to the server
 * @return True if there was an error connecting, false if connection was successful
 */
bool UStartMenuFunctions::ConnectToServer()
{
	ServerSocketClient* SC = ServerSocketClient::GetServerSocketClient();
	if(SC->Connect() != 0)
	{
		// The socket failed to connect
		return true;
	}
	SC->SendStartCommand();
	return false;
}

/**
 * @brief Get whether we are current connected to the server
 * @return True if we are connected to the server, false if not
 */
bool UStartMenuFunctions::IsConnected()
{
	return ServerSocketClient::GetServerSocketClient()->IsConnected();
}

FString UStartMenuFunctions::GetCode()
{
	return ServerSocketClient::GetServerSocketClient()->MazeCode;
}
