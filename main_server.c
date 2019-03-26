#include <netinet/in.h> 	//	includes INADDR_ANY constant 
#include <stdio.h>
#include <stdlib.h>

#include "cksockets.h"
#include "cksettings.h"

int main(int argc, char *argv[]) 
{
		if(argc != 2)
	{
		fprintf(stderr,"USAGE: %s <Port>\n", argv[0]);
		return 1;
	}

	parseAddressInput("0.0.0.0", argv[1]);
	int serverSocket = startServer(TCP, INADDR_ANY, atoi(argv[0]));

	struct sockaddr_in clientAddress;

	// int clientSocket = getConnectionFromQueue(serverSocket, &clientAddress);
	// char incommingMessage[DEFAULT_BUFFER_LEN];
	
	char message[DEFAULT_BUFFER_LEN];

	while(1)
	{
		generateTimeIntMessage(message);	
		int clientSocket = getConnectionFromQueue(serverSocket, &clientAddress);
		writeToSocket(clientSocket, message);
		closeSocket(clientSocket);
	}
	

	return 0;
}