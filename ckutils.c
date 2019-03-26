#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h> 	// API and functionality for creating sockets
#include <netinet/in.h> 	// Structures storing address information
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "cksettings.h"


void logMsg(int logLevel, char* message);





//       :::::::: ::::::::::: :::::::::  ::::::::::: ::::    :::  ::::::::   :::::::: 
//     :+:    :+:    :+:     :+:    :+:     :+:     :+:+:   :+: :+:    :+: :+:    :+: 
//    +:+           +:+     +:+    +:+     +:+     :+:+:+  +:+ +:+        +:+         
//   +#++:++#++    +#+     +#++:++#:      +#+     +#+ +:+ +#+ :#:        +#++:++#++   
//         +#+    +#+     +#+    +#+     +#+     +#+  +#+#+# +#+   +#+#        +#+    
// #+#    #+#    #+#     #+#    #+#     #+#     #+#   #+#+# #+#    #+# #+#    #+#     
// ########     ###     ###    ### ########### ###    ####  ########   ########       


int isInt(const char *str) 
{
    if (*str == '-') {
        ++str;
    }

    if (!*str) {
        return 0;
    }

    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        } else {
            ++str;
        }
    }

    return 1;
}

void addressFromStruct(struct sockaddr_in address, char* addressString)
{
    sprintf(addressString, "%s:%d", inet_ntoa(address.sin_addr), address.sin_port);
    
    return;
}

void localAddressFromDescriptor(int socketDescriptor, char* addressString)
{
    struct sockaddr_in address;
    socklen_t addressSize = sizeof(struct sockaddr_in);
    

    errno = 0;
    int getsocknameStatus = getsockname(                 
        socketDescriptor, 
        (struct sockaddr*) &address, 
        &addressSize
    );

    if (getsocknameStatus == -1) 
    {
        logMsg(1, "Unable to get local address");
        exit(EXIT_FAILURE);
    }

    addressFromStruct(address, addressString);
    
    return ;
}

void remoteAddressFromDescriptor(int socketDescriptor, char* addressString)
{
    struct sockaddr_in address;
    socklen_t addressSize = sizeof(struct sockaddr_in);
    

    errno = 0;
    int getpeernameStatus = getpeername(                 
        socketDescriptor, 
        (struct sockaddr*) &address, 
        &addressSize
    );

    if (getpeernameStatus == -1) 
    {
        logMsg(1, "Unable to get remote address");
        exit(EXIT_FAILURE);
    }

    addressFromStruct(address, addressString);
    
    return ;
}



void appendStringToFile(char* message, char* filename)
{
    FILE *fileDescriptor = fopen("filename", "w");
    
    if (fileDescriptor == NULL)
    {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fileDescriptor, message);
    
    fclose(fileDescriptor);

    return;
}




//   ::::::::::: :::::::::::   :::   :::   :::::::::: 
//      :+:         :+:      :+:+: :+:+:  :+:         
//     +:+         +:+     +:+ +:+:+ +:+ +:+          
//    +#+         +#+     +#+  +:+  +#+ +#++:++#      
//   +#+         +#+     +#+       +#+ +#+            
//  #+#         #+#     #+#       #+# #+#             
// ###     ########### ###       ### ##########       



void timeToString(char* timeString, time_t t)
{
    struct tm currentTime = *localtime(&t);

    snprintf(timeString, DEFAULT_STRING_LEN, "%02d:%02d:%02d %02d-%02d-%04d ", 
                                    currentTime.tm_hour, 
                                    currentTime.tm_min, 
                                    currentTime.tm_sec,
                                    currentTime.tm_mday,
                                    currentTime.tm_mon + 1, 
                                    currentTime.tm_year + 1900 
                                     
                                    
    );

    return;
}

time_t currentTimeAsInt()
{
    errno = 0;
    time_t timeInSeconds = time(NULL);

    if (timeInSeconds == -1)
    {
        logMsg(1, "Error reading current time");
        exit(EXIT_FAILURE);
    }
    
    return timeInSeconds;
}




//       :::        ::::::::   ::::::::   :::::::: ::::::::::: ::::    :::  :::::::: 
//      :+:       :+:    :+: :+:    :+: :+:    :+:    :+:     :+:+:   :+: :+:    :+: 
//     +:+       +:+    +:+ +:+        +:+           +:+     :+:+:+  +:+ +:+         
//    +#+       +#+    +:+ :#:        :#:           +#+     +#+ +:+ +#+ :#:          
//   +#+       +#+    +#+ +#+   +#+# +#+   +#+#    +#+     +#+  +#+#+# +#+   +#+#    
//  #+#       #+#    #+# #+#    #+# #+#    #+#    #+#     #+#   #+#+# #+#    #+#     
// ########## ########   ########   ######## ########### ###    ####  ########       



//log levels
const char* logLabel[] = {
    "  [ALL]",    // 0
    "[FATAL]",    // 1
    "[ERROR]",    // 2
    " [WARN]",    // 3
    " [INFO]",    // 4
    "[DEBUG]"     // 5
};


void composeLogMessage(int logLevel, char* message, char* output)
{
    char timeString[DEFAULT_STRING_LEN];
    timeToString(timeString, time(NULL));

    strcpy(output, timeString);

    // char levelString[DEFAULT_STRING_LEN];

    switch (logLevel)
    {
        case 1:
        case 2:      
            if(errno > 0)
            {
                snprintf(output, DEFAULT_STRING_LEN * 4, "%s %s: %s | %s", 
                                                        logLabel[logLevel], 
                                                        timeString, message, 
                                                        strerror(errno)
                );
            }
            else
            {
                snprintf(output, DEFAULT_STRING_LEN * 4, "%s %s: %s", 
                                                        logLabel[logLevel], 
                                                        timeString, 
                                                        message
                );
            }
            
            break;

        case 3:
        case 4:
        case 5:
            snprintf(output, DEFAULT_STRING_LEN * 4, "%s %s: %s", 
                                                        logLabel[logLevel], 
                                                        timeString, 
                                                        message
            );
            break;

        default:
            snprintf(output, DEFAULT_STRING_LEN * 4, "%s %s: %s", 
                                                        logLabel[logLevel], 
                                                        timeString, 
                                                        message
            );
            break;
    }

    return;
}


void logMsg(int logLevel, char* message)
{
    char output[DEFAULT_STRING_LEN * 4];

    composeLogMessage(logLevel, message, output);

    if(LOG_FILTER == 0 || logLevel <= LOG_FILTER)
    {
        if(TO_CONSOLE)
        {
            printf(output);
            printf("\n");
        }

        if(TO_FILE)
        {
            //TODO: Implement logfile logging
        }
    }

    return;
}

void logMsgAddr(int logLevel, char* message, char* address)
{
    char output[DEFAULT_STRING_LEN];
    
    sprintf(output, "%s: %s", message, address);

    logMsg(logLevel, output);

    return;
}



