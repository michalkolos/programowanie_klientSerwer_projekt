#include <netinet/in.h> 	//	includes INADDR_ANY constant 
#include <stdio.h>

#include "cksockets.h"
#include "cksettings.h"

int main(int argc, char *argv[]) 
{
	if(argc != 3)
	{
		fprintf(stderr,"USAGE: %s <Server IP> <Port>\n", argv[0]);
		return 1;
	}


	struct sockaddr_in  serverAddress = parseAddressInput(argv[1], argv[2]);
	int clientSocket = createSocket(SOCK_STREAM);

	clientConnectToServer(clientSocket, serverAddress);

	char incommingMessage[DEFAULT_BUFFER_LEN];

	readFromSocket(clientSocket, incommingMessage);
	printHumanReadableTime(incommingMessage);
	closeSocket(clientSocket);

	// while(1)
	// {
	// 	fgets(outgoingMessage,DEFAULT_BUFFER_LEN,stdin);
	// 	printf("%s", outgoingMessage);	
	// 	writeToSocket(clientSocket, outgoingMessage);
	// }

	return 0;
}