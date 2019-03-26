#include <sys/types.h>
#include <sys/socket.h> 	// API and functionality for creating sockets
#include <netinet/in.h> 	// Structures storing address information
#include <errno.h>
#include <unistd.h>         // contains close() function
#include <stdlib.h>         // contains exit() function
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>



#include "cksettings.h"
#include "ckutils.h"




int createSocket(int type)
{
	errno = 0;
	int networkSocket = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM

    if (networkSocket == -1)
	{
        logMsg(1, "Error creating socket");
        exit(EXIT_FAILURE);
	}
    else
    {
        logMsg(5, "Created socket");
    }


    return networkSocket;
}

void closeSocket (int networkSocket)
{
    errno = 0;
	if(close(networkSocket) == -1)
	{
		logMsg(1, "Cannot close the net socket");
        exit(EXIT_FAILURE);
	}
    else
    {
        logMsg(4, "Socket closed");
    }

    return;
}

struct sockaddr_in defineServerAddress(int serverIP, int serverPort)
{
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));

	serverAddress.sin_family        = AF_INET;
    serverAddress.sin_port          = serverPort;
    serverAddress.sin_addr.s_addr   = serverIP;

    return serverAddress;
}


void readFromSocket(int socket, char* message)
{
    int in;

    while ( (in = read(socket, message, DEFAULT_BUFFER_LEN - 1)) > 0)
    {
        message[in] = 0;
        logMsg(4, message);
    }

    return;
}

void writeToSocket(int socket, char* message)
{
    errno = 0;
    int writeStatus = write(socket, message, strlen(message));

    if(writeStatus == -1)
    {
        char addressString[DEFAULT_STRING_LEN];
        localAddressFromDescriptor(socket, addressString);

        logMsgAddr(2, "Unable to write to Socket", addressString);
    }
    else
    {
        char logMsgString[DEFAULT_BUFFER_LEN * 2];
        char outboundAddress[DEFAULT_STRING_LEN];

            remoteAddressFromDescriptor(socket, outboundAddress);
            snprintf(logMsgString, DEFAULT_BUFFER_LEN * 2, "Sent string (%ld bytes) to socket %s: %s",
                                    strlen(message),
                                    outboundAddress,
                                    message
            );

        logMsg(4, logMsgString);
    }

    return;
}


void sendMessage(int localSocket, char* outgoingMessage)
{
    send(localSocket, outgoingMessage, sizeof(outgoingMessage), 0);
}

void receiveMessage(int localSocket, char* incommingMessage)
{
    recv(localSocket, incommingMessage, sizeof(incommingMessage), 0);
}





//       ::::::::  :::::::::: :::::::::  :::     ::: :::::::::: :::::::::
//     :+:    :+: :+:        :+:    :+: :+:     :+: :+:        :+:    :+:
//    +:+        +:+        +:+    +:+ +:+     +:+ +:+        +:+    +:+
//   +#++:++#++ +#++:++#   +#++:++#:  +#+     +:+ +#++:++#   +#++:++#:
//         +#+ +#+        +#+    +#+  +#+   +#+  +#+        +#+    +#+
// #+#    #+# #+#        #+#    #+#   #+#+#+#   #+#        #+#    #+#
// ########  ########## ###    ###     ###     ########## ###    ###




//##############################################################################
//  Server creation
//##############################################################################


void bindAddressToSocket(int serverSocket, struct sockaddr_in serverAddress)
{
    errno = 0;
	int bindStatus = bind(serverSocket,
        (struct sockaddr*) &serverAddress,
        sizeof(serverAddress)
    );

    if(bindStatus == -1)
	{
		logMsg(1, "Error establishing connection");
        exit(EXIT_FAILURE);
	}
    else
    {
        char addressString[DEFAULT_STRING_LEN];
        addressFromStruct(serverAddress, addressString);
        logMsgAddr(5, "Server bound to socket", addressString);
    }

    return;
}


void createConnectionQueue(int serverSocket)
{
    errno = 0;
	int listenStatus = listen(serverSocket, 5);

    if(listenStatus == -1)
	{
        logMsg(1, "Unable to use the socket");
        exit(EXIT_FAILURE);
	}
    else
    {
        char addressString[DEFAULT_STRING_LEN];
        localAddressFromDescriptor(serverSocket, addressString);

        logMsgAddr(4, "Server started on socket", addressString);
    }

    return;
}


