#ifndef UTILS_H
#define UTILS_H


int isInt(const char *str);
void addressFromStruct(struct sockaddr_in address, char* addressString);
void localAddressFromDescriptor(int socketDescriptor, char* addressString);
void remoteAddressFromDescriptor(int socketDescriptor, char* addressString);
void timeToString(char* timeString, time_t t);
time_t currentTimeAsInt();



/**
 * @brief Loggs given message to both console and loggfile
 * 
 * @param logLevel - integer denominating the type of the message
 * @param message - body of the message 
 */
void logMsg(int logLevel, char* message);
void logMsgAddr(int logLevel, char* message, char* address);



#endif