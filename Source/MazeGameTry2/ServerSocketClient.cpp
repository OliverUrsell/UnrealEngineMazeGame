#include "ServerSocketClient.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include "Maze.h"

#define PORT "25566" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void* ServerSocketClient::Get_In_Addr(sockaddr *Sa)
{
    if (Sa->sa_family == AF_INET) {
        return &((sockaddr_in*)Sa)->sin_addr;
    }

    return &((sockaddr_in6*)Sa)->sin6_addr;
}

int ServerSocketClient::Connect()
{
    // Returns 0 on success, 1 on error
    addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
        UE_LOG(LogTemp, Error, TEXT("Failed to run getaddrinfo"));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((this->Sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            UE_LOG(LogTemp, Error, TEXT("Failed to create socket to server"));
            continue;
        }

        if (connect(this->Sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(this->Sockfd);
            UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
            continue;
        }

        break;
    }

    if (p == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("Failed to connect to server"));
        
        // The server has failed to connect
        return 1;
    }

    inet_ntop(p->ai_family, Get_In_Addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    UE_LOG(LogTemp, Display, TEXT("Connected to server"));

    // Put the socket in non-blocking mode
    if(fcntl(Sockfd, F_SETFL, fcntl(Sockfd, F_GETFL) | O_NONBLOCK) < 0) {
        UE_LOG(LogTemp, Error, TEXT("Failed to put socket in non-blocking mode"));
        return 1;
    }

    freeaddrinfo(servinfo); // all done with this structure

    this->Connected = true;
    
    return 0;
}

bool ServerSocketClient::IsConnected() const
{
    return this->Connected;
}

void ServerSocketClient::Disconnected()
{
    this->Connected = false;
}

void ServerSocketClient::SendStartCommand() const
{
    // Tell the server about this maze
    this->SendMessage(FString("StartGame ") + this->MazeCode);
}

void ServerSocketClient::SendMaze(const AMaze* Maze) const
{
    this->SendMessage(this->MazeCode + " Maze " + Maze->ToJSONString());
}

void ServerSocketClient::SendPositions(const AMaze* Maze) const
{
    this->SendMessage(FString(this->MazeCode + " Positions ") + Maze->GetPositionsString());
}

void ServerSocketClient::CloseSocket() const
{
    close(this->Sockfd);
}

void ServerSocketClient::SendMessage(const FString Message) const
{
    // Add a new line as a message delimiter 
    FString MessageToSend = Message + FString("\n");
    //TODO: This might not send the whole message, see https://beej.us/guide/bgnet/html/#sendrecv about how send works
    send(this->Sockfd, TCHAR_TO_ANSI(*MessageToSend), MessageToSend.Len(), 0);
}

std::tuple<FString, int> ServerSocketClient::ReadMessage()
{
    // Returns an error code, 0 if success, 1 if server disconnected, 2 on failure
    ssize_t numbytes;
    ANSICHAR buf[MAXDATASIZE];
    if ((numbytes = recv(Sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
            // There was no data to read
            return {FString(""), 0};
        }

        // There was an error reading the data
        perror("recv");
        return {FString(""), 2};
    }

    if(numbytes == 0)
    {
        // The server has disconnected
        Disconnected();
        return {FString(""), 1};
    }

    buf[numbytes] = '\0';
    
    UE_LOG(LogTemp, Display, TEXT("Received string \"%s\" with length - %d"), buf, numbytes);
    
    return {FString(buf).TrimStartAndEnd(), 0};
}

ServerSocketClient* ServerSocketClient::GetServerSocketClient()
{
    if(Singleton == nullptr) {
        Singleton = new ServerSocketClient();
    }
    
    return Singleton;
}

ServerSocketClient* ServerSocketClient::Singleton = nullptr;