int startServer(int type, int serverIP, int serverPort)
{
    int serverSocket = createSocket(type);
    struct sockaddr_in serverAddress = defineServerAddress(serverIP, serverPort);
    bindAddressToSocket(serverSocket, serverAddress);
    createConnectionQueue(serverSocket);

    return serverSocket;
}

//##############################################################################
//  Server operations
//##############################################################################

int getConnectionFromQueue(int socket, struct sockaddr_in* clientAddress)
{
    int clientSocket;
    socklen_t sizeOfClientAddress = sizeof(clientAddress);
    errno = 0;

    clientSocket =  accept(socket, (struct sockaddr *) clientAddress, &sizeOfClientAddress);

    char addressString[DEFAULT_STRING_LEN];


    addressFromStruct(*clientAddress, addressString);


    if (clientSocket == -1)
    {
        logMsgAddr(2, "Unable to accept connection with client", addressString);
    }
    else
    {
        logMsgAddr(4, "Accepted connection with client", addressString);
    }

    return clientSocket;
}

void generateTimeIntMessage(char* outMessage)
{
    long int timeInt = currentTimeAsInt(); // + UNIX_TIME_COMPENSATION;
    printf("%ld", timeInt);
    snprintf(outMessage, DEFAULT_BUFFER_LEN, "%ld", timeInt);

    return;
}





//       ::::::::  :::        ::::::::::: :::::::::: ::::    ::: :::::::::::
//     :+:    :+: :+:            :+:     :+:        :+:+:   :+:     :+:
//    +:+        +:+            +:+     +:+        :+:+:+  +:+     +:+
//   +#+        +#+            +#+     +#++:++#   +#+ +:+ +#+     +#+
//  +#+        +#+            +#+     +#+        +#+  +#+#+#     +#+
// #+#    #+# #+#            #+#     #+#        #+#   #+#+#     #+#
// ########  ########## ########### ########## ###    ####     ###


int startClient(int type)
{

    int clientSocket = createSocket(type);
    char localSocketString[DEFAULT_STRING_LEN];

    localAddressFromDescriptor(clientSocket, localSocketString);

    logMsgAddr(4, "Created client on socket", localSocketString);

    return clientSocket;
}


struct sockaddr_in parseAddressInput(char* ipString, char* portString)
{
    int port = atoi(portString);
    int ip = inet_addr(ipString);

    if(!isInt(portString) || port <= 0)
    {
        logMsg(1, "Wrong port number");

        exit(EXIT_FAILURE);
    }

    if(ip < 0)
    {
        logMsg(1, "Wrong ip address");
        exit(EXIT_FAILURE);
    }

    return defineServerAddress(ip, port);
}



void clientConnectToServer(int clientSocket, struct sockaddr_in serverAddress)
{
    int attempt = 0;
    errno = 0;
    do
    {
	    int connectionStatus = connect(
            clientSocket,
            (struct sockaddr*) &serverAddress,
            sizeof(serverAddress)
        );

        if(connectionStatus == -1)
        {
            logMsg(2, "Error establishing connection");
        }
        ++attempt;
    } while (errno != 0 && attempt < MAX_CONNECT_ATTEMPTS);

    if (attempt >= MAX_CONNECT_ATTEMPTS)
    {
        char addressMessage[DEFAULT_STRING_LEN];
        addressFromStruct(serverAddress, addressMessage);
        logMsgAddr(1, "Failed to connect to server", addressMessage);
        exit(EXIT_FAILURE);
    }
    else
    {
        logMsg(4, "connected");
    }


    return;
}

void printHumanReadableTime(char* intStringTime)
{
    char humanReadable[DEFAULT_BUFFER_LEN];
    long int intTime = atoi(intStringTime) - UNIX_TIME_COMPENSATION;
    printf("%ld", intTime);
    timeToString(humanReadable, intTime);
    printf("\n\n\t %s \n\n", humanReadable);

    return;
}



