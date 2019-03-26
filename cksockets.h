#ifndef CKSOCKETS_H
#define CKSOCKETS_H

struct sockaddr_in parseAddressInput(char* ipString, char* portString);
int createSocket(int type);
void closeSocket (int networkSocket);
int startServer(int type, int serverIP, int serverPort);
int startClient(int type);
void clientConnectToServer(int clientSocket, struct sockaddr_in serverAddress);
int getConnectionFromQueue(int socket, struct sockaddr_in* clientAddress);

void readFromSocket(int socket, char* message);
void writeToSocket(int socket, char* message);

void generateTimeIntMessage(char* outMessage);
void printHumanReadableTime(char* intStringTime);

#endif