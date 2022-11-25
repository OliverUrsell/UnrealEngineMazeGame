#pragma once

#include <sys/socket.h>

#include "CoreMinimal.h"

class AMaze;

class ServerSocketClient
{
private:
	int Sockfd;
public:
	static void* Get_In_Addr(sockaddr* Sa);
	ServerSocketClient();
	void SendStartCommand(const FString Code, const AMaze* Maze) const;
	void CloseSocket() const;
	void SendMessage(const FString Message) const;
	FString ReadMessage() const;
};
