#pragma once

#include <sys/socket.h>

#include "CoreMinimal.h"

class AMaze;

class ServerSocketClient
{
private:
	int Sockfd;
	static ServerSocketClient* Singleton;
public:
	static void* Get_In_Addr(sockaddr* Sa);
	int Connect();
	const FString MazeCode = "2007";
	void SendStartCommand() const;
	void SendMaze(const AMaze* Maze) const;
	void SendPositions(const AMaze* Maze) const;
	void CloseSocket() const;
	void SendMessage(const FString Message) const;
	std::tuple<FString, int> ReadMessage() const;
	static ServerSocketClient* GetServerSocketClient();
};
