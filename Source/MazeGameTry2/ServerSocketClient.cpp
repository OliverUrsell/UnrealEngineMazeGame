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

ServerSocketClient::ServerSocketClient()
{
    addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
        UE_LOG(LogTemp, Error, TEXT("Failed to run getaddrinfo"));
        return;
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
        return;
    }

    inet_ntop(p->ai_family, Get_In_Addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    UE_LOG(LogTemp, Display, TEXT("Connected to server"));

    freeaddrinfo(servinfo); // all done with this structure
}

void ServerSocketClient::SendStartCommand(const FString Code, const AMaze* Maze) const
{
    // Tell the server about this maze
    this->SendMessage(FString("StartGame ") + Code + FString(" ") + Maze->ToJSONString());    
}

void ServerSocketClient::SendPositions(const AMaze* Maze) const
{
    this->SendMessage(FString("Positions ") + Maze->GetPositionsString());
}

void ServerSocketClient::CloseSocket() const
{
    close(this->Sockfd);
}

void ServerSocketClient::SendMessage(const FString Message) const
{
    // Add a new line as a message delimiter 
    FString MessageToSend = Message + FString("\n");
    UE_LOG(LogTemp, Log, TEXT("Sending Message... %s"), *Message);
    send(this->Sockfd, TCHAR_TO_ANSI(*MessageToSend), MessageToSend.Len(), 0);
}

FString ServerSocketClient::ReadMessage() const
{
    int numbytes;
    char buf[MAXDATASIZE];
    if ((numbytes = recv(Sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    
    buf[numbytes] = '\0';
    
    return FString(buf);
}
